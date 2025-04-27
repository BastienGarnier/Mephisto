#include <mephisto/mesh/meshrenderer.hpp>

using namespace mephisto;

MeshRenderer::MeshRenderer(GraphicalApplication *app) : app(app) {
	Context* context = app->context();
	current_frame = 0;

	create_synchronization_objects(context->logical_device());
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	waitStages[0] = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.commandBufferCount = 1;

	submitInfo.pWaitDstStageMask = waitStages;

	command_buffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; // possibly SECONDARY
	allocInfo.commandPool = context->command_pool();
	allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;
	if (vkAllocateCommandBuffers(context->logical_device(), &allocInfo, command_buffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate command buffers !");
	}
}

void MeshRenderer::create_synchronization_objects(VkDevice device) {
	sync.semaphores_image_available.resize(MAX_FRAMES_IN_FLIGHT);
	sync.semaphores_render_finished.resize(MAX_FRAMES_IN_FLIGHT);
	sync.fences_in_flight.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &sync.semaphores_image_available[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create image available semaphore for a frame !");
		}

		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &sync.semaphores_render_finished[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create render finished semaphore for a frame !");
		}
		if (vkCreateFence(device, &fenceInfo, nullptr, &sync.fences_in_flight[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create in flight fence for a frame !");
		}
	}
}


void MeshRenderer::create_command_buffer() {
	Context* context = app->context();
	command_buffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; // possibly SECONDARY
	allocInfo.commandPool = context->command_pool();
	allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;
	if (vkAllocateCommandBuffers(context->logical_device(), &allocInfo, command_buffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate command buffers !");
	}
}

void MeshRenderer::record_command_buffer(VkCommandBuffer command_buffer, Mesh* m, VkPipeline pipeline) {
	vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

	VkBuffer vertexBuffers[] = {m->get_vbo().buffer()};
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(command_buffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(command_buffer, m->get_ibo().buffer(), 0, VK_INDEX_TYPE_UINT16);

	// on peut faire facilement de l'instancing en modifiant la commande ci-dessous :
	vkCmdDrawIndexed(command_buffer, static_cast<uint32_t>(m->get_indices().size()), 1, 0, 0, 0);
	// vkCmdDraw(command_buffer, static_cast<uint32_t>(m->get_vertices().size()), 1, 0, 0); // vertices.size() vertices to draw, 1 instance to render, offset of first vertex, offset of first instance
}

void MeshRenderer::start() {
	query = (USetQuery*)app->entitymanager.query<Transform, Renderer>();
}

void MeshRenderer:: update() {
	Swapchain &swapchain = app->window()->swapchain();
	Context* context = app->context();

	vkWaitForFences(context->logical_device(), 1, &sync.fences_in_flight[current_frame], VK_TRUE, UINT64_MAX);
	
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(context->logical_device(), swapchain.get_swapchain(), UINT64_MAX, sync.semaphores_image_available[current_frame],VK_NULL_HANDLE, &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || frameresized) {
		if (app->window()->width() != 0 && app->window()->height() != 0) {
				swapchain.recreate(context, app->window()->surface());
				frameresized = false;
		}
		return;
	} else if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	VkCommandBuffer command_buffer = command_buffers[current_frame];

	vkResetFences(context->logical_device(), 1, &sync.fences_in_flight[current_frame]); // pour prochaine appel à waitforfences

	vkResetCommandBuffer(command_buffer, 0);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(command_buffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer !");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = context->render_pass();
	renderPassInfo.framebuffer = swapchain.get_framebuffer(imageIndex);

	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = swapchain.get_extent();

	VkClearValue clearColor[] = {
		{.color = {.float32 = {0.0f, 0.0f, 0.0f, 1.0f}}},
		{.depthStencil = {.depth = 1.0,.stencil = 0}}
	};
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = clearColor;

	vkCmdBeginRenderPass(command_buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapchain.get_extent().width);
	viewport.height = static_cast<float>(swapchain.get_extent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(command_buffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = swapchain.get_extent();
	vkCmdSetScissor(command_buffer, 0, 1, &scissor);

	for (auto itr = query->entities.cbegin(); itr != query->entities.cend(); itr++)
	{
		Entity *e = app->entitymanager.get_entity(*itr);
		Renderer *r = app->entitymanager.componentstorage.get_component<Renderer>(e);
		// Transform &t = app->entitymanager.componentstorage.get_component<Transform>(e);
		record_command_buffer(command_buffer, r->mesh, r->pipeline->pipeline());
	}
	vkCmdEndRenderPass(command_buffer);
	if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
		throw std::runtime_error("Failed to record command buffer !");
	}

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {sync.semaphores_image_available[current_frame]};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &command_buffer;

	VkSemaphore signalSemaphores[] = {sync.semaphores_render_finished[current_frame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(context->graphics_queue(), 1, &submitInfo, sync.fences_in_flight[current_frame]) != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit draw command buffer !");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &(sync.semaphores_render_finished[current_frame]);

	VkSwapchainKHR swapChains[] = {swapchain.get_swapchain()};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional
	if (vkQueuePresentKHR(context->present_queue(), &presentInfo)!= VK_SUCCESS) {
		std::cerr << "Failed to present queue KHR" << std::endl;
	}

	current_frame = (current_frame + 1) & 1; // car MAX_FRAMES_IN_FLIGHT = 2
}

void MeshRenderer::stop() {

}

void MeshRenderer::destroy() {
	Context* context = app->context();
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(context->logical_device(), sync.semaphores_image_available[i], nullptr);
		vkDestroySemaphore(context->logical_device(), sync.semaphores_render_finished[i], nullptr);
		vkDestroyFence(context->logical_device(), sync.fences_in_flight[i], nullptr);
	}
}
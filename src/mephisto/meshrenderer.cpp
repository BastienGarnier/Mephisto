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

	// on peut faire facilement de l'instancing en modifiant la commande ci-dessous :
	vkCmdDraw(command_buffer, static_cast<uint32_t>(m->get_vertices().size()), 1, 0, 0); // vertices.size() vertices to draw, 1 instance to render, offset of first vertex, offset of first instance
}
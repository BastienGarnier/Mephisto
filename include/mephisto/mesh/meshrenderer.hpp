#ifndef _MEPHISTO_MESHRENDERER_HPP_INCLUDED
#define _MEPHISTO_MESHRENDERER_HPP_INCLUDED

#include <mephisto/application/graphicalapplication.hpp>
#include <mephisto/processus.hpp>

#include <mephisto/renderer.hpp>
#include <mephisto/transform.hpp>
#include <mephisto/window/window.hpp>

#include <mephisto/queries/usetquery.hpp>

namespace mephisto {
	class MeshRenderer : Processus
	{
	public:
		MeshRenderer(GraphicalApplication *app);
		~MeshRenderer();

		static constexpr unsigned int MAX_FRAMES_IN_FLIGHT = 2;

		void start() {
			query = (USetQuery*)app->entitymanager.query<Transform, Renderer>();
		}

		void update() {
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

		void stop() {
			Context* context = app->context();
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				vkDestroySemaphore(context->logical_device(), sync.semaphores_image_available[i], nullptr);
				vkDestroySemaphore(context->logical_device(), sync.semaphores_render_finished[i], nullptr);
				vkDestroyFence(context->logical_device(), sync.fences_in_flight[i], nullptr);
			}
		}

		void create_synchronization_objects(VkDevice device);
		void create_command_buffer();
		void record_command_buffer(VkCommandBuffer command_buffer, Mesh* m, VkPipeline pipeline);

		uint32_t current_frame;	
		GraphicalApplication* app;
		USetQuery* query;

		VkSubmitInfo submitInfo;
		VkSemaphore waitSemaphores[1];
		VkPipelineStageFlags waitStages[1];
		VkSemaphore signalSemaphores[1];

		// GPU Synchronization
		struct 
		{
			std::vector<VkSemaphore> semaphores_image_available;
			std::vector<VkSemaphore> semaphores_render_finished;
			std::vector<VkFence> fences_in_flight;
		} sync;

		std::vector<VkCommandBuffer> command_buffers;

		bool frameresized;
	};
}

#endif
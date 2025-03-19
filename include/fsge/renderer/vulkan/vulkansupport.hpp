#ifndef _FSGE_VULKANRENDERER_HPP_INCLUDED
#define _FSGE_VULKANRENDERER_HPP_INCLUDED

#include <vulkan/vulkan.h>

#if defined(__linux__)
#include <xcb/xcb.h>
#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan_xcb.h>
#endif

#include <fsge/system/window.hpp>
#include <fsge/core/components/components.hpp>
#include <fsge/renderer/vulkan/swapchain.hpp>

namespace fsge {
	class VulkanSupport
	{
		RegisterAsComponent(VulkanSupport);
		public:
			VulkanSupport() {
				
			}
			~VulkanSupport() {

			}

			void destroy(VkDevice device, VkInstance instance) {
				for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
					vkDestroySemaphore(device, _semaphores_image_available[i], nullptr);
					vkDestroySemaphore(device, _semaphores_render_finished[i], nullptr);
					vkDestroyFence(device, _fences_in_flight[i], nullptr);
				}
				_swapchain.cleanup(device);
				vkDestroySurfaceKHR(instance, _surface, nullptr);
			}

			VkSurfaceKHR surface() {
				return _surface;
			}

			static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

			void create_surface(xcb_connection_t* connection, xcb_window_t winref, VkInstance instance);
			void create_synchronization_objects(VkDevice device);
			void create_swapchain(VkPhysicalDevice physical_device, VkDevice logical_device);
		private:
			VkSurfaceKHR _surface;

			// Vulkan Swapchain
			Swapchain _swapchain;
			
			// GPU Synchronization
			std::vector<VkSemaphore> _semaphores_image_available;
			std::vector<VkSemaphore> _semaphores_render_finished;
			std::vector<VkFence> _fences_in_flight;
	};
}

#endif
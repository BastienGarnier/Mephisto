#ifndef _MEPHISTO_VULKANCONTEXT_HPP_INCLUDED
#define _MEPHISTO_VULKANCONTEXT_HPP_INCLUDED

#include <map>
#include <set>
#include <iostream>

#include <vulkan/vulkan.h>

#include <mephisto/ressources/ressources.hpp>
#include <mephisto/vulkan/queuefamily.hpp>
#include <mephisto/vulkan/swapchainsupport.hpp>

namespace mephisto {
	namespace vulkan {
		class Context
		{
		RegisterAsRessource(Context);
		public:
			Context() : initialized(false) {
				create_instance();
			}
			~Context() {
			}

			void initialize(VkSurfaceKHR surface) {
				setup_debug_messenger();  // indépendant de fenêtre
				pick_physical_device(surface); // indépendant de fenêtre (seulement de système de fenêtrage)
				create_logical_device();  // indépendant de fenêtre
				create_render_pass();
				create_command_pool();
				initialized = true;
			}

			void destroy() {
				vkDestroyCommandPool(_logical_device, _command_pool, nullptr);
				vkDestroyCommandPool(_logical_device, _short_command_pool, nullptr);
				vkDestroyRenderPass(_logical_device, _render_pass, nullptr);
				vkDestroyDevice(_logical_device, nullptr);
				vkDestroyInstance(_instance, nullptr);
			}
			
			bool initialized;

			inline VkInstance instance() {
				return _instance;
			}

			inline VkPhysicalDevice physical_device() {
				return _physical_device;
			}

			inline VkDevice logical_device() {
				return _logical_device;
			}

			inline VkRenderPass render_pass() {
				return _render_pass;
			}

			inline VkFormat format() {
				return _surface_format.format;
			}

			inline VkCommandPool command_pool() {
				return _command_pool;
			}

			inline VkCommandPool short_command_pool() {
				return _short_command_pool;
			}

			inline VkQueue graphics_queue() {
				return _graphics_queue;
			}

			inline VkQueue present_queue() {
				return _present_queue;
			}

		private:
			VkInstance _instance;
			VkPhysicalDevice _physical_device;
			VkDevice _logical_device;
			VkQueue _graphics_queue;
			VkQueue _present_queue;
			QueueFamilyIndices _qfi;
			VkRenderPass _render_pass;
			SwapChainSupportDetails _swapchain_support;
			VkSurfaceFormatKHR _surface_format;
			VkCommandPool _command_pool;
			VkCommandPool _short_command_pool;

			bool check_device_extensions(VkPhysicalDevice device);
			int rate_device_suitability(VkPhysicalDevice device, VkSurfaceKHR surface);
			void create_render_pass();
			void create_instance(); 
			void setup_debug_messenger();
			void pick_physical_device(VkSurfaceKHR surface); 
			void create_logical_device();
			void create_command_pool();
		};
	}
}

#endif
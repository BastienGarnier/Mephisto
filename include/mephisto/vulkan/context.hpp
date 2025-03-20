#ifndef _MEPHISTO_VULKANCONTEXT_HPP_INCLUDED
#define _MEPHISTO_VULKANCONTEXT_HPP_INCLUDED

#include <map>
#include <set>

#include <vulkan/vulkan.h>

#include <mephisto/ressources/ressources.hpp>
#include <mephisto/vulkan/queuefamily.hpp>
#include <mephisto/vulkan/swapchain.hpp>

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
				initialized = true;
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

		private:
			VkInstance _instance;
			VkPhysicalDevice _physical_device;
			VkDevice _logical_device;
			VkQueue _graphics_queue;
			VkQueue _present_queue;
			QueueFamilyIndices _qfi;

			bool check_device_extensions(VkPhysicalDevice device);
			int rate_device_suitability(VkPhysicalDevice device, VkSurfaceKHR surface);

			void create_instance(); 
			void setup_debug_messenger();
			void pick_physical_device(VkSurfaceKHR surface); 
			void create_logical_device();
		};
	}
}

#endif
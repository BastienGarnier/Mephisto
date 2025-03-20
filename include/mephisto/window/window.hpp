#ifndef _MEPHISTO_SYSTEM_WINDOW_HPP_INCLUDED
#define _MEPHISTO_SYSTEM_WINDOW_HPP_INCLUDED

#include <iostream>
#include <cstring>
#include <string>

extern "C" {
	#include <vulkan/vulkan.h>
	#if defined (_WIN32)
	#elif defined(__linux__)
		#include <xcb/xcb.h>
		#include <xcb/sync.h>
		#include <mephisto/input/xkb.h>
	#elif defined(__ANDROID__)
	#endif
}

#include <mephisto/ressources/ressources.hpp>
#include <mephisto/input/inputevent.hpp>

#include <mephisto/vulkan/swapchain.hpp>

namespace mephisto {
	using namespace vulkan;
	class Window
	{
		RegisterAsRessource(Window);
		public:
		enum Flags {
			NoFlags = 0x0,
			FullScreen = 0x1,
			Resizable = 0x2
		};

			Window() {
			}
			~Window() {
				
			}

			xcb_connection_t* connection() {
				return _connection;
			}

			xcb_window_t window() {
				return _winref;
			}

			void destroy(VkDevice device, VkInstance instance) {
				#if defined(__linux__)
				xcb_destroy_window(_connection, _winref);
				xcb_disconnect(_connection);
				#endif
				for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
					vkDestroySemaphore(device, _vulkan.semaphores_image_available[i], nullptr);
					vkDestroySemaphore(device, _vulkan.semaphores_render_finished[i], nullptr);
					vkDestroyFence(device, _vulkan.fences_in_flight[i], nullptr);
				}
				_vulkan.swapchain.cleanup(device);
				vkDestroySurfaceKHR(instance, _vulkan.surface, nullptr);
			}
			Window(unsigned short width, unsigned short height, const char *caption, Flags flags, VkInstance instance);
			
			unsigned short int width();
			void set_width(unsigned short int width);
			unsigned short int height();
			void set_height(unsigned short int height);
			void rename(const char *caption);

			void update_events();
			
			const char* caption;
			InputEvent inputevent;

			VkSurfaceKHR surface() {
				return _vulkan.surface;
			}

			static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

			void initialize(VkPhysicalDevice physical_device, VkDevice logical_device) {
				create_synchronization_objects(logical_device);
				create_swapchain(physical_device, logical_device);
			}
		
		private:
			void create_surface(VkInstance instance);
			void create_synchronization_objects(VkDevice device);
			void create_swapchain(VkPhysicalDevice physical_device, VkDevice logical_device);

			uint32_t _width;
			uint32_t _height;

			// System support
			#if defined(__linux__)
			xcb_connection_t* _connection;
			xcb_window_t _winref;
			xcb_screen_t* _screen;
			const xcb_setup_t* _setup;
			struct AtomProperties {
				xcb_atom_t wm_protocols;
				xcb_atom_t wm_delete_window;
				xcb_atom_t _net_wm_sync_request;
				xcb_atom_t _net_wm_sync_request_counter;
				unsigned int active_properties_count;
			} _props;
			struct Sync {
				xcb_sync_counter_t counter;
				xcb_sync_int64_t value;
			} _sync;
			#endif


			// Graphics support
			struct {
				VkSurfaceKHR surface;
				
				// Vulkan Swapchain
				Swapchain swapchain;
					
				// GPU Synchronization
				std::vector<VkSemaphore> semaphores_image_available;
				std::vector<VkSemaphore> semaphores_render_finished;
				std::vector<VkFence> fences_in_flight;
			} _vulkan;
	};
}
#endif
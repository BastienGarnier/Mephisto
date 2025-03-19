#pragma once

#include <iostream>
#include <map>
#include <set>

#include <fsge/core/processus.hpp>
#include <fsge/core/application.hpp>

#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan.h>
#if defined (_WIN32)
    #include <vulkan/vulkan_win32.h>
#elif defined(__linux__)
	#include <xcb/xcb.h>
	#include <xcb/sync.h>
    #include <vulkan/vulkan_xcb.h>
#elif defined(__ANDROID__)
    #include <vulkan/vulkan_android.h>
#endif

#include <fsge/system/window.hpp>

#include <fsge/renderer/vulkan/vulkansupport.hpp>
#include <fsge/renderer/vulkan/queuefamily.hpp>
#include <fsge/renderer/vulkan/swapchain.hpp>

#include <fsge/core/queries/usetquery.hpp>

namespace fsge {
	class WindowHandler : Processus
	{
		public:
		WindowHandler(Application &a) : app(a) {

		}

		~WindowHandler() {
		}

		void start() {
			query = (USetQuery*)app.entitymanager.query<Window, VulkanSupport>();

			create_instance(); // indépendant de fenêtre (seulement de système de fenêtrage)

			VkSurfaceKHR surface;
			bool is_init = false;

			for (auto itr = query->entities.cbegin(); itr != query->entities.cend(); itr++) {
				EntityId wid = (*itr);
				Entity &e = app.entitymanager.get_entity(wid);
				Window &wref = app.entitymanager.componentstorage.get_component<Window>(e);
				VulkanSupport &vsref = app.entitymanager.componentstorage.get_component<VulkanSupport>(e);
				
				vsref.create_surface(wref.connection(), wref.window(), _vulkan.instance); // dépendant de fenêtre
				if (!is_init) {
					is_init = true;
					surface = vsref.surface();
				}
			}

			if (!is_init) {
				throw std::runtime_error("No windows to create surfaces !");
			}
			
			setup_debug_messenger();  // indépendant de fenêtre
			pick_physical_device(surface); // indépendant de fenêtre (seulement de système de fenêtrage)

			create_logical_device();  // indépendant de fenêtre

			for (auto itr = query->entities.cbegin(); itr != query->entities.cend(); itr++) {
				EntityId wid = (*itr);
				Entity &e = app.entitymanager.get_entity(wid);
				// Window &wref = app.entitymanager.componentstorage.get_component<Window>(e);
				VulkanSupport &vsref = app.entitymanager.componentstorage.get_component<VulkanSupport>(e);

				vsref.create_synchronization_objects(_vulkan.device); // faut en refaire pour chaque fenêtre -> dépendant de fenêtre
				vsref.create_swapchain(_vulkan.physical_device, _vulkan.device); // dépendant de fenêtre	
			}

			std::cout << "Finished Vulkan initialization" << std::endl;
		}

		void update() {
			for (auto itr = query->entities.cbegin(); itr != query->entities.cend(); ) {
				EntityId wid = (*itr);
				Entity &e = app.entitymanager.get_entity(wid);
				Window &wref = app.entitymanager.componentstorage.get_component<Window>(e);

				wref.update_events();

				if (wref.inputevent.type & InputEvent::Closed) { // supprime la fenêtre si reçoit l'évènemment pour quitter
					app.entitymanager.remove_entity<Window>(*(itr++));
				} else {
					++itr;
				}
			}
			if (query->empty()) {
				app.close();
			}
		}

		void stop() {
			vkDeviceWaitIdle(_vulkan.device);
			for (auto itr = query->entities.cbegin(); itr != query->entities.cend(); itr++) {
				EntityId wid = (*itr);
				Entity &e = app.entitymanager.get_entity(wid);
				Window &wref = app.entitymanager.componentstorage.get_component<Window>(e);
				// VulkanSupport &vsref = app.entitymanager.componentstorage.get_component<VulkanSupport>(e);

				wref.destroy();
			}

			for (auto itr = query->entities.cbegin(); itr != query->entities.cend(); itr++) {
				EntityId wid = (*itr);
				Entity &e = app.entitymanager.get_entity(wid);
				// Window &wref = app.entitymanager.componentstorage.get_component<Window>(e);
				VulkanSupport &vsref = app.entitymanager.componentstorage.get_component<VulkanSupport>(e);

				vsref.destroy(_vulkan.device, _vulkan.instance);
			}
			vkDestroyDevice(_vulkan.device, nullptr);
			vkDestroyInstance(_vulkan.instance, nullptr);
		}
		Application &app;
	private:
		USetQuery* query;

		bool check_device_extensions(VkPhysicalDevice device);
		int rate_device_suitability(VkPhysicalDevice device, VkSurfaceKHR surface);

		void create_instance(); 
		void setup_debug_messenger();
		void pick_physical_device(VkSurfaceKHR surface); 
		void create_logical_device();
		 

		struct {
			VkInstance instance;
			VkPhysicalDevice physical_device;
			VkDevice device;
			VkQueue graphics_queue;
			VkQueue present_queue;
			QueueFamilyIndices qfi;
		} _vulkan;
	};
}

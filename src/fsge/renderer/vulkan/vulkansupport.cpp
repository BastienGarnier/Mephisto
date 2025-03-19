#include <fsge/renderer/vulkan/vulkansupport.hpp>

using namespace fsge;

void VulkanSupport::create_surface(xcb_connection_t* connection, xcb_window_t winref, VkInstance instance) {
	#if defined(__linux__)
	VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.pNext = NULL;
	surfaceCreateInfo.flags = 0;
	surfaceCreateInfo.connection = connection;
	surfaceCreateInfo.window = winref;
	VkResult result = vkCreateXcbSurfaceKHR(instance, &surfaceCreateInfo, NULL, &_surface);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to create XCB surface");
	}
	#endif
}

void VulkanSupport::create_synchronization_objects(VkDevice device) {
	_semaphores_image_available.resize(MAX_FRAMES_IN_FLIGHT);
	_semaphores_render_finished.resize(MAX_FRAMES_IN_FLIGHT);
	_fences_in_flight.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &_semaphores_image_available[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create image available semaphore for a frame !");
		}

		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &_semaphores_render_finished[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create render finished semaphore for a frame !");
		}
		if (vkCreateFence(device, &fenceInfo, nullptr, &_fences_in_flight[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create in flight fence for a frame !");
		}
	}
}


void VulkanSupport::create_swapchain(VkPhysicalDevice physical_device, VkDevice logical_device) {
	_swapchain.create(physical_device, logical_device, _surface);
}
#include <mephisto/vulkan/swapchainsupport.hpp>

using namespace mephisto;
using namespace vulkan;

VkSurfaceFormatKHR SwapChainSupportDetails::choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats) {
	for (const auto& available_format : available_formats) {
		if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return available_format;
		}
	}
	return available_formats[0];
}

VkPresentModeKHR SwapChainSupportDetails::choose_swap_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	#if defined(__ANDROID__)
		return VK_PRESENT_MODE_FIFO_KHR; // consomme moins d'énergie que VK_PRESENT_MODE_MAILBOX_KHR -> bien pour mobile
	#else
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}
		return VK_PRESENT_MODE_FIFO_KHR; 
	#endif
}

VkExtent2D SwapChainSupportDetails::choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities) { // swap extent : résolution des images de la swap chain
	return capabilities.currentExtent;
}

SwapChainSupportDetails SwapChainSupportDetails::query_swap_chain_support_details(VkPhysicalDevice device, VkSurfaceKHR surface) {
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
	
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface,&formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

#pragma once
#ifndef _MEPHISTO_SWAPCHAINSUPPORT_HPP_INCLUDED
#define _MEPHISTO_SWAPCHAINSUPPORT_HPP_INCLUDED

#include <vector>
#include <vulkan/vulkan.h>

namespace mephisto{
	namespace vulkan {
		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;

			static SwapChainSupportDetails query_swap_chain_support_details(VkPhysicalDevice device, VkSurfaceKHR surface);
			static VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats);
			static VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			static VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities);

		};
	}
}

#endif

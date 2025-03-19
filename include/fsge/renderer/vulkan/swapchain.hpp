#ifndef _FSGE_SWAPCHAIN_HPP_INCLUDED
#define _FSGE_SWAPCHAIN_HPP_INCLUDED

#include <vector>
#include <iostream>

#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan.h>
#if defined (_WIN32)
    #include <vulkan/vulkan_win32.h>
#elif defined(__linux__)
    #include <vulkan/vulkan_xcb.h>
#elif defined(__ANDROID__)
    #include <vulkan/vulkan_android.h>
#endif

#include "fsge/renderer/vulkan/queuefamily.hpp"

namespace fsge {
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class Swapchain
	{
	public:
		Swapchain();
		~Swapchain();

		void create(VkPhysicalDevice physical_device, VkDevice logical_device, VkSurfaceKHR surface);
		void recreate(VkPhysicalDevice physical_device, VkDevice logical_device, VkSurfaceKHR surface);	

		static SwapChainSupportDetails query_swap_chain_support_details(VkPhysicalDevice device, VkSurfaceKHR surface);
		static VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats);
		static VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		static VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities);

		VkFormat get_surface_format();
		VkExtent2D get_extent();
		VkFramebuffer get_framebuffer(uint32_t image_index);
		VkSwapchainKHR get_swapchain();

		void cleanup(VkDevice logical_device);
	private:
		void create_render_pass(VkDevice device);
		void create_swap_chain(VkPhysicalDevice physical_device, VkDevice logical_device, VkSurfaceKHR surface);
		void create_image_views(VkDevice logical_device);
		
		void create_framebuffers(VkDevice logical_device);

		VkSwapchainKHR m_swap_chain;
		std::vector<VkImage> m_swap_chain_images;
		VkFormat m_swap_chain_surface_format;
		VkExtent2D m_swap_chain_extent;
		std::vector<VkImageView> m_swap_chain_image_views;
		VkRenderPass m_render_pass;
		std::vector<VkFramebuffer> m_swap_chain_framebuffers;	
	};
}

#endif
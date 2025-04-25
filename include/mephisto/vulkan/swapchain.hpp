#ifndef _MEPHISTO_SWAPCHAIN_HPP_INCLUDED
#define _MEPHISTO_SWAPCHAIN_HPP_INCLUDED

#include <vector>
#include <iostream>

#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan.h>
#include <xcb/xcb.h>
#if defined (_WIN32)
    #include <vulkan/vulkan_win32.h>
#elif defined(__linux__)
    #include <vulkan/vulkan_xcb.h>
#elif defined(__ANDROID__)
    #include <vulkan/vulkan_android.h>
#endif

#include <mephisto/vulkan/context.hpp>
#include <mephisto/vulkan/queuefamily.hpp>
#include <mephisto/vulkan/swapchainsupport.hpp>

namespace mephisto {
	namespace vulkan {

		class Swapchain
		{
		public:
			Swapchain();
			~Swapchain();

			void create(Context* context, VkSurfaceKHR surface);
			void recreate(Context* context, VkSurfaceKHR surface);	

			
			VkFormat get_surface_format();
			VkExtent2D get_extent();
			VkFramebuffer get_framebuffer(uint32_t image_index);
			VkSwapchainKHR get_swapchain();

			void cleanup(VkDevice logical_device);
		private:
			void create_swap_chain(VkPhysicalDevice physical_device, VkDevice logical_device, VkSurfaceKHR surface);
			void create_image_views(VkDevice logical_device);
			
			void create_framebuffers(VkDevice logical_device, VkRenderPass render_pass);

			VkSwapchainKHR m_swap_chain;
			std::vector<VkImage> m_swap_chain_images;
			VkFormat m_swap_chain_surface_format;
			VkExtent2D m_swap_chain_extent;
			std::vector<VkImageView> m_swap_chain_image_views;
			std::vector<VkFramebuffer> m_swap_chain_framebuffers;	
		};
	}
}

#endif
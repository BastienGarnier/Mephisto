#include "fsge/renderer/vulkan/swapchain.hpp"

using namespace fsge;

Swapchain::Swapchain() {

}

Swapchain::~Swapchain() {

}

void Swapchain::recreate(VkPhysicalDevice physical_device, VkDevice logical_device, VkSurfaceKHR surface) {
	vkDeviceWaitIdle(logical_device);
	cleanup(logical_device);
	create_swap_chain(physical_device, logical_device, surface);
	create_image_views(logical_device);
	create_render_pass(logical_device);
	create_framebuffers(logical_device);
}
VkSurfaceFormatKHR Swapchain::choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats) {
	for (const auto& available_format : available_formats) {
		if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return available_format;
		}
	}
	return available_formats[0];
}

VkPresentModeKHR Swapchain::choose_swap_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
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

VkExtent2D Swapchain::choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities) { // swap extent : résolution des images de la swap chain
	return capabilities.currentExtent;
}


void Swapchain::create(VkPhysicalDevice physical_device, VkDevice logical_device, VkSurfaceKHR surface) {
	create_swap_chain(physical_device, logical_device, surface);
	create_image_views(logical_device);
	create_render_pass(logical_device);
	create_framebuffers(logical_device);
}

VkFormat Swapchain::get_surface_format() {
	return m_swap_chain_surface_format;
}
VkExtent2D Swapchain::get_extent() {
	return m_swap_chain_extent;
}
VkFramebuffer Swapchain::get_framebuffer(uint32_t image_index) {
	return m_swap_chain_framebuffers[image_index];
}
VkSwapchainKHR Swapchain::get_swapchain() {
	return m_swap_chain;
}

void Swapchain::create_framebuffers(VkDevice logical_device) {
	m_swap_chain_framebuffers.resize(m_swap_chain_image_views.size());
	for (size_t i = 0; i < m_swap_chain_image_views.size(); i++) {
		VkImageView attachments[] = {
			m_swap_chain_image_views[i]
		};
		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = m_render_pass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = m_swap_chain_extent.width;
		framebufferInfo.height = m_swap_chain_extent.height;
		framebufferInfo.layers = 1;
		if (vkCreateFramebuffer(logical_device, &framebufferInfo, nullptr, &m_swap_chain_framebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create framebuffer !");
		}
	}
}

void Swapchain::cleanup(VkDevice logical_device) {
	vkDestroyRenderPass(logical_device, m_render_pass, nullptr);
	for (auto framebuffer : m_swap_chain_framebuffers) {
		vkDestroyFramebuffer(logical_device, framebuffer, nullptr);
	}
	for (auto image_view : m_swap_chain_image_views) {
		vkDestroyImageView(logical_device, image_view, nullptr);
	}
	vkDestroySwapchainKHR(logical_device, m_swap_chain, nullptr);
}


void Swapchain::create_swap_chain(VkPhysicalDevice physical_device, VkDevice logical_device, VkSurfaceKHR surface) {
	SwapChainSupportDetails swap_chain_support = Swapchain::query_swap_chain_support_details(physical_device, surface);

	// std::cout << "Min Image Count : " << swap_chain_support.capabilities.minImageCount << std::endl;
	// std::cout << "Max Image Count : " << swap_chain_support.capabilities.maxImageCount << std::endl;
	// std::cout << "Min Image Extent (width) : " << swap_chain_support.capabilities.minImageExtent.width << std::endl;
	// std::cout << "Min Image Extent (height) : " << swap_chain_support.capabilities.minImageExtent.height << std::endl;
	// std::cout << "Max Image Extent (width) : " << swap_chain_support.capabilities.maxImageExtent.width << std::endl;
	// std::cout << "Max Image Extent (height) : " << swap_chain_support.capabilities.maxImageExtent.height << std::endl;

	VkSurfaceFormatKHR surface_format = choose_swap_surface_format(swap_chain_support.formats);
	m_swap_chain_surface_format = surface_format.format;
	VkPresentModeKHR present_mode = choose_swap_present_mode(swap_chain_support.presentModes);
	VkExtent2D extent = choose_swap_extent(swap_chain_support.capabilities);
	m_swap_chain_extent = extent;
	// std::cout << "Swapchain Extent width : " << extent.width << std::endl;
	// std::cout << "Swapchain Extent height : " << extent.height << std::endl;

	uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1; // nombre d'images dans la file de swap chain. Ajoute 1 pour laisser temps au périphérique de terminer op. internes
	if (swap_chain_support.capabilities.maxImageCount > 0 && image_count > swap_chain_support.capabilities.maxImageCount) {
		image_count = swap_chain_support.capabilities.maxImageCount;
	}

	// std::cout << "Image Count : " << image_count << std::endl;

	VkSwapchainCreateInfoKHR create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info.surface = surface;
	create_info.minImageCount = image_count;
	create_info.imageFormat = surface_format.format;
	create_info.imageColorSpace = surface_format.colorSpace;
	create_info.imageExtent = extent;
	create_info.imageArrayLayers = 1; // > 1 si application 3D stéréoscopique : une image est en 3D
	// VK_IMAGE_USAGE_TRANSFER_DST_BIT instead and use a memory operation to transfer the rendered image to a swap chain image (for post-processing)
	create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; 

	QueueFamilyIndices indices = find_queue_families(physical_device, surface);
	uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),
	indices.presentFamily.value()};
	if (indices.graphicsFamily != indices.presentFamily) {
		create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info.queueFamilyIndexCount = 2;
		create_info.pQueueFamilyIndices = queueFamilyIndices;
	} else {
		create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		create_info.queueFamilyIndexCount = 0; // Optional
		create_info.pQueueFamilyIndices = nullptr; // Optional
	}
	create_info.preTransform = swap_chain_support.capabilities.currentTransform; // on peut spécifier des trucs comme des rotations, ou des flips
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	create_info.presentMode = present_mode;
	create_info.clipped = VK_TRUE;
	create_info.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(logical_device, &create_info, nullptr, &m_swap_chain) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create swap chain");
	}

	vkGetSwapchainImagesKHR(logical_device, m_swap_chain, &image_count, nullptr);
	m_swap_chain_images.resize(image_count);
	vkGetSwapchainImagesKHR(logical_device, m_swap_chain, &image_count, m_swap_chain_images.data());
	// std::cout << "Swapchain image count : " << image_count << std::endl;


}

void Swapchain::create_image_views(VkDevice logical_device) {
	m_swap_chain_image_views.resize(m_swap_chain_images.size());
	for (size_t i = 0; i < m_swap_chain_images.size(); i++) {
		VkImageViewCreateInfo create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
 		create_info.image = m_swap_chain_images[i];
 		create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
 		create_info.format = m_swap_chain_surface_format;
 		create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
 		create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
 		create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
 		create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
 		create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		create_info.subresourceRange.baseMipLevel = 0;
		create_info.subresourceRange.levelCount = 1;
		create_info.subresourceRange.baseArrayLayer = 0;
		create_info.subresourceRange.layerCount = 1;
		if (vkCreateImageView(logical_device, &create_info, nullptr,&m_swap_chain_image_views[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image views !");
		}
	}
}

SwapChainSupportDetails Swapchain::query_swap_chain_support_details(VkPhysicalDevice device, VkSurfaceKHR surface) {
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

void Swapchain::create_render_pass(VkDevice device) {
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = m_swap_chain_surface_format; // -> même format pour toutes les fenêtres puisque même OS
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // efface le framebuffer avant le rendu
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // contenu affiché est stocké et pourra être lu plus tard
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo render_pass_info{};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.attachmentCount = 1;
	render_pass_info.pAttachments = &colorAttachment;
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass;
	render_pass_info.dependencyCount = 1;
	render_pass_info.pDependencies = &dependency;
	if (vkCreateRenderPass(device, &render_pass_info, nullptr, &m_render_pass) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create render pass !");
	}
}
#include "mephisto/vulkan/commandpool.hpp"

RegisterRessource(VkCommandPool);

void mephisto::vulkan::create_command_pool(Context *context, VkCommandPool* cp, VkCommandPoolCreateFlags flags) {
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = flags;
	poolInfo.queueFamilyIndex = context->qfi().graphicsFamily.value();
	if (vkCreateCommandPool(context->logical_device(), &poolInfo, nullptr, cp) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create Command Pool !");
	}
}

VkCommandPool* mephisto::vulkan::new_command_pool(Context *context, VkCommandPoolCreateFlags flags) {
	VkCommandPool* cp = new VkCommandPool;
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = flags;
	poolInfo.queueFamilyIndex = context->qfi().graphicsFamily.value();
	if (vkCreateCommandPool(context->logical_device(), &poolInfo, nullptr, cp) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create Command Pool !");
	}
	return cp;
}
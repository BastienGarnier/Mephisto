#include "mephisto/vulkan/buffer.hpp"

using namespace mephisto;
using namespace vulkan;


uint32_t find_memory_type(VkPhysicalDevice physical_device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physical_device, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}	
	}
	throw std::runtime_error("Failed to find suitable memory type !");
}

Buffer::Buffer(Context* context, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
: context(context), _size(size) {
	VkBufferCreateInfo buffer_info{};
	buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.size = size;
	buffer_info.usage = usage;
	buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(context->logical_device(), &buffer_info, nullptr, &_buffer) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create vertex buffer !");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(context->logical_device(), _buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = 
		find_memory_type(context->physical_device(), memRequirements.memoryTypeBits, properties);

	// TODO : limiter le nombre de vkAllocateMemory pour toutes les ressources avec un allocateur maison
	if (vkAllocateMemory(context->logical_device(), &allocInfo, nullptr, &_buffer_memory) != VK_SUCCESS) { 
		throw std::runtime_error("Failed to allocate vertex buffer memory !");
	}
	vkBindBufferMemory(context->logical_device(), _buffer, _buffer_memory, 0); // 0 : offset within region of memory

}

void Buffer::copy(const Buffer& b) {
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = b.context->short_command_pool();
	allocInfo.commandBufferCount = 1;
	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(b.context->logical_device(), &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = b._size;
	vkCmdCopyBuffer(commandBuffer, b._buffer, _buffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(context->graphics_queue(), 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(context->graphics_queue()); // TODO : optimiser avec une fence, pour permettre d'effectuer plusieurs transferts à la fois

	vkFreeCommandBuffers(context->logical_device(), context->short_command_pool(), 1, &commandBuffer);
}

void Buffer::fill(void* data) {
	void* tmp;
	vkMapMemory(context->logical_device(), _buffer_memory, 0, _size, 0, &tmp);
	memcpy(tmp, data, (size_t)_size);
	vkUnmapMemory(context->logical_device(), _buffer_memory);
}
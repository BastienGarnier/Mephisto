#pragma once
#ifndef _MEPHISTO_VULKAN_BUFFER_HPP_INCLUDED
#define _MEPHISTO_VULKAN_BUFFER_HPP_INCLUDED

extern "C" {
	#include <vulkan/vulkan.h>
	#include <string.h>
}

#include <mephisto/vulkan/context.hpp>

namespace mephisto {
	namespace vulkan {
		class Buffer
		{
		public:
			Buffer() : context(nullptr) {};
			Buffer(Context* context, VkCommandPool cp, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
			~Buffer() {}

			void copy(const Buffer& b);

			void fill(void* data);

			void destroy() {
				vkDestroyBuffer(context->logical_device(), _buffer, nullptr);
				vkFreeMemory(context->logical_device(), _buffer_memory, nullptr);
			}

			inline VkBuffer buffer() const {
				return _buffer;
			}

			Context* context;
		private:
			VkCommandPool _cp;
			VkDeviceSize _size;
			VkBuffer _buffer;
			VkDeviceMemory _buffer_memory;
		};
	}
}

#endif
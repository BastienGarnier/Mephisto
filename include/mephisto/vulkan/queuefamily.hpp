#ifndef _MEPHISTO_QUEUEFAMILY_HPP_INCLUDED
#define _MEPHISTO_QUEUEFAMILY_HPP_INCLUDED

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

namespace mephisto {
	namespace vulkan {
		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;
			bool is_complete() {
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

		QueueFamilyIndices find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface);
	}
}
#endif
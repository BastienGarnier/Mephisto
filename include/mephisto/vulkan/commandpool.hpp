#pragma once
#ifndef _MEPHISTO_VULKAN_COMMANDPOOL_HPP_INCLUDED
#define _MEPHISTO_VULKAN_COMMANDPOOL_HPP_INCLUDED

#include <iostream>

extern "C" {
	#include <vulkan/vulkan.h>
}

#include <mephisto/vulkan/context.hpp>
#include <mephisto/ressources/ressources.hpp>

namespace mephisto {
	namespace vulkan {
		void create_command_pool(Context *context, VkCommandPool* cp, VkCommandPoolCreateFlags flags);
		VkCommandPool* new_command_pool(Context *context, VkCommandPoolCreateFlags flags);
	}
}

#endif
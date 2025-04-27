#pragma once
#ifndef _MEPHISTO_GRAPHICS_PIPELINE_HPP_INCLUDED
#define _MEPHISTO_GRAPHICS_PIPELINE_HPP_INCLUDED

#include <vector>
#include <stdexcept>

#include <vulkan/vulkan.h>
#include <mephisto/ressources/filesystem.hpp>
#include <mephisto/mesh/vertex.hpp>
#include <mephisto/vulkan/context.hpp>


namespace mephisto {
	namespace vulkan {
		class GraphicsPipeline
		{
		public:
			GraphicsPipeline(Context* vk_context, const std::string& vert_filename, const std::string& frag_filename);
			~GraphicsPipeline();

			void destroy(Context* vk_context) {
				vkDestroyPipeline(vk_context->logical_device(), _graphics_pipeline, nullptr);
				vkDestroyPipelineLayout(vk_context->logical_device(), _pipeline_layout, nullptr);
			}

			VkPipelineLayout layout() {
				return _pipeline_layout;
			}

			VkPipeline pipeline() {
				return _graphics_pipeline;
			}
		private:
			VkShaderModule create_shader_module(Context* vk_context, const std::vector<char>& code);

			VkPipelineLayout _pipeline_layout;
			VkPipeline _graphics_pipeline;
		};
	}
}

#endif

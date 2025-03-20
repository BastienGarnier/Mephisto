#ifndef _MEPHISTO_GRAPHICS_PIPELINE_HPP_INCLUDED
#define _MEPHISTO_GRAPHICS_PIPELINE_HPP_INCLUDED

#include <stdexcept>
#include <vulkan/vulkan.h>

namespace mephisto {
	namespace vulkan {
		class GraphicsPipeline
		{
		public:
			GraphicsPipeline();
			~GraphicsPipeline();

			VkPipelineLayout layout() {
				return _pipeline_layout;
			}

			VkPipeline pipeline() {
				return _graphics_pipeline;
			}
		private:
			VkPipelineLayout _pipeline_layout;
			VkPipeline _graphics_pipeline;
		};
	}
}

#endif
#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <array>
#include <vulkan/vulkan.h>
/*
- float : VK_FORMAT_R32_SFLOAT
- vec2  : VK_FORMAT_R32G32_SFLOAT
- vec3  : VK_FORMAT_R32G32B32_SFLOAT
- vec4  : VK_FORMAT_R32G32B32A32_SFLOAT
et pour les entiers : idem mais (U/S)INT au lieu de (S)FLOAT
*/

namespace mephisto {
	namespace vulkan {
		class Vertex
		{
		public:
			Vertex();
			~Vertex();
			Vertex(float px, float py, float pz, float r, float g, float b);
			
			static VkVertexInputBindingDescription get_binding_description() {
				VkVertexInputBindingDescription binding_desc{};
				binding_desc.binding = 0;
				binding_desc.stride = sizeof(Vertex);
				binding_desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
				return binding_desc;
			}

			static std::array<VkVertexInputAttributeDescription, 2> get_attribute_descriptions() { // 2 car position + color, faudra voir pour en avoir un nombre quelconque
				std::array<VkVertexInputAttributeDescription, 2> attribute_desc{};

				attribute_desc[0].binding = 0;
				attribute_desc[0].location = 0;
				attribute_desc[0].format = VK_FORMAT_R32G32B32_SFLOAT;
				attribute_desc[0].offset = offsetof(Vertex, m_pos);
				attribute_desc[1].binding = 0;
				attribute_desc[1].location = 1;
				attribute_desc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
				attribute_desc[1].offset = offsetof(Vertex, m_color);
				return attribute_desc;
			}
		private:
			float m_pos[3];
			float m_color[3];
		};
	}
}

#endif
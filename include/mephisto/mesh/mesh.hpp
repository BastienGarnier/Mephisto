#ifndef _MEPHISTO_MESH_HPP_INCLUDED
#define _MEPHISTO_MESH_HPP_INCLUDED

#include <vector>
#include <stdexcept>

extern "C" {
	#include <vulkan/vulkan.h>
	#include <string.h>
}

#include <mephisto/ressources/ressources.hpp>

#include <mephisto/vulkan/context.hpp>
#include <mephisto/vulkan/buffer.hpp>
#include <mephisto/mesh/vertex.hpp>

namespace mephisto {
	namespace vulkan {
		class Mesh
		{
		RegisterAsRessource(Mesh);
		public:
			Mesh();
			Mesh(const char* filename);
			~Mesh();
			void destroy();

			std::vector<Vertex> get_vertices();
			Buffer get_vbo();
			void create_vertex_buffer(Context *context);
			void add_vertex(Vertex v);
			Context *context;
		private:
			Buffer _staging;
			Buffer _vbo;
			std::vector<Vertex> m_vertices;
		};
	}
}

#endif
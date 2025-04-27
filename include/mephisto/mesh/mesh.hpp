#pragma once
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
		public:
			Mesh();
			Mesh(const char* filename);
			~Mesh();
			void destroy();

			std::vector<Vertex> get_vertices();
			std::vector<uint16_t> get_indices();
			Buffer get_vbo();
			Buffer get_ibo();
			void create_vertex_buffer(Context *context);
			void create_index_buffer(Context *context);
			void add_vertex(Vertex v);
			void add_indice(uint16_t i);
			Context *context;
		private:
			Buffer _vbo; // Vertex Buffer Object
			Buffer _ibo; // Index Buffer Object
			std::vector<Vertex> _vertices;
			// uint16_t si on a que 65536 vertices uniques
			// vkCmdBindIndexBuffer à changer en conséquence dans le renderer
			std::vector<uint16_t> _indices; 
		};
	}
}

#endif
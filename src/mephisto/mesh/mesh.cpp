#include <mephisto/mesh/mesh.hpp>

using namespace mephisto;
using namespace vulkan;

Mesh::Mesh() {

}

Mesh::Mesh(const char* filename) { // depuis un fichier OBJ // TODO : étendre joliment à d'autres types
	
}

Mesh::~Mesh() {
}

void Mesh::destroy() {
	
	_staging.destroy();
	_vbo.destroy();
}

void Mesh::add_vertex(Vertex v) {
	m_vertices.push_back(v);
}

std::vector<Vertex> Mesh::get_vertices() {
	return m_vertices;
}

Buffer Mesh::get_vbo() {
	return _vbo;
}


void Mesh::create_vertex_buffer(Context* context) {
	VkDeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertices.size();

	_staging = Buffer(context, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	_staging.fill(m_vertices.data());

	_vbo = Buffer(context, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	
	_vbo.copy(_staging);	
}


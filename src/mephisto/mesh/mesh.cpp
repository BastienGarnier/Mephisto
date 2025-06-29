#include <mephisto/mesh/mesh.hpp>

using namespace mephisto;
using namespace vulkan;

RegisterRessource(Mesh);

Mesh::Mesh() {

}

Mesh::Mesh(const char* filename) { // depuis un fichier OBJ // TODO : étendre joliment à d'autres types
	
}

Mesh::~Mesh() {
}

void Mesh::destroy() {
	_ibo.destroy();
	_vbo.destroy();
}

void Mesh::add_vertex(Vertex v) {
	_vertices.push_back(v);
}

void Mesh::add_indice(uint16_t i) {
	_indices.push_back(i);
}

// TODO : optimiser pour être sûr qu'il n'y a pas de copie
std::vector<Vertex> Mesh::get_vertices() {
	return _vertices;
}

// TODO : optimiser pour être sûr qu'il n'y a pas de copie
std::vector<uint16_t> Mesh::get_indices() {
	return _indices;
}


Buffer Mesh::get_vbo() {
	return _vbo;
}

Buffer Mesh::get_ibo() {
	return _ibo;
}

void Mesh::create_index_buffer(Context *context, VkCommandPool cp) {
	VkDeviceSize bufferSize = sizeof(_indices[0]) * _indices.size();

	Buffer staging(context, cp, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	staging.fill(_indices.data());

	_ibo = Buffer(context, cp, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	
	_ibo.copy(staging);

	staging.destroy();
}

void Mesh::create_vertex_buffer(Context* context, VkCommandPool cp) {
	VkDeviceSize bufferSize = sizeof(_vertices[0]) * _vertices.size();

	Buffer staging(context, cp, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	staging.fill(_vertices.data());

	_vbo = Buffer(context, cp, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	
	_vbo.copy(staging);

	staging.destroy();
}


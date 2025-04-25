#include "mephisto/mesh/vertex.hpp"

using namespace mephisto;

vulkan::Vertex::Vertex() {

}
vulkan::Vertex::Vertex(float px, float py, float pz, float r, float g, float b) {
	m_pos[0] = px;
	m_pos[1] = py;
	m_pos[2] = pz;

	m_color[0] = r;
	m_color[1] = g;
	m_color[2] = b;
}

vulkan::Vertex::~Vertex() {

}

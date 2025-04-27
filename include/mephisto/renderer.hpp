#pragma once
#ifndef _MEPHISTO_RENDERER_HPP_INCLUDED
#define _MEPHISTO_RENDERER_HPP_INCLUDED

#include <mephisto/components/components.hpp>

#include <mephisto/vulkan/graphicspipeline.hpp>
#include <mephisto/mesh/mesh.hpp>

namespace mephisto {
	class Renderer
	{
	RegisterAsComponent(Renderer);
	public:
		Renderer() {
		 	pipeline = nullptr;
		 	mesh = nullptr;
		 }
		Renderer(vulkan::GraphicsPipeline* pipeline, vulkan::Mesh* mesh) {
		 	pipeline = pipeline;
		 	mesh = mesh;
		 }
		~Renderer() {}

		vulkan::GraphicsPipeline* pipeline;
		vulkan::Mesh* mesh;

	};
}

#endif
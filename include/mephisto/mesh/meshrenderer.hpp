#pragma once
#ifndef _MEPHISTO_MESHRENDERER_HPP_INCLUDED
#define _MEPHISTO_MESHRENDERER_HPP_INCLUDED

#include <vector>
#include <stdint.h>

#include <mephisto/application/graphicalapplication.hpp>
#include <mephisto/processus/processus.hpp>

#include <mephisto/renderer.hpp>
#include <mephisto/transform.hpp>

#include <mephisto/queries/usetquery.hpp>

namespace mephisto {

	class MeshRenderer : Processus
	{
	public:
		MeshRenderer(GraphicalApplication *app);
		~MeshRenderer() {
		}

		static constexpr unsigned int MAX_FRAMES_IN_FLIGHT = 2;

		void destroy();
		void start();
		void update();
		void stop();

		void create_synchronization_objects(VkDevice device);
		void create_command_buffer();
		void record_command_buffer(VkCommandBuffer command_buffer, Mesh* m, VkPipeline pipeline);

		uint32_t current_frame;	
		GraphicalApplication* app;
		USetQuery* query;

		VkSubmitInfo submitInfo;
		VkSemaphore waitSemaphores[1];
		VkPipelineStageFlags waitStages[1];
		VkSemaphore signalSemaphores[1];

		// GPU Synchronization
		struct 
		{
			std::vector<VkSemaphore> semaphores_image_available;
			std::vector<VkSemaphore> semaphores_render_finished;
			std::vector<VkFence> fences_in_flight;
		} sync;

		// devrait probablement pas être là
		std::vector<VkCommandBuffer> command_buffers;

		bool frameresized;
	};
}

#endif
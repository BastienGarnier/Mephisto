#include <iostream>

#include <mephisto/application/graphicalapplication.hpp>
#include <mephisto/vulkan/graphicspipeline.hpp>
#include <mephisto/vulkan/commandpool.hpp>
#include <mephisto/transform.hpp>
#include <mephisto/renderer.hpp>
#include <mephisto/entities/entity.hpp>
#include <mephisto/components/archetype.hpp>
#include <mephisto/queries/usetquery.hpp>
#include <mephisto/mesh/meshrenderer.hpp>

using namespace mephisto;

int main(int argc, const char *argv[])
{
	#ifndef NDEBUG
	std::cout << "=== Running in DEBUG MODE ===" << std::endl;
	try {
	#endif
		GraphicalApplication game;

		game.entitymanager.componentstorage.create_component_storage<Transform>();
		game.entitymanager.componentstorage.create_component_storage<Renderer>();

		Archetype_t aDisplayable = Archetype<Transform, Renderer>();

		game.entitymanager.add_query(aDisplayable, (EntityQuery*)(new USetQuery()));

		vulkan::GraphicsPipeline* test_pipeline = game.ressourcesmanager.add<vulkan::GraphicsPipeline>("test", 
				new vulkan::GraphicsPipeline(game.context(), "data/vert.spv", "data/frag.spv"));

		VkCommandPool *main_cp = game.ressourcesmanager.add<VkCommandPool>("main", vulkan::new_command_pool(game.context(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT));
		VkCommandPool *short_cp = game.ressourcesmanager.add<VkCommandPool>("transient", vulkan::new_command_pool(game.context(), VK_COMMAND_POOL_CREATE_TRANSIENT_BIT));

		vulkan::Mesh* m = game.ressourcesmanager.add<vulkan::Mesh>("triangle", new vulkan::Mesh());
		m->add_vertex(Vertex(-0.5, -0.5, 0.0, 0.0, 0.0, 1.0));
		m->add_vertex(Vertex(0.5, -0.5, 0.0, 1.0, 0.0, 0.0));
		m->add_vertex(Vertex(0.5, 0.5, 0.0, 0.0, 1.0, 0.0));
		m->add_vertex(Vertex(-0.5, 0.5, 0.0, 1.0, 1.0, 1.0));
		m->create_vertex_buffer(game.context(), *short_cp);

		m->add_indice(0);
		m->add_indice(1);
		m->add_indice(2);
		m->add_indice(2);
		m->add_indice(3);
		m->add_indice(0);

		m->create_index_buffer(game.context(), *short_cp);

		Entity *e = game.entitymanager.add_entity<Renderer>(aDisplayable); // TODO : ajouter Transform
		Renderer *r = game.entitymanager.componentstorage.get_component<Renderer>(e);
		r->pipeline = test_pipeline;
		r->mesh = m;

		MeshRenderer *mr = new MeshRenderer(&game);
		mr->query = (USetQuery*)game.entitymanager.query(aDisplayable);
		game.processusmanager.add((Processus*)mr);


		game.run();
		// C'est dégueulasse. Ya forcément moyen de fait plus propre
		vkQueueWaitIdle(game.context()->graphics_queue());
		vkQueueWaitIdle(game.context()->present_queue());

		game.entitymanager.componentstorage.destroy_component_storage<Transform>();
		game.entitymanager.componentstorage.destroy_component_storage<Renderer>();

		m->destroy();
		test_pipeline->destroy(game.context());
		vkDestroyCommandPool(game.context()->logical_device(), *main_cp, nullptr);
		vkDestroyCommandPool(game.context()->logical_device(), *short_cp, nullptr);

		game.processusmanager.destroy_all();
	#ifndef NDEBUG
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	#endif
	return EXIT_SUCCESS;
}
#include <iostream>

#include <mephisto/application/graphicalapplication.hpp>
#include <mephisto/vulkan/graphicspipeline.hpp>

#include <mephisto/vulkan/graphicspipeline.hpp>
#include <mephisto/transform.hpp>
#include <mephisto/renderer.hpp>
#include <mephisto/mesh/meshrenderer.hpp>

using namespace mephisto;

int main(int argc, const char *argv[])
{
	#ifdef DEBUG
	std::cout << "=== Running in DEBUG MODE ===" << std::endl;
	try {
	#endif
		GraphicalApplication game;

		// game.entitymanager -> gestion des entités et composants
		// game.ressourcesmanager -> gestion des ressources
		// game.processusmanager ? gestion des processus 

		game.entitymanager.componentstorage.create_component_storage<Transform>();
		game.entitymanager.componentstorage.create_component_storage<Renderer>();

		std::cout << Components::get_enum<Transform>() << std::endl;
		std::cout << Components::get_enum<Renderer>() << std::endl;

		game.entitymanager.add_query<Transform, Renderer>((EntityQuery*)(new USetQuery()));

		vulkan::GraphicsPipeline* test_pipeline = game.ressourcesmanager.add<vulkan::GraphicsPipeline>("test", 
				new vulkan::GraphicsPipeline(game.context(), "data/vert.spv", "data/frag.spv"));

		vulkan::Mesh* m = game.ressourcesmanager.add<vulkan::Mesh>("triangle", new vulkan::Mesh());
		m->add_vertex(Vertex(0.0, -0.5, 0.0, 1.0, 0.0, 0.0));
		m->add_vertex(Vertex(0.5, 0.5, 0.0, 0.0, 1.0, 0.0));
		m->add_vertex(Vertex(-0.5, 0.5, 0.0, 0.0, 0.0, 1.0));
		m->create_vertex_buffer(game.context());

		EntityId eid = game.entitymanager.add_entity<Renderer, Transform>(Passer<Renderer>(test_pipeline, m), Passer<Transform>());

		Entity *e = game.entitymanager.get_entity(eid);
		// Renderer *r = game.entitymanager.componentstorage.get_component<Renderer>(e);
		// std::cout << r << std::endl;
		// r->pipeline = test_pipeline;
		// r->mesh = m;

		game.add_processus((Processus*)(new MeshRenderer(&game)));

		game.run();

		game.entitymanager.componentstorage.destroy_component_storage<Transform>();
		game.entitymanager.componentstorage.destroy_component_storage<Renderer>();

		test_pipeline->destroy(game.context());
	#ifdef DEBUG
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	#endif
	return EXIT_SUCCESS;
}
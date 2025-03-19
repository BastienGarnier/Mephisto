#include <iostream>

#include <fsge/core/structures/linkedholearray.hpp>
#include <fsge/core/structures/holeblockarray.hpp>

#include <fsge/core/components/components.hpp> 
#include <fsge/core/components/componentstorage.hpp>

#include <fsge/core/entities/entitymanager.hpp>

#include <fsge/core/application.hpp>
#include <fsge/core/utilities.hpp>

#include <fsge/core/queries/usetquery.hpp>
#include <fsge/windowhandler.hpp>

#include <thread>

using namespace fsge;

int main(int argc, const char *argv[])
{
	#ifdef DEBUG
	std::cout << "=== Running in DEBUG MODE ===" << std::endl;
	try {
	#endif

		std::cout << std::thread::hardware_concurrency() << std::endl;
		Application app("FSGE Tests");

		// 1. créer espace pour les composants

		app.entitymanager.componentstorage.create_component_storage<Window>();
		app.entitymanager.componentstorage.create_component_storage<VulkanSupport>();

		// 2. définir les queries

		// TODO : voir pour le destructeur des queries qui n'est pas appelé
		app.entitymanager.add_query<Window, VulkanSupport>((EntityQuery*)(new USetQuery())); //-> renvoie les objets de type Window

		// 3. ajouter les entités au jeu

		app.entitymanager.share("RootWindow", app.entitymanager.add_entity<Window, VulkanSupport>(Passer<Window>(640, 480, "Test", Window::NoFlags), VulkanSupport()));

		// 4. ajouter les processus

		app.add_processus((Processus*)(new WindowHandler(app))); 

		// 5. lancer l'application

		app.run();

		// Libérer espace des composants
		app.entitymanager.componentstorage.destroy_component_storage<Window>();
		app.entitymanager.componentstorage.destroy_component_storage<VulkanSupport>();
	#ifdef DEBUG
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	#endif
	return EXIT_SUCCESS;
}
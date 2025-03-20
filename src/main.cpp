#include <iostream>

#include <mephisto/collections/linkedholearray.hpp>
#include <mephisto/collections/holeblockarray.hpp>

#include <mephisto/components/components.hpp> 
#include <mephisto/components/componentstorage.hpp>

#include <mephisto/ressources/ressources.hpp>
#include <mephisto/ressources/ressourcesmanager.hpp>

#include <mephisto/entities/entitymanager.hpp>

#include <mephisto/application/application.hpp>
#include <mephisto/utilities.hpp>

#include <mephisto/queries/usetquery.hpp>

#include <mephisto/vulkan/context.hpp>

#include <mephisto/window/handler.hpp>

#include <mephisto/application/game.hpp>

using namespace mephisto;

int main(int argc, const char *argv[])
{
	#ifdef DEBUG
	std::cout << "=== Running in DEBUG MODE ===" << std::endl;
	try {
	#endif
		Game game;

		// game.entitymanager -> gestion des entités et composants
		// game.ressourcesmanager -> gestion des ressources
		// game.processusmanager ? gestion des processus ?

		game.run();
	#ifdef DEBUG
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	#endif
	return EXIT_SUCCESS;
}
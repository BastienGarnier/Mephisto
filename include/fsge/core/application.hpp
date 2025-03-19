#ifndef SPGE_GAME_HPP_INCLUDED
#define SPGE_GAME_HPP_INCLUDED

#include <unordered_set>
#include <iostream>
#include <string>
#include <list>

#include <fsge/core/processus.hpp>
#include <fsge/core/entities/entitymanager.hpp>
#include <fsge/core/ressources/ressourcesmanager.hpp>

namespace fsge {
	class Application
	{
	public:
		Application(std::string name) : is_open(true), name(name) {}
		Application() : is_open(true), name("Unnamed") {}
		~Application();

		inline void run() {
			start();
			while (is_open) {
				update();
			}
		}
		void start();
		void update();

		void add_processus(Processus *p, uint32_t priority = 1); // 0 = system

		void close();

		bool is_open;
		std::string name;
		EntityManager entitymanager;
		RessourcesManager ressourcesmanager;

	private:
		std::vector<std::list<Processus*>> _processus;
	};
}
#endif
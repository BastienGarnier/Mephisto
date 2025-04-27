#pragma once
#ifndef _MEPHISTO_APPLICATION_HPP_INCLUDED
#define _MEPHISTO_APPLICATION_HPP_INCLUDED

#include <unordered_set>
#include <iostream>
#include <string>
#include <list>

#include <mephisto/processus/processusmanager.hpp>
#include <mephisto/entities/entitymanager.hpp>
#include <mephisto/ressources/ressourcesmanager.hpp>

namespace mephisto {
	class Application
	{
	public:
		Application() : is_open(true) {}
		~Application();

		inline void run() {
			processusmanager.start_all();
			while (is_open) {
				processusmanager.update_all(&is_open);
			}
		}
		inline void close() {
			is_open = false;
		}

		bool is_open;
		EntityManager entitymanager;
		RessourcesManager ressourcesmanager;
		ProcessusManager processusmanager;

	private:
		
	};
}
#endif
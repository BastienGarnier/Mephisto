#pragma once
#ifndef _MEPHISTO_PROCESSUSMANAGER_HPP_INCLUDED
#define _MEPHISTO_PROCESSUSMANAGER_HPP_INCLUDED

#include <stdint.h>
#include <vector>
#include <list> // à réécrire
#include <mephisto/processus/processus.hpp>

namespace mephisto {
	class ProcessusManager
	{
	public:
		ProcessusManager() {}
		~ProcessusManager() {}

		void start_all();
		void update_all(bool *is_open);
		void destroy_all();

		void add(Processus *p, uint32_t priority = 1); // 0 = system
	private:

		std::list<Processus*>::iterator _piter;
		std::vector<std::list<Processus*>> _processus;	
	};
}

#endif
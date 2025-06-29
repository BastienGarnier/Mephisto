#pragma once
#ifndef _MEPHISTO_PROCESSUS_HPP_INCLUDED
#define _MEPHISTO_PROCESSUS_HPP_INCLUDED

#include <mephisto/entities/entityquery.hpp>

namespace mephisto {
	class Processus
	{
	public:
		Processus() {
		}
		~Processus() {
		}

		virtual void destroy() = 0;
		virtual void start() = 0;
		virtual void update() = 0;
		virtual void stop() = 0;

		EntityQuery* query;
	};
}

#endif
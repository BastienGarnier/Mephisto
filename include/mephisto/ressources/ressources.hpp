#pragma once
#ifndef _MEPHISTO_CORE_RESSOURCES_TYPESENUM_HPP_INCLUDED
#define _MEPHISTO_CORE_RESSOURCES_TYPESENUM_HPP_INCLUDED

#include <mephisto/ressources/constants.hpp>

#define RegisterRessource(RESSOURCE) template<> uint8_t mephisto::Ressource<RESSOURCE>::id = 0

namespace mephisto {
	template <typename T>
	class Ressource
	{
	public:
		inline static constexpr const uint64_t get_id()
		{
			return ((uint64_t)(&id) - MEPHISTO_RESSOURCES_ID_OFFSET);
		}
	private:
		static uint8_t __attribute__((section(".ressourcesSection"))) id;
	};
}

#endif
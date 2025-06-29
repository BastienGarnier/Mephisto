#pragma once
#ifndef _MEPHISTO_ENTITY_HPP_INCLUDED
#define _MEPHISTO_ENTITY_HPP_INCLUDED

#include <cstdint>
#include <unordered_map>

namespace mephisto {
	typedef uint32_t EntityId;
	struct Entity
	{
		EntityId id; 
		std::unordered_map<uint64_t, uint32_t> components_id; // components_indices[i] donne l'indice du composant de type i dans le tableau des instances
	};
}

#endif
#ifndef _FSGE_ENTITY_HPP_INCLUDED
#define _FSGE_ENTITY_HPP_INCLUDED

#include <cstdint>
#include <unordered_map>
#include <iostream>

#include <fsge/core/components/archetype.hpp>

namespace fsge {
	typedef uint32_t EntityId;
	class Entity
	{
	public:
		Entity();

		~Entity();

		EntityId id; 
		std::unordered_map<uint32_t, uint32_t> components_id; // components_indices[i] donne l'indice du composant de type i dans le tableau des instances
	};
}

#endif
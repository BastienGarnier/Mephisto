#ifndef _FSGE_QUERY_HPP_INCLUDED
#define _FSGE_QUERY_HPP_INCLUDED

#include <vector>
#include <cstdint>

#include <fsge/core/entities/entity.hpp>

namespace fsge {
	/*
	T est la structure de données utilisée pour stocker les entités dans la requête.
	Il faut a minima :
	- possibilité d'itérer sur les éléments de la structure
	- ajouter/supprimer des éléments
	Ce serait pas mal :
	- d'assurer que les éléments sont uniques
	*/
	class EntityQuery
	{
	public:
		EntityQuery() {}
		~EntityQuery() {}

		virtual void add_entity(EntityId e) = 0;
		virtual void remove_entity(EntityId e) = 0;
	};
}

#endif
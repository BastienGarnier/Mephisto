#pragma once
#ifndef _MEPHISTO_QUERIES_USET_HPP_INCLUDED
#define _MEPHISTO_QUERIES_USET_HPP_INCLUDED
#include <unordered_set>
#include <iostream>

#include <mephisto/entities/entity.hpp>
#include <mephisto/entities/entityquery.hpp>

namespace mephisto {
	class USetQuery : EntityQuery
	{
	public:
		USetQuery() {

		}
		~USetQuery() {

		}	

		void add_entity(EntityId e) {
			entities.insert(e);
		}
		void remove_entity(EntityId e) {
			entities.erase(e);
		}

		inline bool empty() {
			return entities.empty();
		}

		std::unordered_set<EntityId> entities;
	};
}
#endif
#pragma once

#include <unordered_set>
#include <iostream>

#include <fsge/core/entities/entity.hpp>
#include <fsge/core/entities/entityquery.hpp>

namespace fsge {
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
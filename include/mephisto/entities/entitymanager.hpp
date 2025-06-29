#pragma once
#ifndef _MEPHISTO_ENTITIES_MANAGER_HPP_INCLUDED
#define _MEPHISTO_ENTITIES_MANAGER_HPP_INCLUDED

#include <unordered_map>
#include <vector>
#include <utility>
#include <cstdint>


#include <mephisto/collections/linkedholearray.hpp>

#include <mephisto/components/components.hpp>
#include <mephisto/components/componentstorage.hpp>
#include <mephisto/components/archetype.hpp>

#include <mephisto/entities/entity.hpp>
#include <mephisto/entities/entityquery.hpp>

namespace mephisto {
	class EntityManager
	{
	public:
		EntityManager();
		~EntityManager();

		template <typename ...Cpnts>
		Entity* add_entity(Archetype_t arch) {
			EntityId id = _entities.new_instance();
			Entity &e = _entities[id];
			e.id = id;
			componentstorage._new_components_instances<Cpnts...>(e);
			for (auto& [key, query]: _queries) {
				if (key < arch) {
					query->add_entity(id);
				}
			}
			return &e;
		}

		template <typename ...Cpnts>
		std::vector<Entity*> add_entities(Archetype_t arch, uint32_t count) {
			uint32_t n = count;
			std::vector<Entity*> v;
			v.resize(count);
			for (unsigned int i = 0; i < count; i++) {
				EntityId id = _entities.new_instance();
				Entity &e = _entities[id];
				e.id = id;
				v[i] = &e;
				componentstorage._new_components_instances<Cpnts...>(e);
			}
			for (auto& [key, query]: _queries) {
				if (key < arch) {
					for (Entity* e : v) {
						query->add_entity(e->id);
					}
				}
			}
			return v;
		}

		template <typename ...Cpnts>
		void remove_entity(Archetype_t arch, EntityId eid) {
			componentstorage._del_components_instances<Cpnts...>(_entities[eid]);
			for (auto& [key, query]: _queries) {
				if (key < arch) {
					query->remove_entity(eid);
				}
			}
			_entities.del_instance(eid);
			unshare(eid);
		}
		
		void add_query(Archetype_t arch, EntityQuery *query) {
			_queries[arch] = query;
		}

		inline EntityQuery* query(Archetype_t arch) {
			return _queries[arch];
		}

		Entity* get_entity(EntityId eid);
		void share(const std::string& name, EntityId eid);
		Entity get_shared(const std::string& name);
		void unshare(const std::string& name);
		void unshare(EntityId eid);
		
		ComponentStorage componentstorage;
	private:
		std::unordered_map<Archetype_t, EntityQuery*> _queries; // associe à chaque archétype l'ensemble des entités de cet archétype
		LinkedHoleArray<Entity> _entities; // all entities
		struct {
			std::unordered_map<std::string, EntityId> name;
			std::unordered_map<EntityId, std::string> id;
		} _shared;
	};
}
#endif

#ifndef _FSGE_ENTITIES_MANAGER_HPP_INCLUDED
#define _FSGE_ENTITIES_MANAGER_HPP_INCLUDED

#include <unordered_map>
#include <vector>
#include <utility>
#include <cstdint>


#include <fsge/core/structures/linkedholearray.hpp>

#include <fsge/core/components/components.hpp>
#include <fsge/core/components/componentstorage.hpp>
#include <fsge/core/components/archetype.hpp>

#include <fsge/core/entities/entity.hpp>
#include <fsge/core/entities/entityquery.hpp>

namespace fsge {
	class EntityManager
	{
	public:
		EntityManager();
		~EntityManager();

		template <typename ...Cpnts>
		EntityId add_entity(Passer<Cpnts>&&... components) {
			EntityId id = _entities.new_instance();
			Entity &e = _entities[id];
			e.id = id;
			componentstorage._new_components_instances<Cpnts...>(e, std::forward<Passer<Cpnts>>(components)...);
			constexpr auto entity_arch = Archetype<Cpnts...>();
			for (auto& [key, value]: _queries) {
				if (key < entity_arch) {
					value->add_entity(id);
				}
			}
			return id;
		}

		template <typename ...Cpnts>
		void add_entities(uint32_t count, Passer<Cpnts>&&... components) {
			uint32_t n = count;
			std::vector<EntityId> v;
			v.resize(count);
			for (unsigned int i = 0; i < count; i++) {
				EntityId id = _entities.new_instance();
				Entity &e = _entities[id];
				e.id = id;
				v[i] = id;
				componentstorage._new_components_instances<Cpnts...>(e, std::forward<Passer<Cpnts>>(components)...);
			}
			constexpr auto entity_arch = Archetype<Cpnts...>();
			for (auto& [key, value]: _queries) {
				if (key < entity_arch) {
					for (EntityId id : v) {
						value->add_entity(id);
					}
				}
			}
		}

		template <typename ...Cpnts>
		void remove_entity(EntityId eid) {
			componentstorage._del_components_instances<Cpnts...>(_entities[eid]);
			constexpr auto entity_arch = Archetype<Cpnts...>();
			for (auto& [key, value]: _queries) {
				if (key < entity_arch) {
					value->remove_entity(eid);
				}
			}
			_entities.del_instance(eid);
			unshare(eid);
		}
		
		template <class... Cpnts>
		void add_query(EntityQuery *eq) {
			_queries[Archetype<Cpnts...>()] = eq;
		}

		template <class... Cpnts>
		EntityQuery* query() {
			return _queries[Archetype<Cpnts...>()];
		}

		Entity &get_entity(EntityId eid) {
			return _entities[eid];
		}

		void share(const std::string& name, EntityId eid) {
			_shared.id[eid] = name;
			_shared.name[name] = eid;
		}
		Entity get_shared(const std::string& name) {
			return _entities[_shared.name[name]];
		}
		void unshare(const std::string& name) {
			_shared.id.erase(_shared.name[name]);
			_shared.name.erase(name);
		}
		void unshare(EntityId eid) {
			_shared.name.erase(_shared.id[eid]);
			_shared.id.erase(eid);	
		}
		
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

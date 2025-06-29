#pragma once
#ifndef _MEPHISTO_COMPONENTSYSTEM_HPP_INCLUDED
#define _MEPHISTO_COMPONENTSYSTEM_HPP_INCLUDED

#include <cstdint>
#include <unordered_map>

#include <mephisto/components/components.hpp>
#include <mephisto/utilities.hpp>
#include <mephisto/collections/linkedholearray.hpp>
#include <mephisto/entities/entity.hpp>

namespace mephisto {
	typedef uint32_t ComponentId;

	class ComponentStorage
	{
	friend class EntityManager;
	public:
		ComponentStorage();
		~ComponentStorage() {
			// call_destructors();
		}

		template <typename T>
		void create_component_storage() {
			// TODO : avoir un truc moins dégueu (a priori pas trop dérangeant puisque appelé seulement au début)
			if (instances.size() <= (uint64_t)Component<T>::get_id()) {
				instances.resize((uint64_t)Component<T>::get_id() + 1);
			}
			instances[(uint64_t)Component<T>::get_id()] = (Object*)(new LinkedHoleArray<T>);
		}

		template <typename T>
		void destroy_component_storage() {
			delete ((LinkedHoleArray<T>*)(instances[(uint64_t)Component<T>::get_id()]));
		}

		template <typename T>
		T* get_component(Entity *e) {
			// still fast if not too much components by entity ? (should maybe have vector for entity)
			uint32_t component_indice = e->components_id[(uint64_t)Component<T>::get_id()];
			return &(*((LinkedHoleArray<T>*)instances[(uint64_t)(Component<T>::get_id())]))[component_indice];
		}

		// template <auto C = 0, auto unique = []{}>
		// inline void call_destructors() {
		// 	static_assert(Components::get_count() == 2, "ok");
		// 	if constexpr (C < Components::get_count()) {
		// 		((LinkedHoleArray<decltype(Components::get_type<C>())>*)(instances[C]))->~LinkedHoleArray();
		// 		call_destructors<C+1>();
		// 	}
		// }

		// std::vector<std::size_t> components_sizes;
		std::vector<Object*> instances; // table de conteneurs des instances de composants. T[i] donne le conteneur des instances du composant i.
	private:
		template <typename T, typename ...Cpnts>
		void _new_components_instances(Entity &e) {
			LinkedHoleArray<T> *lha = (LinkedHoleArray<T>*)(instances[(uint64_t)Component<T>::get_id()]);
			e.components_id[(uint64_t)Component<T>::get_id()] = lha->new_instance();
			if constexpr (sizeof...(Cpnts)) { // reste-t-il des arguments ?
				_new_components_instances<Cpnts...>(e);
			}
		}

		template <typename T, typename ...Cpnts>
		void _del_components_instances(Entity &e) {
			LinkedHoleArray<T> *lha = (LinkedHoleArray<T>*)(instances[(uint64_t)Component<T>::get_id()]);
			lha->del_instance(e.components_id[(uint64_t)Component<T>::get_id()]);
			if constexpr (sizeof...(Cpnts)) { // reste-t-il des arguments ?
				_del_components_instances<Cpnts...>(e);
			}
		}
	};
}

#endif

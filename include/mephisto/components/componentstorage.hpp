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
		inline void create_component_storage() {
			instances[Components::get_enum<T>()] = (Object*)(new LinkedHoleArray<T>);
		}

		template <typename T>
		inline void destroy_component_storage() {
			((LinkedHoleArray<T>*)(instances[Components::get_enum<T>()]))->~LinkedHoleArray();
		}

		template <typename T>
		inline T& get_component(Entity e) {
			constexpr ComponentId component = Components::get_enum<T>();
			uint32_t component_indice = e.components_id[Components::get_enum<T>()];
			return (*((LinkedHoleArray<T>*)instances[component]))[component_indice];
		}

		// template <auto C = 0, auto unique = []{}>
		// inline void call_destructors() {
		// 	static_assert(Components::get_count() == 2, "ok");
		// 	if constexpr (C < Components::get_count()) {
		// 		((LinkedHoleArray<decltype(Components::get_type<C>())>*)(instances[C]))->~LinkedHoleArray();
		// 		call_destructors<C+1>();
		// 	}
		// }


		uint32_t count;
		// std::vector<std::size_t> components_sizes;
		std::unordered_map<uint32_t, Object*> instances; // table de conteneurs des instances de composants. T[i] donne le conteneur des instances du composant i.
	private:
		template <typename T, typename ...Cpnts>
		void _new_components_instances(Entity &e) {
			LinkedHoleArray<T> *lha = (LinkedHoleArray<T>*)(instances[Components::get_enum<T>()]);
			e.components_id[Components::get_enum<T>()] = lha->new_instance();
			if constexpr (sizeof...(Cpnts)) { // reste-t-il des arguments ?
				_new_components_instances<Cpnts...>(e);
			}
		}

		template <typename T, typename ...Cpnts>
		void _new_components_instances(Entity &e, Passer<T> &&cpnt, Passer<Cpnts>&&... components) {
			LinkedHoleArray<T> *lha = (LinkedHoleArray<T>*)(instances[Components::get_enum<T>()]);
			e.components_id[Components::get_enum<T>()] = lha->new_instance(std::move(cpnt));
			if constexpr (sizeof...(Cpnts)) { // reste-t-il des arguments ?
				_new_components_instances<Cpnts...>(e, std::forward<Passer<Cpnts>>(components)...);
			}
		}

		template <typename T, typename ...Cpnts>
		void _del_components_instances(Entity &e) {
			LinkedHoleArray<T> *lha = (LinkedHoleArray<T>*)(instances[Components::get_enum<T>()]);
			lha->del_instance(e.components_id[Components::get_enum<T>()]);
			if constexpr (sizeof...(Cpnts)) { // reste-t-il des arguments ?
				_del_components_instances<Cpnts...>(e);
			}
		}
	};
}

#endif

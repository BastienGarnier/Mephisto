#ifndef _MEPHISTO_CORE_RESSOURCES_MANAGER_HPP_INCLUDED
#define _MEPHISTO_CORE_RESSOURCES_MANAGER_HPP_INCLUDED

#include <string>
#include <unordered_map>

#include <mephisto/utilities.hpp>
#include <mephisto/ressources/ressources.hpp>

namespace mephisto {
	class RessourcesManager
	{
	public:
		RessourcesManager() {}
		~RessourcesManager() {}

		template <typename T>
		T* add(std::string name, T* ress) {
			_ressources[Ressources::get_enum<T>()][name] = Object::make(ress);
			return ress;
		}

		// template <auto C = 0, auto unique = []{}>
		// void call_destructors() {
		// 	static_assert(Components::get_count() == 2, "ok");
		// 	if constexpr (C < Components::get_count()) {
		// 		((LinkedHoleArray<decltype(Components::get_type<C>())>*)(instances[C]))->~LinkedHoleArray();
		// 		call_destructors<C+1>();
		// 	}
		// }

		template <typename T>
		void remove(std::string name) {
			T* t = (T*)(_ressources[Ressources::get_enum<T>()][name]);
			_ressources[Ressources::get_enum<T>()].erase(name);
			t->~T();
		}

		template <typename T>
		T* get(std::string name) {
			return (T*)(_ressources[Ressources::get_enum<T>()][name]);
		}

		template <typename T>
		std::unordered_map<std::string, Object*> all() {
			return _ressources[Ressources::get_enum<T>()];
		}
		
		std::unordered_map<uint32_t, std::unordered_map<std::string, Object*>> _ressources; // ressources[i] donne la table des ressources de type Ressources::get_type<i>()
	};
}
#endif
#pragma once
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
		~RessourcesManager() {
			
		}

		template <typename T>
		T* add(std::string name, T* ress) {
			_ressources[(uint64_t)Ressource<T>::get_id()][name] = Object::make(ress);
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
			T* t = (T*)(_ressources[(uint64_t)Ressource<T>::get_id()][name]);
			_ressources[(uint64_t)Ressource<T>::get_id()].erase(name);
			t->~T();
		}

		template <typename T>
		T* get(std::string name) {
			return (T*)(_ressources[(uint64_t)Ressource<T>::get_id()][name]);
		}

		template <typename T>
		std::unordered_map<std::string, Object*> all() {
			return _ressources[(uint64_t)Ressource<T>::get_id()];
		}
		
		std::unordered_map<uint64_t, std::unordered_map<std::string, Object*>> _ressources; // ressources[i] donne la table des ressources de type Ressources::get_type<i>()
	};
}
#endif
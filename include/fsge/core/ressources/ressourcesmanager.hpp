#ifndef _FSGE_CORE_RESSOURCES_MANAGER_HPP_INCLUDED
#define _FSGE_CORE_RESSOURCES_MANAGER_HPP_INCLUDED

#include <string>
#include <unordered_map>

#include <fsge/core/utilities.hpp>
#include <fsge/core/ressources/ressources.hpp>

namespace fsge {
	class RessourcesManager
	{
	public:
		RessourcesManager() {}
		~RessourcesManager() {}

		template <typename T>
		void add(std::string name, T* ress) {
			_ressources[Ressources::get_enum<T>()][name] = Object::make(ress);
		}

		template <typename T>
		void remove(std::string name) {
			T* t =  (T*)(_ressources[Ressources::get_enum<T>()][name]);
			_ressources[Ressources::get_enum<T>()].erase(name);
			t.~T();
		}

		template <typename T>
		T* get(std::string name) {
			return (T*)(_ressources[Ressources::get_enum<T>()][name]);
		}
		
		std::unordered_map<uint32_t, std::unordered_map<std::string, Object*>> _ressources; // ressources[i] donne la table des ressources de type Ressources::get_type<i>()
	};
}
#endif
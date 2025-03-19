#ifndef _FSGE_ARCHETYPE_HPP_INCLUDED
#define _FSGE_ARCHETYPE_HPP_INCLUDED

#include <bitset>

#include <fsge/core/components/components.hpp>

namespace fsge {
	typedef std::bitset<Components::get_count()> Archetype_t;

	bool operator<(const Archetype_t &a, const Archetype_t &b);

	template <class T, class... Ts>
	consteval auto Archetype(const Archetype_t b = 0) {
		if constexpr (sizeof...(Ts)) {
			return Archetype<Ts...>(b | Archetype_t(1 << Components::get_enum<T>()));
		} else {
			return b;
		}
	}
}

// class Archetype
// 	{
// 	private:
// 		template <class T, class... Ts>
// 		consteval std::bitset<2000> _construct_archetype(std::bitset<2000> b = 0) const {
// 			if constexpr (sizeof...(Ts)) {
// 				return _construct_archetype<Ts...>(b | std::bitset<2000>(1 << Components::get_enum<T>()));
// 			} else {
// 				return (b | std::bitset<2000>(1 << Components::get_enum<T>()));
// 			}
// 		}
// 	public:
// 		const std::bitset<2000> value;
		
// 		template <class... Ts>
// 		consteval Archetype(std::in_place_type_t<Ts>...) : value(_construct_archetype<Ts...>()) {
// 		}	
// 	};

// template <>
// 	struct std::hash<Archetype>
// 	{
// 	  std::size_t operator()(const Archetype& a) const
// 	  {
// 	    using std::size_t;
// 	    using std::hash;
// 	    using std::string;

// 	    // Compute individual hash values for first,
// 	    // second and third and combine them using XOR
// 	    // and bit shifting:

// 	    return hash<std::bitset<2000>>()(a.value);
// 	  }
// 	};

#endif
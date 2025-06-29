#pragma once
#ifndef _MEPHISTO_ARCHETYPE_HPP_INCLUDED
#define _MEPHISTO_ARCHETYPE_HPP_INCLUDED

#include <vector>

#include <mephisto/components/components.hpp>

namespace mephisto {
	typedef std::vector<uint32_t> Archetype_t;

	bool operator<(const Archetype_t &a, const Archetype_t &b);

	template <class T, class... Ts>
	void construct(Archetype_t &arch) {
		if constexpr (sizeof...(Ts)) {
			arch.push_back(Component<T>::get_id());
			return construct<Ts...>(arch);
		}
	}

	template <class... Ts>
	Archetype_t Archetype() {
		Archetype_t arch(1);
		construct<Ts...>(arch);
		std::stable_sort(arch.begin(), arch.end());
		return arch;
	}
}

template<> struct std::hash<mephisto::Archetype_t> {
	std::size_t operator()(mephisto::Archetype_t const& vec) const {
		std::size_t seed = vec.size();
		for(auto x : vec) {
			x = ((x >> 16) ^ x) * 0x45d9f3b;
			x = ((x >> 16) ^ x) * 0x45d9f3b;
			x = (x >> 16) ^ x;
			seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	}
};

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
// 		using std::size_t;
// 		using std::hash;
// 		using std::string;

// 		// Compute individual hash values for first,
// 		// second and third and combine them using XOR
// 		// and bit shifting:

// 		return hash<std::bitset<2000>>()(a.value);
// 	  }
// 	};

#endif
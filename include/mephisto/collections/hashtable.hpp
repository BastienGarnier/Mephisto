#pragma once
#ifndef _MEPHISTO_COLLECTIONS_HASHTABLE_HPP_INCLUDED
#define _MEPHISTO_COLLECTIONS_HASHTABLE_HPP_INCLUDED

#include <vector>

namespace mephisto {
	template<typename K, typename T>
	class HashTable
	{
	public:
		HashTable();
		~HashTable();

		inline uint32_t capacity() {
			return _table.size()
		}

		uint32_t reduction(uint64_t index) {
			return (index << _inverse_p) >> _inverse_p;
		}

		T 
	private:
		std::vector<T> _table; // assuming size is power of 2
		uint8_t _inverse_p; // (32 - power) to make reduction works
	};
}

#endif
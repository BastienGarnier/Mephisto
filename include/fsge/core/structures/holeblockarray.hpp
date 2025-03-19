#ifndef _FSGE_CORE_STRUCTURES_HOLEBLOCKARRAY_HPP_INCLUDED
#define _FSGE_CORE_STRUCTURES_HOLEBLOCKARRAY_HPP_INCLUDED

#include <cstdlib>
#include <vector>
#include <iterator>
#include <cstddef>
#include <unordered_map>

// #include "fsge/structures/linkedholearray.hpp"
#include <fsge/core/lowlevel/cpuid.hpp>
#include <fsge/core/lowlevel/hugepage.hpp>

namespace fsge {
	template <typename T>
	class HoleBlockArray
	{
	public:
		struct Instance {
		public:
			union {
				uint32_t end;
				T e;
			};
			bool valid;
		};

		HoleBlockArray(bool is_tag = false) {
			_tail_index = 0;
			_size = 0;
			if (!is_tag) {
				_hugepages.emplace_back();
			}
		}
		~HoleBlockArray() {
			for (auto itr = this->begin(); itr < this->end(); itr++) {
				(*itr).~T();
			}
		}

		uint32_t size() {
			return _size;
		}

		T& get_value(uint32_t index);
		T& operator[](uint32_t i);
		
		Instance* get_instance(uint32_t index);
		uint32_t new_instance(); // renvoie l'indice de la nouvelle instance
		template <typename... Args>
		uint32_t new_instance(Args&&... args);
		
		uint32_t new_instance(Passer<T> &&t);
		void del_instance(uint32_t index);
		
		inline bool is_instance_valid(Instance* instance) {
			return instance->valid;
		}	

		class Iterator {
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = value_type*;
			using reference = value_type&;
		public:

			Iterator(HoleBlockArray& hba, uint32_t index) : _hba(hba), _index(index) {
				_inst = _hba.get_instance(_index);
				_index = (!_inst->valid && _index < _hba._tail_index) ? _inst->end + 1 : _index;
				_inst = _hba.get_instance(_inst->end + 1);
			};

			reference operator*() const {
				return _inst->e;
			}

			pointer operator->() {return &(_inst->e);}

			Iterator& operator++() {
				// pass to next element
				_inst = _hba.get_instance(++_index);
				if (!_inst->valid && _index < _hba._tail_index) {
					_index = _inst->end + 1;
				}
				return *this;
			}
			Iterator operator++(int) {
				Iterator retvalue = *this;
				++(*this);
				return retvalue;
			}
			bool operator==(Iterator other) const {return _index == other._index;}
			bool operator!=(Iterator other) const {return !(*this == other);}
			bool operator<(Iterator other) const {return _index < other._index;}
			bool operator>(Iterator other) const {return _index > other._index;}
		private:
			HoleBlockArray& _hba;
			Instance* _inst;
			uint32_t _index;
		};

		Iterator begin() {
			return Iterator(*this, 0);
		}

		Iterator end() {
			return Iterator(*this, _tail_index);
		}
	private:
		std::vector<HugePage> _hugepages; // a priori, que des pages de 2Mio, mais en soi ça peut être n'imp
		std::unordered_map<uint32_t, uint32_t> _hole_blocks;

		uint32_t _tail_index;
		uint32_t _size;	
	};
}

#include "fsge/core/structures/holeblockarray.tpp"

#endif
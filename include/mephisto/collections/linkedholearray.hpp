#ifndef _MEPHISTO_CORE_STRUCTURES_LINKEDHOLEARRAY_HPP_INCLUDED
#define _MEPHISTO_CORE_STRUCTURES_LINKEDHOLEARRAY_HPP_INCLUDED

#include <cstdlib>
#include <vector>
#include <iterator>
#include <cstddef>

#include <mephisto/utilities.hpp>
#include <mephisto/lowlevel/cpuid.hpp>
#include <mephisto/lowlevel/hugepage.hpp>

namespace mephisto {
	template <typename T>
	class LinkedHoleArray : Object
	{
	public:
		struct Instance {
		public:
			union {
				T e;
				uint32_t next_hole;
			};
			bool valid;
		};

		LinkedHoleArray() {
			_first_valid_instance = -1;
			_first_invalid_instance = -1;
			_tail_index = 0;
			_size = 0;
			_hugepages.emplace_back();
		}
		~LinkedHoleArray() {
			for (auto itr = this->begin(); itr < this->end(); itr++) {
				(*itr).~T();
			}
		}

		uint32_t size() {
			return _size;
		}
		
		uint32_t capacity() {
			return _tail_index;
		}
	
		T& get_value(uint32_t index);
		T& operator[](uint32_t i);

		Instance* get_instance(uint32_t index);
		
		uint32_t new_instance();// renvoie l'indice de la nouvelle instance
		template <typename... Args>
		uint32_t new_instance(Args&&... args);
		// uint32_t new_instance(T t);
		// uint32_t new_instance(T &&t);
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

			Iterator(LinkedHoleArray& lha, uint32_t index) : _lha(lha), _index(index) {
				Instance *inst = _lha.get_instance(_index);
				while (!inst->valid && _index < _lha._tail_index) {
					inst = _lha.get_instance(++_index);
				}
			};

			reference operator*() const {
				return _lha.get_instance(_index)->e;
			}

			pointer operator->() {return &(_lha.get_instance(_index)->e);}

			Iterator& operator++() {
				Instance *inst = _lha.get_instance(++_index);
				while (!inst->valid && _index < _lha._tail_index) {
					inst = _lha.get_instance(++_index);
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
			LinkedHoleArray& _lha;
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
		uint32_t _first_valid_instance;
		uint32_t _first_invalid_instance;
		uint32_t _tail_index;
		uint32_t _size;
	};
}

#include <mephisto/collections/linkedholearray.tpp>

#endif
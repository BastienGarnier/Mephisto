namespace mephisto {
	template <typename T>
	typename HoleBlockArray<T>::Instance* HoleBlockArray<T>::get_instance(uint32_t index) {
		uint32_t _count_by_page = _HUGEPAGE_SIZE / sizeof(Instance);
		uint32_t page = index / _count_by_page;
		uint32_t local_index = index % _count_by_page;

		return (_hugepages[page].get<Instance>(local_index));
	}

	template <typename T>
	T& HoleBlockArray<T>::get_value(uint32_t index) { // renvoie l'instance de composant d'indice 'index'
		uint32_t _count_by_page = _HUGEPAGE_SIZE / sizeof(Instance);
		uint32_t page = index / _count_by_page;
		uint32_t local_index = index % _count_by_page;

		return (_hugepages[page].get<Instance>(local_index))->e;
	}

	template <typename T>
	T& HoleBlockArray<T>::operator[](uint32_t index) {
		return get_value(index);
	}


	template <typename T>
	uint32_t HoleBlockArray<T>::new_instance() {
		uint32_t _count_by_page = _HUGEPAGE_SIZE / sizeof(Instance);
		uint32_t new_index;
		if (_hole_blocks.empty()) { // pas de trous, on insert à la fin
			new_index = _tail_index++;
			if (_tail_index >= _hugepages.size() * _count_by_page) {
				_hugepages.resize(_hugepages.size() + 1);
			}
		} else {
			auto block = _hole_blocks.begin(); // premier bloc
			new_index = block->second--; // on insert à la fin du bloc
			
			if ((signed)block->first > (signed)block->second) { // signed => au cas où block->first = 0
				_hole_blocks.erase(block); // plus de bloc
			} else { // 
				Instance* new_end_inst = get_instance(block->second);
				new_end_inst->end = block->first;
				Instance* left_inst = get_instance(block->first);
				left_inst->end = block->second;
			}
		}
		Instance *inst = get_instance(new_index);
		inst->valid = 1;
		new (&inst->e) T();
		_size++;

		return new_index;
	}

	template <typename T> template<class ... Args>
	uint32_t HoleBlockArray<T>::new_instance(Args&&... args) {
		uint32_t _count_by_page = _HUGEPAGE_SIZE / sizeof(Instance);
		uint32_t new_index;
		if (_hole_blocks.empty()) { // pas de trous, on insert à la fin
			new_index = _tail_index++;
			if (_tail_index >= _hugepages.size() * _count_by_page) {
				_hugepages.resize(_hugepages.size() + 1);
			}
		} else {
			auto block = _hole_blocks.begin(); // premier bloc
			new_index = block->second--; // on insert à la fin du bloc
			
			if ((signed)block->first > (signed)block->second) { // signed => au cas où block->first = 0
				_hole_blocks.erase(block); // plus de bloc
			} else { // 
				Instance* new_end_inst = get_instance(block->second);
				new_end_inst->end = block->first;
				Instance* left_inst = get_instance(block->first);
				left_inst->end = block->second;
			}
		}
		Instance *inst = get_instance(new_index);
		inst->valid = 1;
		new (&inst->e) T(std::forward<Args>(args)...);
		_size++;

		return new_index;
	}

	template <typename T>
	uint32_t HoleBlockArray<T>::new_instance(Passer<T> &&t) {
		uint32_t _count_by_page = _HUGEPAGE_SIZE / sizeof(Instance);
		uint32_t new_index;
		if (_hole_blocks.empty()) { // pas de trous, on insert à la fin
			new_index = _tail_index++;
			if (_tail_index >= _hugepages.size() * _count_by_page) {
				_hugepages.resize(_hugepages.size() + 1);
			}
		} else {
			auto block = _hole_blocks.begin(); // premier bloc
			new_index = block->second--; // on insert à la fin du bloc
			
			if ((signed)block->first > (signed)block->second) { // signed => au cas où block->first = 0
				_hole_blocks.erase(block); // plus de bloc
			} else { // 
				Instance* new_end_inst = get_instance(block->second);
				new_end_inst->end = block->first;
				Instance* left_inst = get_instance(block->first);
				left_inst->end = block->second;
			}
		}
		Instance *inst = get_instance(new_index);
		inst->valid = 1;
		new (&inst->e) T(t.value);
		_size++;

		return new_index;
	}

	template <typename T>
	void HoleBlockArray<T>::del_instance(uint32_t index) { // OK
		Instance* inst = get_instance(index);
		_size -= inst->valid;
		inst->valid = 0; // invalide l'instance -> devient un trou
		inst->end = index;
		inst->e.~T();

		Instance* right_inst = (index + 1 < _tail_index) ? get_instance(index + 1) : nullptr;
		Instance* left_inst = (index > 0) ? get_instance(index - 1) : nullptr;

		if (right_inst != nullptr && !right_inst->valid) {
			Instance* right_end_inst = get_instance(right_inst->end);
			inst->end = right_inst->end;
			right_end_inst->end = index;
			_hole_blocks.erase(index + 1);
		}

		_hole_blocks[index] = inst->end;

		if (left_inst != nullptr && !left_inst->valid) {
			Instance* right_end_inst = get_instance(inst->end);
			Instance* left_end_inst = get_instance(left_inst->end);
			unsigned int e = inst->end;
			right_end_inst->end = left_inst->end;
			left_end_inst->end = e;

			_hole_blocks.erase(index);
			_hole_blocks[right_end_inst->end] = left_end_inst->end;
		}
	}
}
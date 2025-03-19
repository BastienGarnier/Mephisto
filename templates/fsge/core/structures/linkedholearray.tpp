namespace fsge {
	template <typename T>
	typename LinkedHoleArray<T>::Instance* LinkedHoleArray<T>::get_instance(uint32_t index) {
		uint32_t _count_by_page = _HUGEPAGE_SIZE / sizeof(Instance);
		uint32_t page = index / _count_by_page;
		uint32_t local_index = index % _count_by_page;

		return (_hugepages[page].get<Instance>(local_index));
	}

	template <typename T>
	T& LinkedHoleArray<T>::operator[](uint32_t index) {
		return get_value(index);
	}

	template <typename T>
	T& LinkedHoleArray<T>::get_value(uint32_t index) { // renvoie l'instance de composant d'indice 'index'
		uint32_t _count_by_page = _HUGEPAGE_SIZE / sizeof(Instance);
		uint32_t page = index / _count_by_page;
		uint32_t local_index = index % _count_by_page;

		return (_hugepages[page].get<Instance>(local_index))->e;
	}

	template <typename T>
	uint32_t LinkedHoleArray<T>::new_instance() {
		uint32_t _count_by_page = _HUGEPAGE_SIZE / sizeof(Instance);
		uint32_t new_index = (_first_invalid_instance == (uint32_t)-1) ? (_tail_index++) : _first_invalid_instance;
		uint32_t page = new_index / _count_by_page;
		uint32_t index = new_index % _count_by_page;

		HugePage &hp = _hugepages[page];
		(hp.get<Instance>(index))->valid = 1; // valide la nouvelle instance
		_first_invalid_instance = (new_index == _first_invalid_instance) ? (hp.get<Instance>(index))->next_hole : -1; // get la prochaine invalide
		new (&(hp.get<Instance>(index))->e) T();

		_size++;
		if (_first_invalid_instance == (uint32_t)-1 && _tail_index >= _hugepages.size() * _count_by_page) {
			_hugepages.resize(_hugepages.size() + 1);
		}
		return new_index;
	}

	template <typename T> template<class ... Args>
	uint32_t LinkedHoleArray<T>::new_instance(Args&&... args) {
		uint32_t _count_by_page = _HUGEPAGE_SIZE / sizeof(Instance);
		uint32_t new_index = (_first_invalid_instance == (uint32_t)-1) ? (_tail_index++) : _first_invalid_instance;
		uint32_t page = new_index / _count_by_page;
		uint32_t index = new_index % _count_by_page;

		HugePage &hp = _hugepages[page];
		(hp.get<Instance>(index))->valid = 1; // valide la nouvelle instance
		_first_invalid_instance = (new_index == _first_invalid_instance) ? (hp.get<Instance>(index))->next_hole : -1; // get la prochaine invalide
		new (&(hp.get<Instance>(index))->e) T(std::forward<Args>(args)...);
		_size++;
		if (_first_invalid_instance == (uint32_t)-1 && _tail_index >= _hugepages.size() * _count_by_page) {
			_hugepages.resize(_hugepages.size() + 1);
		}
		return new_index;
	}

	// template <typename T>
	// uint32_t LinkedHoleArray<T>::new_instance(T t) {
	// 	uint32_t _count_by_page = _HUGEPAGE_SIZE / sizeof(Instance);
	// 	uint32_t new_index = (_first_invalid_instance == (uint32_t)-1) ? (_tail_index++) : _first_invalid_instance;
	// 	uint32_t page = new_index / _count_by_page;
	// 	uint32_t index = new_index % _count_by_page;

	// 	HugePage &hp = _hugepages[page];
	// 	(hp.get<Instance>(index))->valid = 1; // valide la nouvelle instance
	// 	_first_invalid_instance = (new_index == _first_invalid_instance) ? (hp.get<Instance>(index))->next_hole : -1; // get la prochaine invalide
	// 	(hp.get<Instance>(index))->e = std::move(t);

	// 	_size++;
	// 	if (_first_invalid_instance == (uint32_t)-1 && _tail_index >= _hugepages.size() * _count_by_page) {
	// 		_hugepages.resize(_hugepages.size() + 1);
	// 	}
	// 	return new_index;
	// }

	// template <typename T>
	// uint32_t LinkedHoleArray<T>::new_instance(T &&t) {
	// 	uint32_t _count_by_page = _HUGEPAGE_SIZE / sizeof(Instance);
	// 	uint32_t new_index = (_first_invalid_instance == (uint32_t)-1) ? (_tail_index++) : _first_invalid_instance;
	// 	uint32_t page = new_index / _count_by_page;
	// 	uint32_t index = new_index % _count_by_page;

	// 	HugePage &hp = _hugepages[page];
	// 	(hp.get<Instance>(index))->valid = 1; // valide la nouvelle instance
	// 	_first_invalid_instance = (new_index == _first_invalid_instance) ? (hp.get<Instance>(index))->next_hole : -1; // get la prochaine invalide
	// 	new (&(hp.get<Instance>(index))->e) T(t);
	// 	_size++;
	// 	if (_first_invalid_instance == (uint32_t)-1 && _tail_index >= _hugepages.size() * _count_by_page) {
	// 		_hugepages.resize(_hugepages.size() + 1);
	// 	}
	// 	return new_index;
	// }

	template <typename T>
	uint32_t LinkedHoleArray<T>::new_instance(Passer<T> &&t) {
		uint32_t _count_by_page = _HUGEPAGE_SIZE / sizeof(Instance);
		uint32_t new_index = (_first_invalid_instance == (uint32_t)-1) ? (_tail_index++) : _first_invalid_instance;
		uint32_t page = new_index / _count_by_page;
		uint32_t index = new_index % _count_by_page;

		HugePage &hp = _hugepages[page];
		(hp.get<Instance>(index))->valid = 1; // valide la nouvelle instance
		_first_invalid_instance = (new_index == _first_invalid_instance) ? (hp.get<Instance>(index))->next_hole : -1; // get la prochaine invalide
		new (&(hp.get<Instance>(index))->e) T(t.value);
		_size++;
		if (_first_invalid_instance == (uint32_t)-1 && _tail_index >= _hugepages.size() * _count_by_page) {
			_hugepages.resize(_hugepages.size() + 1);
		}
		return new_index;
	}

	template <typename T>
	void LinkedHoleArray<T>::del_instance(uint32_t index) {
		uint32_t _count_by_page = _HUGEPAGE_SIZE / sizeof(Instance);
		uint32_t page = index / _count_by_page;
		uint32_t local_index = index % _count_by_page;

		HugePage &hp = _hugepages[page];

		_size -= (hp.get<Instance>(local_index))->valid;
		(hp.get<Instance>(local_index))->valid = 0; // invalide l'instance
		(hp.get<Instance>(local_index))->e.~T(); // détruit l'instance
		(hp.get<Instance>(local_index))->next_hole = _first_invalid_instance;
		_first_invalid_instance = local_index;
	}
}
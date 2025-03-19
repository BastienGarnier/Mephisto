#ifndef _FSGE_HUGEPAGE_HPP_INCLUDED
#define _FSGE_HUGEPAGE_HPP_INCLUDED

#include <cstdint>
#include <iostream>
#include <stdlib.h>
#include <sys/mman.h>
#include <memory>

#define _PAGE_SIZE (1 << 12) // 4 Kio

#define _HUGEPAGE_SIZE (1 << 21) // 2 Mio

namespace fsge {
	class HugePage
	{
	public:
		HugePage(uint32_t count = 1);
		~HugePage();
		size_t size;
		std::shared_ptr<char> buffer;

		template <typename T>
		T* get(uint32_t i) {
			return (T*)(buffer.get()) + i;
		}

		template <typename T>
		T& operator[](uint32_t i) {
			return (*get<T>(i));
		}
	};
}

#endif
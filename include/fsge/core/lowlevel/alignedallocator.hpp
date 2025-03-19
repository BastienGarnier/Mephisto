#ifndef _FSGE_ALIGNED_ALLOCATOR_HPP_INCLUDED
#define _FSGE_ALIGNED_ALLOCATOR_HPP_INCLUDED

#include <cstdlib>
#include <memory>

namespace fsge {
	template <class T, int N> // N est l'alignement
	class AlignedAllocator
	{
	public:
		AlignedAllocator() = default;
		~AlignedAllocator() = default;
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		template <typename U, int M>
	    AlignedAllocator(const AlignedAllocator<U, M>&) noexcept {}

		template <class U>
		struct rebind
		{
			typedef AlignedAllocator<U,N> other;
		};

		pointer allocate(size_type n) {
			pointer result = reinterpret_cast<pointer>(std::aligned_alloc(N, sizeof(T)*n));
			if (result == nullptr) {
				throw std::bad_alloc();
			}
			return result;
		}
        inline void deallocate(pointer p, size_type) {
        	std::free(p);
        }

        inline void construct(pointer p, const_reference value) { new (p) value_type(value); }
        inline void destroy(pointer p) { p->~value_type(); }

        inline size_type max_size() const throw() { return size_type(-1) / sizeof(T); }

        inline bool operator==(const AlignedAllocator& aa) { return true; }
        inline bool operator!=(const AlignedAllocator& rhs) { return !operator==(rhs); }
	private:
		template <typename U, int M>
		friend class AlignedAllocator;
	};
}

#endif

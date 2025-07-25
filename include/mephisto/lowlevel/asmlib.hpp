#pragma once
#ifndef _MEPHISTO_ASMLIB_HPP_INCLUDED
#define _MEPHISTO_ASMLIB_HPP_INCLUDED

namespace mephisto {
	static inline __attribute__((always_inline)) unsigned long long __rdtsc(void) {
		unsigned long long time_stamp;
		__asm__ __volatile__("rdtsc" : "=A"(time_stamp));
		return time_stamp;
	}

	static inline uint32_t __cpuid(uint32_t code, uint32_t where[4]) {
		__asm__ __volatile__ ("cpuid\n\t"
							: "=a" (where[0]), "=b" (where[1]), "=c" (where[2]), "=d" (where[3])
							: "0" (code));
		return (uint32_t)where[0];
	}
}

#endif
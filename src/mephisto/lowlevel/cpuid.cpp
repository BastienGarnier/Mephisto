#include "mephisto/lowlevel/cpuid.hpp"

using namespace mephisto;

bool CpuInfo::is_cpuid_available() {
	
	#ifndef __x86_64__
	// TODO : il y a un bogue lors de l'optimisation O3.
	// il faut empêcher le compilateur de perdre les opérations assembleurs
	unsigned int cpuid_available = 0;
	__asm__ __volatile__ (
			"pushf;" // copie de l'original
			"pushf;" // copie pour modification de ID (bit 0x21)
			"pop rax;"
			"xor rax, 0x00200000;"
			"push rax;"
			"popf;"
			"pushf;"
			"pop rax;"
			"xor rax, qword[rsp];" // eax a l'info de ID modifié ou non. Si modifiable, CPUID est supporté
			"popf;" // restore l'original
			"and rax, 0x00200000;"
			:"=a"(cpuid_available)
		); 
	return (cpuid_available != 0);
	#else 
	return true; // toujours vrai pour AMD64
	#endif
}

void CpuInfo::query_cpu_features() {
	vendor = cpuid_vendor_string();
	uint32_t registers[4];
	__cpuid(1, registers);
	uint8_t* p_ebx = (uint8_t*)&registers[1];
	cache_alignement = 8*p_ebx[1];
	is_valid = true;
}
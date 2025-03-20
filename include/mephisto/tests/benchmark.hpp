#pragma once

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <chrono>
#include <thread>

#include <mephisto/utilities.hpp>
#include <mephisto/lowlevel/asmlib.hpp>

namespace mephisto {
	void _noopy_origin();

	class Benchmark
	{
	public:
		static const auto MEASURER_MEAN_NUMBER = 80;
		Benchmark(void (* f)(void), void (* noopy)(void)) {
			func_ptr = f;
			_noopy = noopy;
		}
		~Benchmark() {

		}

		void run() {
			printf("=== RUNNING BENCHMARK ===\n");
			warmup_RDTSC();
			steady_start = std::chrono::steady_clock::now();
			c_start = clock();
			func_ptr();
			c_end = clock();
			steady_end = std::chrono::steady_clock::now();
		}

		void display_results() {
			printf("=== BENCHMARK RESULTS ===\n");
			printf("CPU cycles : %ld, CPU time : %.3lf seconds\n", (c_end - c_start - _correctionFactor), (c_end - c_start - _correctionFactor) / (double)CLOCKS_PER_SEC);
			printf("Correction factor %u clocks\n", _correctionFactor);
			printf("Measurement Accuracy (in clocks) : %u\n", _accuracy);
			std::cout << "Real time : " << std::chrono::duration<double>(steady_end - steady_start).count() << " s\n";
			printf("=========================\n");
		}

		private:
			long long get_RDTSC() { // -> Adaptation d'un code de Agnes Fog
				uint32_t dummy[4];
				volatile int DontSkip; // Volatile to prevent optimizing
				long long clock; // Time
				DontSkip = __cpuid(0, dummy); // Prevent optimizing away cpuid
				UNUSED(DontSkip);
				clock = __rdtsc(); // Read time
				return clock;
			}

			inline void warmup_RDTSC()
			{
			    uint32_t a[4];
			    __cpuid(0x80000000, a); // warmup cpuid.
			    __cpuid(0x80000000, a);
			    __cpuid(0x80000000, a);

				unsigned long long tick1 = get_RDTSC();
				unsigned long long tick2 = get_RDTSC();
				uint32_t diff = (uint32_t)(tick2 - tick1);

			    // calcule le temps qu'il faut en moyenne au minuteur pour mesurer. Il faut le supprimer ensuite du temps mesuré
			    uint32_t minDiff = diff;
			    uint32_t maxDiff = 0; 
			    for (int i = 0; i < MEASURER_MEAN_NUMBER; i++)
			    {
			        tick1 = get_RDTSC();
			        tick2 = get_RDTSC();
			        diff =  (uint32_t)(tick2 - tick1); // Jsp pourquoi mais ça marche, alors que tick2-tick1 donne des valeurs absurdes :| Un truc bizarre sur les unsigned long long
			        minDiff = (minDiff > diff) ? diff : minDiff;
			        maxDiff = (maxDiff < diff) ? diff : maxDiff;
			    }
			    _correctionFactor = minDiff;
			    _accuracy = maxDiff - minDiff;
			}

			uint32_t _correctionFactor;
			uint32_t _accuracy;
			static uint64_t const errormeasure = ~((uint64_t)0);

			clock_t c_start, c_end;
			std::chrono::steady_clock::time_point steady_start, steady_end;
			void (* func_ptr)(void);
			void (* _noopy)(void);
	};
}

#include "fsge/core/components/archetype.hpp"

bool fsge::operator<(const Archetype_t &a, const Archetype_t &b) {
	return ((a & b) ^ a).none();
}
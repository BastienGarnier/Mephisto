#include "mephisto/components/archetype.hpp"

bool mephisto::operator<(const Archetype_t &a, const Archetype_t &b) {
	return ((a & b) ^ a).none();
}
#include "mephisto/components/archetype.hpp"

bool mephisto::operator<(const Archetype_t &a, const Archetype_t &b) {
	if (b.size() < a.size()) return false;
	uint16_t i = 0, j = 0;
	while (i < a.size()) {
		if (a[i] < b[j]) return false;

		i = (a[i] == b[j]) ? (i + 1) : i;
		j++;
	}
	return true;
}
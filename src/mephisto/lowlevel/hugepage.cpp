#include "mephisto/lowlevel/hugepage.hpp"

using namespace mephisto;

HugePage::HugePage(uint32_t count) {
	size = count * _HUGEPAGE_SIZE;
	buffer = std::shared_ptr<char>(new (std::align_val_t(_HUGEPAGE_SIZE)) char[size]);
	#if defined DEBUG
	if (!buffer.get()) {
		throw std::runtime_error("Failed to allocate hugepage !");
	}
	#endif
	madvise(buffer.get(), size, MADV_HUGEPAGE);
}

HugePage::~HugePage() {
	buffer.reset();
}

#include "mephisto/processus/processusmanager.hpp"

using namespace mephisto;

void ProcessusManager::add(Processus *p, uint32_t priority) {
	if (priority >= _processus.size()) {
		_processus.resize(priority + 1);
	}
	_processus[priority].push_back(p);
}

void ProcessusManager::destroy_all() {
	for (auto plist : _processus) {
		for (Processus *p : plist) p->destroy();
	}
}


void ProcessusManager::start_all() {
	for (auto plist : _processus) {
		for (Processus *p : plist) p->start();
	}
}

void ProcessusManager::update_all(bool *is_open) {
	for (auto plist : _processus) {
		for (Processus *p : plist) {
			if (*is_open) {
				p->update();
			} else {
				return;
			}
		}
	}
}

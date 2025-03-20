#include "mephisto/application/application.hpp"

using namespace mephisto;


Application::~Application()
{

}

void Application::start() {
	for (auto plist : _processus) {
		for (Processus *p : plist) p->start();
	}
}

void Application::update() {
	for (auto plist : _processus) {
		for (Processus *p : plist) p->update();
	}
}

void Application::close() {
	is_open = false;

	for (auto plist : _processus) {
		for (Processus *p : plist) p->stop();
	}
}

void Application::add_processus(Processus *p, uint32_t priority) {
	if (priority >= _processus.size()) {
		_processus.resize(priority + 1);
	}
	_processus[priority].push_back(p);
}
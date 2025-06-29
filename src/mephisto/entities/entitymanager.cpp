#include "mephisto/entities/entitymanager.hpp"

using namespace mephisto;

EntityManager::EntityManager() {
}

EntityManager::~EntityManager() {

}

Entity* EntityManager::get_entity(EntityId eid) {
	return &(_entities[eid]);
}

void EntityManager::share(const std::string& name, EntityId eid) {
	_shared.id[eid] = name;
	_shared.name[name] = eid;
}

Entity EntityManager::get_shared(const std::string& name) {
	return _entities[_shared.name[name]];
}

void EntityManager::unshare(const std::string& name) {
	_shared.id.erase(_shared.name[name]);
	_shared.name.erase(name);
}

void EntityManager::unshare(EntityId eid) {
	_shared.name.erase(_shared.id[eid]);
	_shared.id.erase(eid);	
}
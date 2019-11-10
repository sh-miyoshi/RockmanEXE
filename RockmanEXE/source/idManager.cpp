#include "idManager.h"

int IDManager::id = 1;

int IDManager::CreateUniqueID() {
	// TODO(generate hash)
	return id++;
}

#include "idManager.h"

namespace {
	static int id = 0;
}

int IDManager::CreateUniqueID() {
	return id++;
}

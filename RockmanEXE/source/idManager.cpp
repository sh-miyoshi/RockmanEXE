#include "idManager.h"

unsigned IDManager::id = 1;

unsigned IDManager::CreateUniqueID(){
	return id++;
}

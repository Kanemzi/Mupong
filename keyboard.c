#include "usefull.h"

#include "keyboard.h"

static char keys[KEYSL] = {0};

void updateKeys() {
	int i;
	for(i = 0; i < KEYSL ; i++) {
		if(key_down(i)) {
			keys[i] = 1;
		}else{
			keys[i] = 0;
		}
	}
}

char* getKeys() {
	return keys;
}
#ifndef SDLINPUT_H
#define SDLINPUT_H

typedef struct{
	char key[512];

	char& operator[](int index){
		if(index > 127)
			return key[index - 1073741824 + 128];
		else return key[index];
	}

} Input;

int pool_int();

#endif

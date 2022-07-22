#include "bmp.h"
#include <string>
#include <iostream>

using namespace std;

int main(){
	BMP_Picture image = BMP_Picture(2, 3);
	image(0,0).set_c(255,255,255);
	image(0,1).set_c(255,255,255);

	image(1,0).set_c(255,255,255);
	image(1,1).set_c(255,255,0);

	image(2,0).set_c(255,0,0);
	image(2,1).set_c(0,255,0);

	//cout << (int) image(0,0).get_r() << " " << (int) image(0,0).get_g() << " " << (int) image(0,0).get_b() << endl;

	image.save_BMP("dab.bmp");
}

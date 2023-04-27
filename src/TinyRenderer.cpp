// TinyRenderer.cpp : Defines the entry point for the application.
//

#include "TinyRenderer.h"
#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255); // last value is for transparency (RGBa)
const TGAColor red = TGAColor(255, 0, 0, 255);


int main() {
	TGAImage image(100, 100, TGAImage::RGB);
	image.set(52, 41, red);
	image.flip_vertically(); //I want to have the origin at left bottom corner of image
	image.write_tga_file("output.tga");
	return 0;
}

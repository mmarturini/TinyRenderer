﻿// TinyRenderer.cpp : Defines the entry point for the application.
//

#include "TinyRenderer.h"
#include "tgaimage.h"


void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0 * (1. - t) + y1 * t;
		image.set(x, y, color);
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
	}
}


const TGAColor white = TGAColor(255, 255, 255, 255); // last value is for transparency (RGBa)
const TGAColor red = TGAColor(255, 0, 0, 255);


int main() {

	TGAImage image(100, 100, TGAImage::RGB);
	line(20, 13, 40, 80, image, red);
	line(80, 40, 13, 20, image, red);
	image.flip_vertically(); // I want to have the origin at left bottom corner of image
	image.write_tga_file("output.tga");
	std::cout << "Image rendered successfully" << std::endl;
	return 0;
}

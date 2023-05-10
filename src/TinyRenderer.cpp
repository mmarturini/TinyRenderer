// TinyRenderer.cpp : Defines the entry point for the application.
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

	int dx = x1 - x0;
	int dy = y1 - y0;
	float derror = std::abs(dy / float(dx));
	float error = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
		error += derror;
		if (error > .5) {
			y += (y1 > y0 ? 1 : -1);
			error -= 1;
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

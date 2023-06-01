// TinyRenderer.cpp : Defines the entry point for the application.
//

#include<vector>
#include<cmath>
#include<stdlib.h>
#include<iostream>
#include"tgaimage.h"
#include"model.h"
#include"geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255); // last value is for transparency (RGBa)
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
Model* model = NULL;
const int width = 800;
const int height = 800;


void line(Vec2i v0, Vec2i v1, TGAImage& image, const TGAColor& color) {
	int x0{ v0.u };
	int x1{ v1.u };
	int y0{ v0.v };
	int y1{ v1.v };

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
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
		error2 += derror2;
		if (error2 > 1) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx*2;
		}
	}
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
	line(t0, t1, image, color);
	line(t0, t2, image, color);
	line(t1, t2, image, color);
}


int main(int argc, char** argv) {

	try {
		if (argc == 2) {
			model = new Model(argv[1]);
		}
		else {
			model = new Model("C:\\Users\\matte\\source\\repos\\TinyRenderer\\data\\african_head.obj");
		}
	}
	catch (const std::exception& ex) {
		std::cout << "Error: " << ex.what() << std::endl;
		return 1;
	}

	/* African face rendering
	TGAImage image(width, height, TGAImage::RGB);
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.) * width / 2;
			int y0 = (v0.y + 1.) * height / 2;
			int x1 = (v1.x + 1.) * width / 2;
			int y1 = (v1.y + 1.) * height / 2;
			line(x0, y0, x1, y1, image, white);
		}
	}
	*/
	TGAImage image(width, height, TGAImage::RGB);
	Vec2i t0[3] = { Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80) };
	Vec2i t1[3] = { Vec2i(180, 50), Vec2i(150, 1), Vec2i(70, 180) };
	Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };

	triangle(t0[0], t0[1], t0[2], image, red);
	triangle(t1[0], t1[1], t1[2], image, white);
	triangle(t2[0], t2[1], t2[2], image, green);

	image.flip_vertically(); // I want to have the origin at left bottom corner of image
	image.write_tga_file("output.tga");
	std::cout << "Image rendered successfully" << std::endl;
	delete model;
	return 0;
}

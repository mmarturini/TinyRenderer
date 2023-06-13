// TinyRenderer.cpp : Defines the entry point for the application.
//

#include<vector>
#include<cmath>
#include<stdlib.h>
#include<iostream>
#include<algorithm>
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

Vec3f barycentricCoords(Vec2i point, Vec2i v0, Vec2i v1, Vec2i v2) {

	float alpha = ((static_cast<float>(v1.v) - v2.v) * (point.u - v2.u) + (v2.u - v1.u) * (point.v - v2.v)) / ((v1.v - v2.v) * (v0.u - v2.u) + (v2.u - v1.u) * (v0.v - v2.v));
	float beta = ((static_cast<float>(v2.v) - v0.v) * (point.u - v2.u) + (v0.u - v2.u) * (point.v - v2.v)) / ((v1.v - v2.v) * (v0.u - v2.u) + (v2.u - v1.u) * (v0.v - v2.v));
	float gamma = 1.0f - alpha - beta;

	return Vec3f(alpha, beta, gamma);
}

void drawFillTriangle(Vec2i v0, Vec2i v1, Vec2i v2, TGAImage& image, TGAColor color) {
	// Determine bounding box of triangle
	int minX{ std::min({v0.u, v1.u, v2.u})};
	int maxX{ std::max({v0.u, v1.u, v2.u}) };
	int minY{ std::min({v0.v, v1.v, v2.v}) };
	int maxY{ std::max({v0.v, v1.v, v2.v}) };

	// Loop through each pixel in bounding box
	for (int y = minY; y <= maxY; y++) {
		for (int x = minX; x <= maxX; x++) {
			// Check if current pixel is inside triangle using barycentric coordinate method
			Vec3f barycentric = barycentricCoords(Vec2i(x,y), v0, v1, v2);
			if (barycentric.x < 0 || barycentric.y < 0 || barycentric.z < 0) {
				continue;
			}
			image.set(x, y, color);
		}
	}
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

	// African face rendering with lighting
	TGAImage image(width, height, TGAImage::RGB);
	Vec3f light_dir(0, 0, -1); // define light_dirV

	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		Vec2i screen_coords[3];
		Vec3f world_coords[3];
		for (int j = 0; j < 3; j++) {
			Vec3f v = model->vert(face[j]);
			screen_coords[j] = Vec2i((v.x + 1.) * width / 2., (v.y + 1.) * height / 2.);
			world_coords[j] = v;
		}
		// cross prod of two sides of triangle, to compute normal of traingle
		Vec3f n = (world_coords[2]-world_coords[0]) ^ (world_coords[1] - world_coords[0]);
		n.normalize();
		// the intensity of illumination is equal to the scalar product of the light vector and the normal to the given triangle
		float intensity = n * light_dir;
		if (intensity > 0) {
			// drawFillTriangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255)); draw colorful triangles
			drawFillTriangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
		}
	}
	
	image.flip_vertically(); // I want to have the origin at left bottom corner of image
	image.write_tga_file("output.tga");
	std::cout << "Image rendered successfully" << std::endl;
	delete model;
	return 0;
}

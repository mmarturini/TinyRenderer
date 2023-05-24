#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

// class that allows to interface with Wavefront .obj file

// the Constructor takes the path to the .obj file, read its format and saves in verts_ the set of vertices using Vec3f class
// same for the faces but as std::vector
Model::Model(const char* filename) : verts_(), faces_() {
	std::ifstream in; // class to read data from files
	in.open(filename, std::ifstream::in);
	if (in.fail()) {
		throw std::runtime_error("Error in reading the file.");
		return;
	}
	std::string line;
	while (!in.eof()) { // eof is a member returning a boolean indicating whether end of file has been reached
		std::getline(in, line); 
		std::istringstream iss(line.c_str()); // c_str(): C style string representation of variable line
		// std::istringstream class is used to parse strings, allows to extract values from a string as if it were a stream of input data
		char trash;
		// int compare(size_t pos, size_t len, const std::string& str) const: This version compares a substring of the calling std::string object, starting at position pos and with a length of len, with the contents of the str parameter. It returns 0 if the two strings are equal, a negative value if the substring is less than str lexicographically, and a positive value if the substring is greater than str lexicographically
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v.raw[i];
			verts_.push_back(v);
		}
		else if (!line.compare(0, 2, "f ")) {
			std::vector<int> f;
			int itrash, idx;
			iss >> trash;
			while (iss >> idx >> trash >> itrash >> trash >> itrash) {
				idx--; // in wavefront object all indices start at 1 not zero
				f.push_back(idx);
			}
			faces_.push_back(f);
		}
	}
	std::cerr << "# v# " << verts_.size() << " f#" << faces_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
	return (int)verts_.size();
}

int Model::nfaces() {
	return (int)faces_.size();
}

Vec3f Model::vert(int i) {
	return verts_[i];
}

std::vector<int> Model::face(int idx) {
	return faces_[idx];
}

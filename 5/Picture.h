#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

struct color {
	unsigned char r, g, b;
};

class PNM{
public:
	PNM(string);
	void output(string);
	void auto_brightness(int, double, double);
private:
	int width, height, depth; 
	pair<unsigned char, unsigned char>type;
	vector<vector<color>>pnm;
	void YCbCr_601_RGB();
	void RGB_YCbCr_601();
};



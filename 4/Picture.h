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

class PPM{
public:
	PPM(string);
	PPM(string, string, string);
	void output(string);
	void output(string, string, string);
	void convert(string, string);
private:
	int width, height, depth;
	vector<vector<color>>ppm;
	void HSL_RGB();
	void RGB_HSL();
	void HSV_RGB();
	void RGB_HSV();
	void YCbCr_601_RGB();
	void RGB_YCbCr_601();
	void YCbCr_709_RGB();
	void RGB_YCbCr_709();
	void YCoCg_RGB();
	void RGB_YCoCg();
	void CMY_RGB();
	void RGB_CMY();
};



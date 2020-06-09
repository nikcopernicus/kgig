#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

struct Point {
	double x, y;
};

class PGM{
public:
	PGM(string);
	void output(string);
	void draw_line(double, int, int, int, int, double);
	void draw_pix(int, int, double, double, int);
private:
	double fpart(double x);
	int width, height, depth;
	vector<vector<unsigned char>>pgm;
};



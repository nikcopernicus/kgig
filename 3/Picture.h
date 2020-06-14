#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;


class PGM{
public:
	PGM(string,bool);
	void output(string);
	void dithering(int, int, double);
private:
	int width, height, depth;
	vector<vector<double>>pgm;
	double threshold_color(double, int);
};



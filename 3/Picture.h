#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;


class PGM{
public:
	PGM(string,bool,double);
	void output(string, double);
	void dithering(int, int);
private:
	int width, height, depth;
	vector<vector<unsigned char>>pgm;
	unsigned char threshold_color(unsigned char, int);
};



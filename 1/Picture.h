#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

struct color {
	char r, g, b;
};

class PNM {
public:
	virtual void inverse() = 0;
	virtual void hflip() = 0;
	virtual void vflip() = 0;
	virtual void lrotate() = 0;
	virtual void rrotate() = 0;
	virtual void output(string) = 0;
};

class PGM: public PNM {
public:
	PGM(string);
	void inverse();	
	void hflip();
	void vflip();	
	void lrotate();
	void rrotate();
	void output(string);
private:
	int width, height, depth;
	vector<vector<char>>pgm;
};

class PPM: public PNM {
public:
	PPM(string);
	void inverse();
	void hflip();
	void vflip();	
	void lrotate();	
	void rrotate();
	void output(string);
private:
	int width, height, depth;
	vector<vector<color>>ppm;
};



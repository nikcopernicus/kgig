#include "Picture.h"

PGM::PGM(string name) {
	ifstream in(name, ios_base::binary);
	if (!in.is_open()) throw runtime_error("Error: Unable to open file");
	unsigned char type[2];
	in >> type[0] >> type[1];
	if(type[0]!='P'||type[1]!='5')	throw runtime_error("Error: File format is not P5");
	in >> width >> height >> depth;
	if (width <= 0 || height <= 0) {
		in.close();
		throw runtime_error("Error: Wrong width/height in file");
	}
	pgm.assign(height,vector<unsigned char>(width));
	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			pgm[i][j] = 0;
		}
	}
	in.close();
}



void PGM::output(string name) {
	ofstream out(name, ios::binary);
	if (!out.is_open()) {
		throw runtime_error("Error: File is not found");
	}
	out << "P5" << endl << width << ' ' << height << endl << depth << endl;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			out << pgm[i][j];
		}
	}

	out.flush();
	out.close();
}

void PGM::draw_pix(int x, int y, double brightness, double gamma, int max_brightness) {
	if (x >= width || x < 0 || y < 0 || y >= height) return;
	brightness = min(max(brightness, 0.0), 1.0);
	brightness = pow(max_brightness * brightness, 1.0 / gamma);
	pgm[y][x] = min(max((int)round(brightness), 0), 255);
}

double PGM::fpart(double x) {
	return x - floor(x);
}

void PGM::draw_line(double brightness, double x1, double y1, double x2, double y2, double gamma) {
	if ((x1 < 0) || (y1 < 0) || (x2 < 0) || (y2 < 0) || (y1 >= height) || (y2 >= height) || (x1 >= width) || (x2 >= width) || (brightness < 0) || (brightness > 255)) throw runtime_error("Error: Wrong arguments");
	brightness = pow(brightness, gamma);
	bool steep = abs(y2 - y1) > abs(x2 - x1);
	if (steep) {
		swap(x1, y1);
		swap(x2, y2);
	}
	if (x2 < x1) {
		swap(x1, x2);
		swap(y1, y2);
	}
	double dx = x2 - x1;
	double dy = y2 - y1;
	double k = dy / dx;
	
	double step = y1 + k;
	if (steep) {
		draw_pix(y1, x1, 1, gamma, brightness);
		draw_pix(y1 + 1, x1, 0, gamma, brightness);
	}
	else {
		draw_pix(x1, y1, 1, gamma, brightness);
		draw_pix(x1, y1 + 1, 0, gamma, brightness);
	}
	if (steep) {
		draw_pix(y2, x2, 1, gamma, brightness);
		draw_pix(y2 + 1, x2, 0, gamma, brightness);
	}
	else {
		draw_pix(x2, y2, 1, gamma, brightness);
		draw_pix(x2, y2 + 1, 0, gamma, brightness);
	}
	for (double x = x1 + 1; x < x2; x++) {
		if (steep) {
			draw_pix(floor(step), x, (1 - fpart(step)), gamma, brightness);
			draw_pix(floor(step) + 1, x, fpart(step), gamma, brightness);
		}
		else {
			draw_pix(x, floor(step), (1 - fpart(step)), gamma, brightness);
			draw_pix(x, floor(step) + 1, fpart(step), gamma, brightness);
		}
		step += k;
	}

}

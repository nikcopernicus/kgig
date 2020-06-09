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

void PGM::draw_line(double brightness, int x0, int y0, int x1, int y1, double gamma) {
	if ((x0 < 0) || (y0 < 0) || (x1 < 0) || (y1 < 0) || (y0 >= height) || (y1 >= height) || (x0 >= width) || (x1 >= width) || (brightness < 0) || (brightness > 255)) throw runtime_error("Error: Wrong arguments");
	brightness = pow(brightness, gamma);
	if (x1 < x0) {
		swap(x0, x1);
		swap(y0, y1);
	}
	double dx = x1 - x0;
	double dy = y1 - y0;
	double k = dy / dx;
	if (dy > dx) {
		k = dx / dy;
		swap(x0, y0);
		swap(x1, y1);
	}
	double step = y0 + k;
	if (dy > dx) {
		draw_pix(y0, x0, 1, gamma, brightness);
		draw_pix(y0 + 1, x0, 0, gamma, brightness);
	}
	else {
		draw_pix(x0, y0, 1, gamma, brightness);
		draw_pix(x0, y0 + 1, 0, gamma, brightness);
	}
	if (dy > dx) {
		draw_pix(y1, x1, 1, gamma, brightness);
		draw_pix(y1 + 1, x1, 0, gamma, brightness);
	}
	else {
		draw_pix(x1, y1, 1, gamma, brightness);
		draw_pix(x1, y1 + 1, 0, gamma, brightness);
	}
	for (int x = x0 + 1; x < x1; x++) {
		if (dy > dx) {
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

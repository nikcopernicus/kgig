#include "Picture.h"

PGM::PGM(string name) {
	ifstream in(name, ios_base::binary);
	char type;
	in >> type >> type;
	in >> width >> height >> depth;
	char pixel;
	in.read(&pixel, sizeof(char));
	pgm.assign(height,vector<char>(width));
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (in.eof()) {
				in.close();
				throw runtime_error("Error: Not enough pixels");
			}
			in.read(&pixel, sizeof(char));
			pgm[i][j] = pixel;
		}
	}
	in.close();
}

PPM::PPM(string name) {
	ifstream in(name, ios_base::binary);
	char type;
	in >> type >> type;
	in >> width >> height >> depth;	
	in.read(&type, sizeof(char));
	color pixel;
	ppm.assign(height, vector<color>(width));
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (in.eof()) {
				in.close();
				throw runtime_error("Error: Not enough pixels");
			}
			in.read(&pixel.r, sizeof(char));
			if (in.eof()) {
				in.close();
				throw runtime_error("Error: Not enough pixels");
			}
			in.read(&pixel.g, sizeof(char));
			if (in.eof()) {
				in.close();
				throw runtime_error("Error: Not enough pixels");
			}
			in.read(&pixel.b, sizeof(char));
			ppm[i][j] = pixel;
		}
	}
	in.close();
}

void PGM::inverse() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			pgm[i][j] = depth - pgm[i][j];
		}
	}
}

void PPM::inverse() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			ppm[i][j] = { (char)(depth - ppm[i][j].r),(char)(depth - ppm[i][j].g),(char)(depth - ppm[i][j].b) };
		}
	}
}

void PGM::hflip() {
	for (int i = 0; i < height; i++) {
		reverse(pgm[i].begin(), pgm[i].end());
	}
}

void PPM::hflip() {
	for (int i = 0; i < height; i++) {
		reverse(ppm[i].begin(), ppm[i].end());
	}
}

void PGM::vflip() {
	for (int i = 0; i < height / 2; i++) {
		for (int j = 0; j < width; j++) {
			swap(pgm[i][j], pgm[height-i-1][j]);
		}
	}
}

void PPM::vflip() {
	for (int i = 0; i < height / 2; i++) {
		for (int j = 0; j < width; j++) {
			swap(ppm[i][j], ppm[height - i - 1][j]);
		}
	}
}

void PGM::lrotate() {
	vector<vector<char>>new_pgm(width, vector<char>(height));
	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			new_pgm[width - j - 1][i] = pgm[i][j];
		}
	}	
	pgm = new_pgm;
	swap(height, width);
}

void PPM::lrotate() {
	vector<vector<color>>new_ppm(width, vector<color>(height));

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			new_ppm[width - j - 1][i] = ppm[i][j];
		}
	}
	ppm = new_ppm;
	swap(height, width);
}

void PGM::rrotate() {
	vector<vector<char>>new_pgm(width, vector<char>(height));

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			new_pgm[j][height - i - 1] = pgm[i][j];
		}
	}
	pgm = new_pgm;
	swap(height, width);
}

void PPM::rrotate() {
	vector<vector<color>>new_ppm(width, vector<color>(height));

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			new_ppm[j][height - i - 1] = ppm[i][j];
		}
	}
	ppm = new_ppm;
	swap(height, width);
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

void PPM::output(string name) {
	ofstream out(name, ios::binary);
	if (!out.is_open()) {
		throw runtime_error("Error: File is not found");
	}
	out << "P6" << endl << width << ' ' << height << endl << depth << endl;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			out << ppm[i][j].r << ppm[i][j].g << ppm[i][j].b;
		}
	}

	out.flush();
	out.close();
}
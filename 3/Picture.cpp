#include "Picture.h"

PGM::PGM(string name, bool gradient, double gamma) {
	ifstream in(name, ios_base::binary);
	if (!in.is_open()) {
		in.close();
		throw runtime_error("Error: Unable to open input file");
	}
	unsigned char type[2];
	in >> type[0] >> type[1];
	if (type[0] != 'P' || type[1] != '5') {
		in.close();
		throw runtime_error("Error: File format is not P5");
	}
	in >> width >> height >> depth;
	if (width <= 0 || height <= 0 || depth <= 0 || depth > 255) {
		in.close();
		throw runtime_error("Error: Wrong width/height in file");
	}
	pgm.assign(height,vector<unsigned char>(width));
	if (gradient) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				pgm[i][j] = (j % width) * 255 / width;
			}
		}
	}
	else {
		char pixel;
		in.read(&pixel, sizeof(char));
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
	}
	in.close();	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double temp = (double)pgm[i][j] / (double)depth;
			if (gamma) {
				temp = min(pow(temp, gamma), 1.);
			}
			else {
				if (temp <= 0.04045) {
					temp = 25. / 323. * temp;
				}
				else {
					temp = pow((200. * temp + 11.) / 211., 12. / 5.);
				}
			}
			pgm[i][j] = temp * (double)depth;
		}
	}
	
}


void PGM::output(string name, double gamma, int bitrate) {
	ofstream out(name, ios::binary);
	if (!out.is_open()) {
		throw runtime_error("Error: Unable to open input file");
	}
	out << "P5" << endl << width << ' ' << height << endl << (1 << bitrate) - 1 << endl;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double temp = (double)pgm[i][j] / (double)depth;
			if (gamma) {
				temp = pow(temp, 1. / gamma);
			}
			else {
				if (temp <= 0.0031308) {
					temp = 323. / 25. * temp ;
				}
				else {
					temp = (211. * pow(temp, 5. / 12.) - 11.) / 200.;
				}
			}
			pgm[i][j] = round(temp * (double)((1 << bitrate)-1));
			out << pgm[i][j];
		}
	}
	out.flush();
	out.close();
}

unsigned char PGM::threshold_color(unsigned char color, int bitrate) {
	vector<bool> arr(bitrate,0);
	unsigned char temp = color >> (8 - bitrate);
	for (int i = 0; i < bitrate; i++) {
		arr[bitrate-i-1]=temp&(1<<i);
	}
	temp = 0;
	for (int i = 0; i < 8; i++) {
		temp = temp << 1;
		temp = temp | arr[i%bitrate];
	}
	return temp;
}

double treshhold_map[8][8] = {
	{0,48,12,60,3,51,15,63},
	{32,16,44,28,35,19,47,31},
	{8,56,4,52,11,59,7,55},
	{40,24,36,20,43,27,39,23},
	{2,50,14,62,1,49,13,61},
	{34,18,46,30,33,17,45,29},
	{10,58,6,54,9,57,5,53},
	{42,26,38,22,41,25,37,21},
};
double halftone_map[4][4] = {
	{7, 13, 11, 4},
	{12, 16, 14, 8},
	{10, 15, 6, 2},
	{5, 9, 3, 1},
};
void PGM::dithering(int type, int bitrate) {
	switch (type) {
	case 0:
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				pgm[i][j] = threshold_color(pgm[i][j], bitrate);
			}
		}
		break;
	case 1:
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				double color1 = (double)pgm[i][j] / (double)depth,
					color2 = (treshhold_map[i % 8][j % 8] / 64) - 0.5;
				double color = max(min(color1 + color2, 1.), 0.);
				pgm[i][j] = threshold_color((unsigned char)(color * (double)depth), bitrate);
			} 
		}
		break;
	case 2:
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				double color1 = (double)pgm[i][j] / (double)depth,
					color2 = (rand()*1.)/(RAND_MAX*1.) - 0.5;
				double color = max(min(color1 + color2, 1.), 0.);
				pgm[i][j] = threshold_color((unsigned char)(color * (double)depth), bitrate);
			}
		}
		break;
	case 3:
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				unsigned char
					oldpixel = pgm[i][j],
					newpixel = threshold_color(oldpixel, bitrate);
				double	quant_error = (oldpixel - newpixel);
				pgm[i][j] = newpixel;
				if (j + 1 < width) {
					pgm[i][j + 1] = max(min(pgm[i][j + 1] + quant_error * 7. / 16., 255.), 0.);
				}
				if (i + 1 < height) {
					if (j + 1 < width) {
						pgm[i + 1][j + 1] = max(min(pgm[i + 1][j + 1] + quant_error * 1. / 16., 255.), 0.);
					}
					pgm[i + 1][j] = max(min(pgm[i + 1][j] + quant_error * 5. / 16., 255.), 0.);
					if (j - 1 >= 0) {
						pgm[i + 1][j - 1] = max(min(pgm[i + 1][j - 1] + quant_error * 3. / 16., 255.), 0.);
					}
				}
			}
		}
		break;
	case 4:
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				unsigned char
					oldpixel = pgm[i][j],
					newpixel = threshold_color(oldpixel, bitrate);
				double	quant_error = (oldpixel - newpixel);
				pgm[i][j] = newpixel;
				if (j + 1 < width) {
					pgm[i][j + 1] = max(min(pgm[i][j + 1] + quant_error * 7. / 48., 255.), 0.);
				}
				if (j + 2 < width) {
					pgm[i][j + 2] = max(min(pgm[i][j + 2] + quant_error * 5. / 48., 255.), 0.);
				}
				if (i + 1 < height) {
					if (j + 2 < width) {
						pgm[i + 1][j + 2] = max(min(pgm[i + 1][j + 2] + quant_error * 3. / 48., 255.), 0.);
					}
					if (j + 1 < width) {
						pgm[i + 1][j + 1] = max(min(pgm[i + 1][j + 1] + quant_error * 5. / 48., 255.), 0.);
					}
					pgm[i + 1][j] = max(min(pgm[i + 1][j] + quant_error * 7. / 48., 255.), 0.);
					if (j - 1 >= 0) {
						pgm[i + 1][j - 1] = max(min(pgm[i + 1][j - 1] + quant_error * 5. / 48., 255.), 0.);
					}
					if (j - 2 >= 0) {
						pgm[i + 1][j - 2] = max(min(pgm[i + 1][j - 2] + quant_error * 3. / 48., 255.), 0.);
					}
				}
				if (i + 2 < height) {
					if (j + 2 < width) {
						pgm[i + 2][j + 2] = max(min(pgm[i + 2][j + 2] + quant_error * 1. / 48., 255.), 0.);
					}
					if (j + 1 < width) {
						pgm[i + 2][j + 1] = max(min(pgm[i + 2][j + 1] + quant_error * 3. / 48., 255.), 0.);
					}
					pgm[i + 2][j] = max(min(pgm[i + 2][j] + quant_error * 5. / 48., 255.), 0.);
					if (j - 1 >= 0) {
						pgm[i + 2][j - 1] = max(min(pgm[i + 2][j - 1] + quant_error * 3. / 48., 255.), 0.);
					}
					if (j - 2 >= 0) {
						pgm[i + 2][j - 2] = max(min(pgm[i + 2][j - 2] + quant_error * 1. / 48., 255.), 0.);
					}
				}
			}
		}
		break;
	case 5:
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				unsigned char
					oldpixel = pgm[i][j],
					newpixel = threshold_color(oldpixel, bitrate);
				double	quant_error = (oldpixel - newpixel);
				pgm[i][j] = newpixel;
				if (j + 1 < width) {
					pgm[i][j + 1] = max(min(pgm[i][j + 1] + quant_error * 5. / 32., 255.), 0.);
				}
				if (j + 2 < width) {
					pgm[i][j + 2] = max(min(pgm[i][j + 2] + quant_error * 3. / 32., 255.), 0.);
				}
				if (i + 1 < height) {
					if (j + 2 < width) {
						pgm[i + 1][j + 2] = max(min(pgm[i + 1][j + 2] + quant_error * 2. / 32., 255.), 0.);
					}
					if (j + 1 < width) {
						pgm[i + 1][j + 1] = max(min(pgm[i + 1][j + 1] + quant_error * 4. / 32., 255.), 0.);
					}
					pgm[i + 1][j] = max(min(pgm[i + 1][j] + quant_error * 5. / 32., 255.), 0.);
					if (j - 1 >= 0) {
						pgm[i + 1][j - 1] = max(min(pgm[i + 1][j - 1] + quant_error * 4. / 32., 255.), 0.);
					}
					if (j - 2 >= 0) {
						pgm[i + 1][j - 2] = max(min(pgm[i + 1][j - 2] + quant_error * 2. / 32., 255.), 0.);
					}
				}
				if (i + 2 < height) {
					if (j + 1 < width) {
						pgm[i + 2][j + 1] = max(min(pgm[i + 2][j + 1] + quant_error * 2. / 32., 255.), 0.);
					}
					pgm[i + 2][j] = max(min(pgm[i + 2][j] + quant_error * 3. / 32., 255.), 0.);
					if (j - 1 >= 0) {
						pgm[i + 2][j - 1] = max(min(pgm[i + 2][j - 1] + quant_error * 2. / 32., 255.), 0.);
					}
				}
			}
		}
		break;
	case 6:
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				unsigned char
					oldpixel = pgm[i][j],
					newpixel = threshold_color(oldpixel, bitrate);
				double	quant_error = (oldpixel - newpixel);
				pgm[i][j] = newpixel;
				if (j + 1 < width) {
					pgm[i][j + 1] = max(min(pgm[i][j + 1] + quant_error * 1. / 8., 255.), 0.);
				}
				if (j + 2 < width) {
					pgm[i][j + 2] = max(min(pgm[i][j + 2] + quant_error * 1. / 8., 255.), 0.);
				}
				if (i + 1 < height) {
					if (j + 1 < width) {
						pgm[i + 1][j + 1] = max(min(pgm[i + 1][j + 1] + quant_error * 1. / 8., 255.), 0.);
					}
					pgm[i + 1][j] = max(min(pgm[i + 1][j] + quant_error * 1. / 8., 255.), 0.);
					if (j - 1 >= 0) {
						pgm[i + 1][j - 1] = max(min(pgm[i + 1][j - 1] + quant_error * 1. / 8., 255.), 0.);
					}
				}
				if (i + 2 < height) {
					pgm[i + 2][j] = max(min(pgm[i + 2][j] + quant_error * 1. / 8., 255.), 0.);
				}
			}
		}
		break;
	case 7:
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				double color1 = (double)pgm[i][j] / (double)depth,
					color2 = (halftone_map[i % 4][j % 4] / 16.) - 0.5;
				double color = max(min(color1 + color2, 1.), 0.);
				pgm[i][j] = threshold_color((unsigned char)(color * (double)depth), bitrate);
			}
		}
		break;
	default:
		throw runtime_error("Error: Wrong algorithm type");
		break;
	}
}
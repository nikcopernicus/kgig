#include "Picture.h"

PPM::PPM(string name) {
	ifstream in(name, ios_base::binary);
	if (!in.is_open()) throw runtime_error("Error: Unable to open file");
	char type[2];
	in >> type[0] >> type[1];
	if (type[0] != 'P' || type[1] != '6') {
		in.close();
		throw runtime_error("Error: Wrong file format");
	}
	in >> width >> height >> depth;
	char r,g,b;
	in.read(&r, sizeof(unsigned char));
	ppm.assign(height, vector<color>(width));
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			in.read(&r, sizeof(char));
			in.read(&g, sizeof(char));
			in.read(&b, sizeof(char));
			ppm[i][j] = {(unsigned char)r,(unsigned char)g,(unsigned char)b};
		}
	}
	in.close();
}

PPM::PPM(string name1, string name2, string name3) {
	ifstream in1(name1, ios_base::binary);
	if (!in1.is_open()) throw runtime_error("Error: Unable to open file 1");
	ifstream in2(name2, ios_base::binary);
	if (!in2.is_open()) throw runtime_error("Error: Unable to open file 2");
	ifstream in3(name3, ios_base::binary);
	if (!in3.is_open()) throw runtime_error("Error: Unable to open file 3");
	char type[2];
	in1 >> type[0] >> type[1];
	if (type[0] != 'P' || type[1] != '5') {
		in1.close();in2.close();in3.close();
		throw runtime_error("Error: Wrong file 1 format");
	}
	in1 >> width >> height >> depth;
	in2 >> type[0] >> type[1];
	if (type[0] != 'P' || type[1] != '5') {
		in1.close();in2.close();in3.close();
		throw runtime_error("Error: Wrong file 2 format");
	}
	in2 >> width >> height >> depth;
	in3 >> type[0] >> type[1];
	if (type[0] != 'P' || type[1] != '5') {
		in1.close();in2.close();in3.close();
		throw runtime_error("Error: Wrong file 3 format");
	}
	in3 >> width >> height >> depth;	

	char r, g, b;
	in1.read(&r, sizeof(char));
	in2.read(&g, sizeof(char));
	in3.read(&b, sizeof(char));
	ppm.assign(height, vector<color>(width));
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			in1.read(&r, sizeof(char));
			in2.read(&g, sizeof(char));
			in3.read(&b, sizeof(char));
			ppm[i][j] = { (unsigned char)r,(unsigned char)g,(unsigned char)b };
		}
	}
	in1.close();
	in2.close();
	in3.close();
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

void PPM::output(string name1, string name2, string name3) {
	ofstream out1(name1, ios::binary);
	ofstream out2(name2, ios::binary);
	ofstream out3(name3, ios::binary);
	if (!out1.is_open()) {
		throw runtime_error("Error: File 1 is not found");
	}
	if (!out2.is_open()) {
		throw runtime_error("Error: File 2 is not found");
	}
	if (!out3.is_open()) {
		throw runtime_error("Error: File 3 is not found");
	}
	out1 << "P5" << endl << width << ' ' << height << endl << depth << endl;
	out2 << "P5" << endl << width << ' ' << height << endl << depth << endl;
	out3 << "P5" << endl << width << ' ' << height << endl << depth << endl;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			out1 << ppm[i][j].r;
			out2 << ppm[i][j].g;
			out3 << ppm[i][j].b;
		}
	}
	out1.flush();
	out1.close();
	out2.flush();
	out2.close();
	out3.flush();
	out3.close();
}

void PPM::HSL_RGB() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double h = ppm[i][j].r / 255. * 360, s = ppm[i][j].g / 255., l = ppm[i][j].b / 255.;
			double q = l < 0.5 ? l * (1. + s) : l + s - l * s;
			double p = 2. * l - q;
			double h_k = h / 360.;
			double t_r = h_k + 1. / 3. > 1 ? h_k + 1. / 3 - 1 : h_k + 1. / 3. < 0 ? h_k + 1. / 3 + 1 : h_k + 1. / 3;
			double t_g = h_k > 1 ? h_k - 1 : h_k < 0 ? h_k + 1 : h_k;
			double t_b = h_k - 1. / 3. > 1 ? h_k - 1. / 3. - 1 : h_k - 1. / 3. < 0 ? h_k - 1. / 3. + 1 : h_k - 1. / 3.;
			
			ppm[i][j] = {
				(unsigned char)((t_r < 1. / 6. ? p + ((q - p) * 6. * t_r) : t_r < 1. / 2. ? q : t_r < 2. / 3. ? p + ((q - p) * (2. / 3. - t_r) * 6.) : p) * 255),
				(unsigned char)((t_g < 1. / 6. ? p + ((q - p) * 6. * t_g) : t_g < 1. / 2. ? q : t_g < 2. / 3. ? p + ((q - p) * (2. / 3. - t_g) * 6.) : p) * 255),
				(unsigned char)((t_b < 1. / 6. ? p + ((q - p) * 6. * t_b) : t_b < 1. / 2. ? q : t_b < 2. / 3. ? p + ((q - p) * (2. / 3. - t_b) * 6.) : p) * 255)
			};
		}
	}
}

void PPM::RGB_HSL() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double r = ppm[i][j].r / 255., g = ppm[i][j].g / 255., b = ppm[i][j].b / 255.;
			double mx = max({ r,g,b }), mn = min({ r,g,b });
			double h = 0, l = (mx + mn) / 2, s = 0;
			if (mx == mn) h = 0;
			else if ((mx == r) && (g >= b)) h = 60 * (g - b) / (mx - mn) + 0;
			else if ((mx == r) && (g < b)) h = 60 * (g - b) / (mx - mn) + 360;
			else if (mx == g) h = 60 * (b - r) / (mx - mn) + 120;
			else if (mx == b) h = 60 * (r - g) / (mx - mn) + 240;
			if (mx == mn || l == 0) s = 0;
			else if (0 < l && l < 0.5) s = (mx - mn) / (2 * l);
			else if (0.5 < l && l < 1) s = (mx - mn) / (2 - 2 * l);
			ppm[i][j] = {
				(unsigned char)round(h / 360. * 255.),
				(unsigned char)round(s * 255.),
				(unsigned char)round(l * 255.)
			};
		}
	}
}
void PPM::HSV_RGB() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double h = ppm[i][j].r / 255. * 360., s = ppm[i][j].g / 255. * 100., v = ppm[i][j].b / 255. * 100.;
			int h_i = (int)floor(h / 60.) % 6;
			double v_min = (100. - s) * v / 100.;
			double a = (v - v_min) * ((int)h % 60) / 60;
			double v_inc = v_min + a;
			double v_dec = v - a;
			switch (h_i) {
			case 0:
				ppm[i][j] = { 
					(unsigned char)(v * 255. / 100.),
					(unsigned char)(v_inc * 255. / 100.),
					(unsigned char)(v_min * 255. / 100.)
				};
				break;			
			case 1:
				ppm[i][j] = { 
					(unsigned char)(v_dec * 255. / 100.),
					(unsigned char)(v * 255. / 100.),
					(unsigned char)(v_min * 255. / 100.)
				};
				break;
			case 2:
				ppm[i][j] = { 
					(unsigned char)(v_min * 255. / 100.),
					(unsigned char)(v * 255. / 100.),
					(unsigned char)(v_inc * 255. / 100.)
				};
				break;
			case 3:
				ppm[i][j] = { 
					(unsigned char)(v_min * 255. / 100.),
					(unsigned char)(v_dec * 255. / 100.),
					(unsigned char)(v * 255. / 100.)
				};
				break;
			case 4:
				ppm[i][j] = { 
					(unsigned char)(v_inc * 255. / 100.),
					(unsigned char)(v_min * 255. / 100.),
					(unsigned char)(v * 255. / 100.)
				};
				break;
			case 5:
				ppm[i][j] = { 
					(unsigned char)(v * 255. / 100.),
					(unsigned char)(v_min * 255. / 100.),
					(unsigned char)(v_dec * 255. / 100.)
				};
				break;
			}
		}
	}
}
void PPM::RGB_HSV() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double r = ppm[i][j].r / 255., g = ppm[i][j].g / 255., b = ppm[i][j].b / 255.;
			double mx = max({ r,g,b }), mn = min({ r,g,b });
			double h = 0, v = mx, s = mx == 0 ? 0 : 1. - mn / mx;
			if (mx == mn) h = 0;
			else if ((mx == r) && (g >= b)) h = 60 * (g - b) / (mx - mn) + 0;
			else if ((mx == r) && (g < b)) h = 60 * (g - b) / (mx - mn) + 360;
			else if (mx == g) h = 60 * (b - r) / (mx - mn) + 120;
			else if (mx == b) h = 60 * (r - g) / (mx - mn) + 240;
			ppm[i][j] = {
				(unsigned char)round(h / 360. * 255.),
				(unsigned char)round(s * 255.),
				(unsigned char)round(v * 255.)
			};
		}
	}
}
void PPM::YCbCr_601_RGB() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double y = ppm[i][j].r / 255., c_b = ppm[i][j].g / 255. - 0.5, c_r = ppm[i][j].b / 255. - 0.5;
			double k_r = 0.299, k_g = 0.587, k_b = 0.114;
			double r = y + (2. - 2. * k_r) * c_r,
				g = y - (k_b / k_g) * (2. - 2. * k_b) * c_b - (k_r / k_g) * (2. - 2. * k_r) * c_r,
				b = y + (2. - 2. * k_b) * c_b;
			ppm[i][j] = {
				(unsigned char)round(max(min(r * 255.,255.),0.)),
				(unsigned char)round(max(min(g * 255.,255.),0.)),
				(unsigned char)round(max(min(b * 255.,255.),0.))
			};
		}
	}
}
void PPM::RGB_YCbCr_601() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double r = ppm[i][j].r / 255., g = ppm[i][j].g / 255., b = ppm[i][j].b / 255.;
			double k_r = 0.299, k_g = 0.587, k_b = 0.114;
			double y = k_r * r + k_g * g + k_b * b,
				c_b = -r * k_r / ((1 - k_b) * 2.) - g * k_g / ((1 - k_b) * 2.) + b / 2.,
				c_r = r / 2. - g * k_g / ((1 - k_r) * 2.) - b * k_b / ((1 - k_r) * 2.);
			ppm[i][j] = {
				(unsigned char)round(max(min(y * 255.,255.),0.)),
				(unsigned char)round(max(min((c_b + 0.5) * 255.,255.),0.)),
				(unsigned char)round(max(min((c_r + 0.5) * 255.,255.),0.))
			};
		}
	}
}
void PPM::YCbCr_709_RGB() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double y = ppm[i][j].r / 255., c_b = ppm[i][j].g / 255. - 0.5, c_r = ppm[i][j].b / 255. - 0.5;
			double k_r = 0.2126, k_g = 0.7152, k_b = 0.0722;
			double r = y + (2. - 2. * k_r) * c_r,
				g = y - (k_b / k_g) * (2. - 2. * k_b) * c_b - (k_r / k_g) * (2. - 2. * k_r) * c_r,
				b = y + (2. - 2. * k_b) * c_b;
			ppm[i][j] = {
				(unsigned char)round(max(min(r * 255.,255.),0.)),
				(unsigned char)round(max(min(g * 255.,255.),0.)),
				(unsigned char)round(max(min(b * 255.,255.),0.))
			};
		}
	}
}
void PPM::RGB_YCbCr_709() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double r = ppm[i][j].r / 255., g = ppm[i][j].g / 255., b = ppm[i][j].b / 255.;
			double k_r = 0.2126, k_g = 0.7152, k_b = 0.0722;
			double y = k_r * r + k_g * g + k_b * b,
				c_b = -r * k_r / ((1 - k_b) * 2.) - g * k_g / ((1 - k_b) * 2.) + b / 2.,
				c_r = r / 2. - g * k_g / ((1 - k_r) * 2.) - b * k_b / ((1 - k_r) * 2.);
			ppm[i][j] = {
				(unsigned char)round(max(min(y * 255.,255.),0.)),
				(unsigned char)round(max(min((c_b + 0.5) * 255.,255.),0.)),
				(unsigned char)round(max(min((c_r + 0.5) * 255.,255.),0.))
			};
		}
	}
}
void PPM::YCoCg_RGB() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double y = ppm[i][j].r / 255., c_o = ppm[i][j].g / 255. - 0.5, c_g = ppm[i][j].b / 255. - 0.5;
			ppm[i][j] = {
				(unsigned char)round(max(min((y + c_o - c_g) * 255.,255.),0.)),
				(unsigned char)round(max(min((y + c_g) * 255.,255.),0.)),
				(unsigned char)round(max(min((y - c_o - c_g) * 255.,255.),0.))
			};
		}
	}
}
void PPM::RGB_YCoCg() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double r = ppm[i][j].r / 255., g = ppm[i][j].g / 255., b = ppm[i][j].b / 255.;
			ppm[i][j] = {
				(unsigned char)round(max(min((r / 4. + g / 2. + b / 4.) * 255.,255.),0.)),
				(unsigned char)round(max(min((r / 2. - b / 2. + 0.5) * 255.,255.),0.)),
				(unsigned char)round(max(min((-r / 4. + g / 2. - b / 4. + 0.5) * 255.,255.),0.))
			};
		}
	}
}
void PPM::CMY_RGB() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			ppm[i][j] = {
				(unsigned char)(255. - ppm[i][j].r),
				(unsigned char)(255. - ppm[i][j].g),
				(unsigned char)(255. - ppm[i][j].b)
			};
		}
	}
}
void PPM::RGB_CMY() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			ppm[i][j] = {
				(unsigned char)(255. - ppm[i][j].r),
				(unsigned char)(255. - ppm[i][j].g),
				(unsigned char)(255. - ppm[i][j].b)
			};
		}
	}
}

void PPM::convert(string from, string to) {
	// RGB / HSL / HSV / YCbCr.601 / YCbCr.709 / YCoCg / CMY

	if (from == "RGB");
	else if (from == "HSL")HSL_RGB();
	else if (from == "HSV")HSV_RGB();
	else if (from == "YCbCr.601")YCbCr_601_RGB();
	else if (from == "YCbCr.709")YCbCr_709_RGB();
	else if (from == "YCoCg")YCoCg_RGB();
	else if (from == "CMY")CMY_RGB();
	else throw runtime_error("Error: Wrong <from_color_space> form");

	if (to == "RGB");
	else if (to == "HSL")RGB_HSL();
	else if (to == "HSV")RGB_HSV();
	else if (to == "YCbCr.601")RGB_YCbCr_601();
	else if (to == "YCbCr.709")RGB_YCbCr_709();
	else if (to == "YCoCg")RGB_YCoCg();
	else if (to == "CMY")RGB_CMY();
	else throw runtime_error("Error: Wrong <to_color_space> form");
}
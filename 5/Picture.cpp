#include "Picture.h"

PNM::PNM(string name) {
	ifstream in(name, ios_base::binary);
	if (!in.is_open()) throw runtime_error("Error: Unable to open input file");;
	in >> type.first >> type.second;
	if (type != pair<unsigned char, unsigned char>('P', '6') && type != pair<unsigned char, unsigned char>('P', '5')) {
		in.close();
		throw runtime_error("Error: Wrong input file format");
	}
	in >> width >> height >> depth;
	if (width < 0 || height < 0 || depth < 1 || depth>255) {
		in.close();
		throw runtime_error("Error: Wrong width/height/depth data");
	}
	char c;
	in.read(&c, sizeof(unsigned char));
	pnm.assign(height, vector<color>(width));
	switch (type.second) {
	case '5':
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (in.eof()) {
					in.close();
					throw("Error: Not enough pixels in input file");
				}
				in.read(&c, sizeof(unsigned char));
				pnm[i][j].r = (unsigned char)c;
				pnm[i][j].g = (unsigned char)c;
				pnm[i][j].b = (unsigned char)c;
			}
		}
		break;
	case '6':
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (in.eof()) {
					in.close();
					throw("Error: Not enough pixels in input file");
				}
				in.read(&c, sizeof(unsigned char));
				pnm[i][j].r = (unsigned char)c;
				if (in.eof()) {
					in.close();
					throw("Error: Not enough pixels in input file");
				}
				in.read(&c, sizeof(unsigned char));
				pnm[i][j].g = (unsigned char)c;
				if (in.eof()) {
					in.close();
					throw("Error: Not enough pixels in input file");
				}
				in.read(&c, sizeof(unsigned char));
				pnm[i][j].b = (unsigned char)c;
			}
		}		
		break;
	default:
		in.close();
		throw runtime_error("Error: Wrong input file format");
		break;
	}
	in.close();
}


void PNM::output(string name) {
	ofstream out(name, ios::binary);
	if (!out.is_open()) {
		throw runtime_error("Error: Unable to open output file");
	}
	out << type.first << type.second << endl << width << ' ' << height << endl << depth << endl;
	switch (type.second) {
	case '5':
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				out << (unsigned char)pnm[i][j].r;
			}
		}
		break;
	case '6':
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				out << (unsigned char)pnm[i][j].r << (unsigned char)pnm[i][j].g << (unsigned char)pnm[i][j].b;
			}
		}
		break;
	default:
		out.flush();
		out.close();
		throw runtime_error("Error: Wrong input file format");
		break;
	}

	out.flush();
	out.close();
}

void PNM::YCbCr_601_RGB() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double y = (double)pnm[i][j].r / 255., c_b = (double)pnm[i][j].g / 255. - 0.5, c_r = (double)pnm[i][j].b / 255. - 0.5;
			double k_r = 0.299, k_g = 0.587, k_b = 0.114;
			double r = y + (2. - 2. * k_r) * c_r,
				g = y - (k_b / k_g) * (2. - 2. * k_b) * c_b - (k_r / k_g) * (2. - 2. * k_r) * c_r,
				b = y + (2. - 2. * k_b) * c_b;
			pnm[i][j].r = max(min((int)round(r * 255), 255), 0);
			pnm[i][j].g = max(min((int)round(g * 255), 255), 0);
			pnm[i][j].b = max(min((int)round(b * 255), 255), 0);
		}
	}
}

void PNM::RGB_YCbCr_601() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double r = (double)pnm[i][j].r / 255., g = (double)pnm[i][j].g / 255., b = (double)pnm[i][j].b / 255.;
			double k_r = 0.299, k_g = 0.587, k_b = 0.114;
			double y = k_r * r + k_g * g + k_b * b,
				c_b = (b - y) / (2. * (1 - k_b)),
				c_r = (r - y) / (2. * (1 - k_r));
			pnm[i][j].r = max(min((int)round(y * 255), 255), 0);
			pnm[i][j].g = max(min((int)round((c_b + 0.5) * 255), 255), 0);
			pnm[i][j].b = max(min((int)round((c_r + 0.5) * 255), 255), 0);
		}
	}
}


void PNM::auto_brightness(int transformation, int offset, double multiplier) {
	if (transformation == 0) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				pnm[i][j].r = max(min((int)round((pnm[i][j].r - offset) * multiplier), 255), 0);
				pnm[i][j].g = max(min((int)round((pnm[i][j].g - offset) * multiplier), 255), 0);
				pnm[i][j].b = max(min((int)round((pnm[i][j].b - offset) * multiplier), 255), 0);
			}
		}
	}
	else if (transformation == 1) {
		RGB_YCbCr_601();
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				pnm[i][j].r = max(min((int)round((pnm[i][j].r - offset) * multiplier), 255), 0);
			}
		}
		YCbCr_601_RGB();
	}
	else if (transformation == 2) {
		int mx = -1e9, mn = 1e9;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				mx = max({ mx,pnm[i][j].r,pnm[i][j].g ,pnm[i][j].b });
				mn = min({ mn,pnm[i][j].r,pnm[i][j].g ,pnm[i][j].b });
			}
		}
		offset = (int)mn;
		multiplier = 255. / ((int)mx - (int)mn);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				pnm[i][j].r = max(min((int)round((pnm[i][j].r - offset) * multiplier), 255), 0);
				pnm[i][j].g = max(min((int)round((pnm[i][j].g - offset) * multiplier), 255), 0);
				pnm[i][j].b = max(min((int)round((pnm[i][j].b - offset) * multiplier), 255), 0);
			}
		}
	}
	else if (transformation == 3) {
		RGB_YCbCr_601();
		int mx = -1e9, mn = 1e9;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				mx = max(mx, pnm[i][j].r);
				mn = min(mn, pnm[i][j].r);
			}
		}
		offset = (int)mn;
		multiplier = 255. / ((int)mx - (int)mn);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				pnm[i][j].r = max(min((int)round((pnm[i][j].r - offset) * multiplier), 255), 0);
			}
		}
		YCbCr_601_RGB();
	}
	else if (transformation == 4) {
		vector<int>cnt(256, 0);
		int mx = 1e9, mn = 1e-9;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				cnt[pnm[i][j].r]++;
				cnt[pnm[i][j].g]++;
				cnt[pnm[i][j].b]++;
			}
		}
		int todelete = (int)round(width * height * 3 * 0.0039);		
		int deletecount = todelete;	
		for (mn = 0;mn<256;mn++) {
			if (cnt[mn] <= deletecount) {
				deletecount -= cnt[mn];				
			}
			else {
				break;		
			}
		}
		deletecount = todelete;
		for (mx = 255;mx >= 0;mx--) {
			if (cnt[mx] <= deletecount) {
				deletecount -= cnt[mx];
			}
			else {
				break;
			}
		}
		offset = mn;
		multiplier = 255. / (mx - mn);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				pnm[i][j].r = max(min((int)round((pnm[i][j].r - offset) * multiplier), 255), 0);
				pnm[i][j].g = max(min((int)round((pnm[i][j].g - offset) * multiplier), 255), 0);
				pnm[i][j].b = max(min((int)round((pnm[i][j].b - offset) * multiplier), 255), 0);
			}
		}
	}
	else if (transformation == 5) {
		RGB_YCbCr_601();
		vector<int>cnt(256, 0);
		int mx = 255, mn = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				cnt[pnm[i][j].r]++;
			}
		}
		int todelete = (int)round(width * height * 0.0039);
		int deletecount = todelete;
		for (mn = 0;mn < 256;mn++) {
			if (cnt[mn] <= deletecount) {
				deletecount -= cnt[mn];
			}
			else {
				break;
			}
		}
		deletecount = todelete;
		for (mx = 255;mx >= 0;mx--) {
			if (cnt[mx] <= deletecount) {
				deletecount -= cnt[mx];
			}
			else {
				break;
			}
		}
		offset = mn;
		multiplier = 255. / (mx - mn);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				pnm[i][j].r = max(min((int)round((pnm[i][j].r - offset) * multiplier), 255), 0);
			}
		}
		YCbCr_601_RGB();
	}
	if (transformation > 1)cout << offset << ' ' << multiplier;
}
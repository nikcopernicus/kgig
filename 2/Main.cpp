#include "Picture.h"
using namespace std;

int main(int argc, char *argv[]) {
	if ((argc != 9) && (argc != 10)) {
		cerr << "Error: Wrong number of arguments";
		return 1;
	}
	string input = argv[1];
	string output = argv[2];
	double brightness = (double)stoi(argv[3]), thickness = 1.0;
	int x0 = stoi(argv[5]), y0 = stoi(argv[6]), x1 = stoi(argv[7]), y1 = stoi(argv[8]);
	double gamma = 2.0;
	PGM *picture;
	try {
		picture = new PGM(input);
	}
	catch (exception e) {
		cerr << e.what();
		return 1;
	}
	try {
		if ((x0 == x1) && (y0 == y1)) {
			brightness = pow(brightness, gamma);
			picture->draw_pix(x0, y0, brightness, gamma, brightness);
		}
		else {
			picture->draw_line(brightness, x0, y0, x1, y1, gamma);
		}
	}
	catch (exception e) {
		cerr << e.what();
		return 1;
	}
	try {
		picture->output(output);
	}
	catch (exception e) {
		cerr << e.what();
		return 1;
	}
	return 0;
}
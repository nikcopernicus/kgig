#include "Picture.h"
using namespace std;

int main(int argc, char *argv[]) {
	
	if ((argc != 9) && (argc != 10)) {
		cerr << "Error: Wrong number of arguments";
		return 1;
	}
	string input = argv[1];
	string output = argv[2];
	double brightness = stoi(argv[3]), thickness = 1;
	double x1 = stod(argv[5]), y1 = stod(argv[6]), x2 = stod(argv[7]), y2 = stod(argv[8]);
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
		if ((x1 == x2) && (y1 == y2)) {
			brightness = pow(brightness, gamma);
			picture->draw_pix(x1, y1, brightness, gamma, brightness);
		}
		else {
			picture->draw_line(brightness, x1, y1, x2, y2, gamma);
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
	delete picture;
	return 0;
}
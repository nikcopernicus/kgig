#include "Picture.h"

int main(int argc, char* argv[]) {
	/*PNM pic("test.ppm");
	pic.auto_brightness(2, 0, 0);
	pic.output("out.ppm");*/
	if (argc != 4 && argc != 6) {
        cerr << "Error: Wrong number of arguments";
        return 1;
    }    
	string input, output;
	int mode = 0;	
	int offset = 0;
	double multiplier = 1;
	try {
		input = argv[1];
		output = argv[2];
		mode = atoi(argv[3]);
		if (mode < 2) {
			if (argc == 6) {
				offset = atoi(argv[4]);
				multiplier = atof(argv[5]);
			}
			else {
				cerr << "Error: Wrong number of arguments (need 6 args for mode < 2)";
				return 1;
			}
		}
	}
	catch (exception e) {
		cerr << "Error: Unable to convert arguments";
		return 1;
	}
	if (mode < 0 || mode>5 || offset < -255 || offset>255 || multiplier < 1. / 255. || multiplier>255.) {
		cerr << "Wrong arguments";
		return 1;
	}
	PNM* picture = nullptr;
	try {
		picture = new PNM(input);
	}
	catch (exception e) {
		cerr << e.what();
		return 1;
	}
	try {
		picture->auto_brightness(mode, offset, multiplier);
	}
	catch (exception e) {
		delete picture;
		cerr << e.what();
		return 1;
	}
	try {
		picture->output(output);
	}
	catch (exception e) {
		delete picture;
		cerr << e.what();
		return 1;
	}
	delete picture;

	return 0;
}

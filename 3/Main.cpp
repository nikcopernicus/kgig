#include "Picture.h"

int main(int argc, char* argv[]) {
	
	if (argc != 7) {
		cerr << "Error: Wrong number of arguments";
		return 1;
	}
	string input, output;
	bool gradient = 0;
	int dithering = 0, bitrate = 0;
	double gamma = 0;
	try{
		input = argv[1];
		output = argv[2];
		gradient = atoi(argv[3]);
		dithering = atoi(argv[4]);
		bitrate = atoi(argv[5]);
		gamma = atof(argv[6]);
	}
	catch (exception e) {
		cerr << "Error: Unable to convert arguments";
		return 1;
	}
	if (gradient < 0 || gradient > 1 || dithering < 0 || dithering > 7 || bitrate < 1 || bitrate > 8 || gamma < 0) {
		cerr << gradient << "Error: Wrong arguments" << dithering << ' ' << bitrate << ' ' << gamma;
		return 1;
	}
	PGM *picture;
	try {
		picture = new PGM(input,gradient,gamma);
	}
	catch (exception e) {
		cerr << e.what();
		return 1;
	}
	try {
		picture->dithering(dithering, bitrate);
	}
	catch (exception e) {
		cerr << e.what();
		return 1;
	}
	try {
		picture->output(output,gamma);
	}
	catch (exception e) {
		cerr << e.what();
		return 1;
	}
	delete picture;
	/*
	PGM pic("test2.pgm",0,1);
	pic.dithering(7, 1);
	pic.output("out.pgm",1);
	*/
	return 0;
}
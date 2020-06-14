#include "Picture.h"

int main(int argc, char* argv[]) {
	/*
	PPM pic("test.ppm");
	pic.convert("RGB", "HSL");
	pic.output("z.ppm")
	*/
	if (argc != 11) {
		cerr << "Error: Wrong number of arguments";
		return 1;
	}
	bool ff = 0, tt = 0, ii = 0, oo = 0;
	string from, to;
	int in_count = 0, out_count = 0;
	string input, output;
	try {
		for (int i = 1; i < 11; i++) {
			if (!strcmp(argv[i], "-f") && i + 1 < 11) {
				ff = 1;
				from = argv[i + 1];
			}
			if (!strcmp(argv[i], "-t") && i + 1 < 11) {
				tt = 1;
				to = argv[i + 1];
			}
			if (!strcmp(argv[i], "-i") && i + 2 < 11) {
				ii = 1;
				in_count = atoi(argv[i + 1]);
				input = argv[i + 2];
			}
			if (!strcmp(argv[i], "-o") && i + 2 < 11) {
				oo = 1;
				out_count = atoi(argv[i + 1]);
				output = argv[i + 2];
			}
		}
	}
	catch (exception e) {
		cerr << "Error: Unable to convert arguments";
		return 1;
	}

	if (ff + tt + ii + oo != 4) {
		cerr << "Error: Wrong arguments";
		return 1;
	}
	PPM* picture=nullptr;
	try {
		if (in_count == 1) {
			picture = new PPM(input);
		}
		if (in_count == 3) {
			string s1, s2, s3;
			int pnt = -1;
			for (int i = input.size() - 1; i >= 0; i--) {
				if (input[i] == '.') {
					pnt = i;
					break;
				}
			}
			if (pnt == -1) {
				cerr << "Error: Wrong input file pattern";
				return 1;
			}
			else {
				for (int i = 0;i < input.size(); i++) {
					if (i!=pnt) {
						s1 += input[i];
						s2 += input[i];
						s3 += input[i];
					}
					else {
						s1 += "_1.pgm";
						s2 += "_2.pgm";
						s3 += "_3.pgm";
						break;
					}
				}
			}
			picture = new PPM(s1,s2,s3);
		}
	}
	catch (exception e) {
		cerr << e.what();
		return 1;
	}
	try {
		picture->convert(from, to);
	}
	catch (exception e) {
		delete picture;
		cerr << e.what();
		return 1;
	}
	try {
		if (out_count == 1) {
			picture->output(output);
		}
		if (out_count == 3) {
			string s1, s2, s3;
			int pnt = -1;
			for (int i = output.size() - 1; i >= 0; i--) {
				if (output[i] == '.') {
					pnt = i;
					break;
				}
			}
			if (pnt == -1) {
				cerr << "Error: Wrong input file pattern";
				return 1;
			}
			else {
				for (int i = 0;i < output.size(); i++) {
					if (i != pnt) {
						s1 += output[i];
						s2 += output[i];
						s3 += output[i];
					}
					else {
						s1 += "_1.pgm";
						s2 += "_2.pgm";
						s3 += "_3.pgm";
						break;
					}
				}
			}
			picture->output(s1, s2, s3);
		}
	}
	catch (exception e) {
		delete picture;
		cerr << e.what();
		return 1;
	}
	delete picture;
	return 0;
}
/*
0 - инверсия,
1 - зеркальное отражение по горизонтали,
2 - зеркальное отражение по вертикали,
3 - поворот на 90 градусов по часовой стрелке,
4 - поворот на 90 градусов против часовой стрелки.
*/
#include "Picture.h"

int main(int argc, char* argv[]) {
	if (argc != 4) {
		cout << "Error: Wrong number of arguments";
		return -1;
	}
	string input = argv[1];
	string output = argv[2];
	string transformation = argv[3];

	ifstream fin(input, ios::binary);
	if (!fin.is_open()) {
		cout << "Error: File can't be opened";
		return -1;
	}
	string type;
	while (type.size() != 2) {
		char c;
		if (!fin.eof()) {
			fin >> c;
			type += c;
		}
		else {
			fin.close();
			cout << "Error: Wrong file type";
			return -1;
		}
	}
	fin.close();
	if ((type != "P5") && (type != "P6")) {
		cout << "Error: Wrong file type";
		return -1;
	}

	PNM *picture;
	try {
		if (type == "P5") {
			picture = new PGM(input);
		}
		else {
			picture = new PPM(input);
		}
	}
	catch(exception e){
		cout << e.what();
		return -1;
	}

	if (transformation == "0") {
		picture->inverse();
	}
	else if (transformation == "1") {
		picture->hflip();
	}
	else if (transformation == "2") {
		picture->vflip();
	}
	else if (transformation == "3") {
		picture->rrotate();
	}
	else if (transformation == "4") {
		picture->lrotate();
	}
	else {
		cout << "Error: Wrong transformation type";
		return -1;
	}

	try {
		picture->output(output);
	}
	catch (exception e) {
		cout << e.what();
		return -1;
	}

	return 0;
}
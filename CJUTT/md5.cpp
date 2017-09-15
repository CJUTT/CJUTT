// MD5.cpp : 定义控制台应用程序的入口点。
//
#include "file.h"

int main(int argc, char *argv[]) {
	std::string fname;
	fname = "in.txt";
	//while (std::cin >> fname) {
		file f = file(fname);
		//f.intype();
		f.declare();
		f.runmain();
	//}
	return 0;
}
// MD5.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "file.h"

int main(int argc, char *argv[]) {
	std::string fname;
	while (std::cin >> fname) {
		file f;
		f.init(fname);
		f.shengming();
		f.runmain();
	}
	return 0;
}
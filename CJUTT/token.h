#ifndef _TOKEN_H_
#define _TOKEN_H_

#include "all.h"

class token {
public:
	int type;
	std::string value;

	token() 
	{
		type = INT;
		value = "";
	}
	token(int t, std::string s)
	{
		type = t;
		value = s;
	}

	int _toInt(std::string x);
	float _toFloat(std::string x);
	std::string toString(int x);
	std::string toString(float x);

	friend token operator + (token a, token b) {
		if (((a.type == STRING) ^ (b.type == STRING)) || ((a.type != INT && a.type != REAL) ^ (b.type != INT && b.type != REAL))) {
			std::cout << "�Ӻ����ߵ����Ͳ�֧�ִ�����" << std::endl;
			exit(0);
			return token();
		}
		else if (a.type == STRING && b.type == STRING) {
			return token(STRING, a.value + b.value);
		}
		else if (a.type == INT && b.type == INT) {
			return token(INT, a.toString(a._toInt(a.value) + b._toInt(b.value)));
		}
		else {
			return token(REAL, a.toString(a._toFloat(a.value) + b._toFloat(b.value)));
		}
	}

	friend token operator-(token a, token b)
	{
		if ((a.type != INT && a.type != REAL) || (b.type != INT && b.type != REAL)) {
			std::cout << "�������ߵ����Ͳ�֧�ִ�����" << std::endl;
			exit(0);
			return token();
		}
		else if (a.type == INT && b.type == INT) {
			return token(INT, a.toString(a._toInt(a.value) - b._toInt(b.value)));
		}
		else {
			return token(REAL, a.toString(a._toFloat(a.value) - b._toFloat(b.value)));
		}
		return token();
	}

	friend token operator*(token a, token b)
	{
		if ((a.type != INT && a.type != REAL) || (b.type != INT && b.type != REAL)) {
			std::cout << "���Ͳ������˳˷����㡣" << std::endl;
			exit(0);
			return token();
		}
		else if (a.type == INT && b.type == INT) {
			return token(INT, a.toString(a._toInt(a.value) * b._toInt(b.value)));
		}
		else {
			return token(REAL, a.toString(a._toFloat(a.value) * b._toFloat(b.value)));
		}
	}

	friend token operator/(token a, token b)
	{
		if ((a.type != INT && a.type != REAL) || (b.type != INT && b.type != REAL)) {
			std::cerr << "���Ͳ������˳������㡣" << std::endl;
			exit(0);
			return token();
		}
		else if (a.type == INT && b.type == INT) {
			if (b.value == "0") {
				std::cerr << "Error:����Ϊ0" << std::endl;
				exit(0);
			}
			return token(INT, a.toString(a._toInt(a.value) / b._toInt(b.value)));
		}
		else {
			if (b._toFloat(b.value) == 0) {
				std::cerr << "Error:����Ϊ0" << std::endl;
				exit(0);
			}
			return token(REAL, a.toString(a._toFloat(a.value) / b._toFloat(b.value)));
		}
	}

	friend token operator%(token a, token b)
	{
		if ((a.type != INT && a.type != REAL) || (b.type != INT && b.type != REAL)) {
			std::cout << "���Ͳ�������ȡģ���㡣" << std::endl;
			exit(0);
		}
		else if (a.type == INT && b.type == INT) {
			return token(INT, a.toString(a._toInt(a.value) % b._toInt(b.value)));
		}
		else {
			std::cout << "ģ�����߱�ģ������Ϊʵ�͡�" << std::endl;
			exit(0);
		}
		return token();
	}

	friend token operator^(token a, token b) {
		if ((a.type != INT && a.type != REAL) || (b.type != INT && b.type != REAL)) {
			std::cout << "���Ͳ������˳˷����㡣" << std::endl;
			exit(0);
			return token();
		}
		else if (a.type == INT && b.type == INT) {
			return token(INT, a.toString(int(pow(a._toInt(a.value), b._toInt(b.value)))));
		}
		return token(REAL, a.toString(pow(a._toFloat(a.value), b._toFloat(b.value))));
	}

	friend token operator&&(token a, token b)
	{
		if ((a.type != INT && a.type != REAL) || (b.type != INT && b.type != REAL)) {
			std::cout << "���Ͳ�����������㡣" << std::endl;
			exit(0);
		}
		else if (a._toFloat(a.value) != 0 && a._toFloat(b.value) != 0) {
			return token(INT, "1");
		}
		else {
			return token(INT, "0");
		}
		return token();
	}

	friend token operator||(token a, token b)
	{
		if ((a.type != INT && a.type != REAL) || (b.type != INT && b.type != REAL)) {
			std::cout << "���Ͳ������˻���㡣" << std::endl;
			exit(0);
		}
		else if (a._toFloat(a.value) != 0 || a._toFloat(b.value) != 0) {
			return token(INT, "1");
		}
		else {
			return token(INT, "0");
		}
		return token();
	}

	friend token operator!(token a)
	{
		if (a.type != INT && a.type != REAL) {
			std::cout << "���Ͳ������˷Ǽ��㡣" << std::endl;
			exit(0);
		}
		else if (a._toFloat(a.value) != 0) {
			return token(INT, "0");
		}
		else {
			return token(INT, "1");
		}
		return token();
	}

	friend token operator>(token a, token b)
	{
		if ((a.type != INT && a.type != REAL) || (b.type != INT && b.type != REAL)) {
			std::cout << "���Ͳ������˴��ڼ��㡣" << std::endl;
			exit(0);
		}
		else if (a._toFloat(a.value) > b._toFloat(b.value)) {
			return token(INT, "1");
		}
		else {
			return token(INT, "0");
		}
		return token();
	}

	friend token operator<(token a, token b)
	{
		if ((a.type != INT && a.type != REAL) || (b.type != INT && b.type != REAL)) {
			std::cout << "���Ͳ�������С�ڼ��㡣" << std::endl;
			exit(0);
		}
		else if (a._toFloat(a.value) < a._toFloat(b.value)) {
			return token(INT, "1");
		}
		else {
			return token(INT, "0");
		}
		return token();
	}

	friend token operator>=(token a, token b)
	{
		if ((a.type != INT && a.type != REAL) || (b.type != INT && b.type != REAL)) {
			std::cout << "���Ͳ������˴��ڵ��ڼ��㡣" << std::endl;
			exit(0);
		}
		else if (a._toFloat(a.value) >= a._toFloat(b.value)) {
			return token(INT, "1");
		}
		else {
			return token(INT, "0");
		}
		return token();
	}

	friend token operator<=(token a, token b)
	{
		if ((a.type != INT && a.type != REAL) || (b.type != INT && b.type != REAL)) {
			std::cout << "���Ͳ�������С�ڵ��ڼ��㡣" << std::endl;
			exit(0);
		}
		else if (a._toFloat(a.value) <= a._toFloat(b.value)) {
			return token(INT, "1");
		}
		else {
			return token(INT, "0");
		}
		return token();
	}

	friend token operator!=(token a, token b)
	{
		if ((a.type != INT && a.type != REAL) || (b.type != INT && b.type != REAL)) {
			std::cout << "���Ͳ������˲����ڼ��㡣" << std::endl;
			exit(0);
		}
		else if (a._toFloat(a.value) != a._toFloat(b.value)) {
			return token(INT, "1");
		}
		else {
			return token(INT, "0");
		}
		return token();
	}

	friend token operator==(token a, token b)
	{
		if (((a.type == STRING) ^ (b.type == STRING)) || ((a.type != INT && a.type != REAL) ^ (b.type != INT && b.type != REAL))) {
			std::cout << "���Ͳ������˵��ڼ��㡣" << std::endl;
			exit(0);
		}
		else if (a.type == STRING && b.type == STRING) {
			if (a.value == b.value) {
				return token(INT, "1");
			}
			else {
				return token(INT, "0");
			}
		}
		else if (a.type == STRING || b.type == STRING) {
			std::cout << "���Ͳ������˵��ڼ��㡣" << std::endl;
			exit(0);
		}
		else if (a._toFloat(a.value) == a._toFloat(b.value)) {
			return token(INT, "1");
		}
		else {
			return token(INT, "0");
		}
		return token();
	}

};

inline std::string token::toString(int x)
{
	std::stringstream ss;
	ss << x;
	return ss.str();
}

inline std::string token::toString(float x)
{
	std::stringstream ss;
	ss << x;
	return ss.str();
}

inline int token::_toInt(std::string x)
{
	std::stringstream ss;
	ss << x;
	int ret;
	ss >> ret;
	return ret;
	return 0;
}

inline float token::_toFloat(std::string x)
{
	std::stringstream ss;
	ss << x;
	double ret;
	ss >> ret;
	return ret;
}


#endif
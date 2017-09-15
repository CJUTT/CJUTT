#ifndef _SCAN_H_
#define _SCAN_H_

#include "token.h"

class scan {
private:
	int pos;
	std::vector<token> stringToVectorToken(std::string buf);
public:
	std::vector<token> v;
	scan() {}
	scan(char _buf[]) {
		pos = 0;
		v = stringToVectorToken(std::string(_buf));
	}
	scan(std::string _buf) {
		pos = 0;
		v = stringToVectorToken(_buf);
		int x = 0;
	}
	scan(std::vector<token> _v) {
		pos = 0;
		v = _v;
	}
private:
	void advance() { ++pos;	}
	void skip_whitespace() {
		while (!end() && isSpace(v[pos]))
			advance();
	}
public:
	bool isSpace(token t) {
		if (t.value == " " || t.value == "\t") return true;
		return false;
	}
	token next();
	bool end();
	std::string toString();
	std::string firstToString(std::string);
};

inline bool scan::end() { return pos >= v.size(); }

inline token scan::next() {
	skip_whitespace();
	if (end()) {
		return token();
	}
	return v[pos++];
}

inline std::vector<token> scan::stringToVectorToken(std::string buf)
{
	int i = 0;
	while (i < buf.size()) {
		// ����
		if (buf[i] == '@') {
			std::string str = "";
			i++;
			while (buf[i] != '@') {
				str.append(1, buf[i]);
				i++;
			}
			i++;
			v.push_back(token(NEXTLINE, token().toString(token()._toInt(str))));
		}
		// tab
		else if (buf[i] == '\t') {
			i++;
			//v.push_back(token(TAB, "\t"));
			continue;
		}
		else if (buf[i] == ' ') {
			i++;
			//v.push_back(token(SPACE, " "));
			continue;
		}
		// ���� | ������ | �ؼ���
		else if ((buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= 'A' && buf[i] <= 'Z') || buf[i] == '_') {
			std::string str = "";
			while (((buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= 'A' && buf[i] <= 'Z') || buf[i] == '_' || (buf[i] >= '0' && buf[i] <= '9')) && i < buf.size()) {
				str.append(1, buf[i]);
				i++;
			}
			if (str == "int" || str == "real" || str == "string" || str == "while" || str == "if" || str == "else" || str == "in" || str == "out" || str == "return" || str == "break" || str == "continue" || str == "do" || str == "until") {
				v.push_back(token(KEY, str));
			}
			else {
				while (buf[i] == ' ')
					i++;
				if (buf[i] == '(') {
					int flag = 1;
					str.append(1, buf[i]);
					i++;
					while (i < buf.size() && flag && buf[i] != '@') {
						if (buf[i] == '(')
							flag++;
						else if (buf[i] == ')')
							flag--;
						str.append(1, buf[i]);
						i++;
					}
					if (flag == 0) {
						v.push_back(token(FUNCTION, str));
					}
					else {
						std::cerr << "������ƥ�䲻��������" << std::endl;
						exit(0);
					}
				}
				else {
					v.push_back(token(VARIABLE, str));
				}
			}
		}
		// ����233 | ʵ��2.33
		else if (buf[i] >= '0' && buf[i] <= '9') {
			std::string str = "";
			while (buf[i] >= '0' && buf[i] <= '9' && i < buf.size()) {
				str.append(1, buf[i]);
				i++;
			}
			if (i < buf.size() && buf[i] == '.') {
				str.append(1, buf[i]);
				i++;
				while (buf[i] >= '0' && buf[i] <= '9' && i < buf.size()) {
					str.append(1, buf[i]);
					i++;
				}
				v.push_back(token(REAL, str));
			}
			else {
				v.push_back(token(INT, str));
			}
		}
		// �ַ���"abcd"
		else if (buf[i] == '"') {
			i++;
			std::string str = "";
			while (buf[i] != '\"' && buf[i] != '@' && i < buf.size()) {
				if (buf[i] == '\\') {
					str.append(1, buf[i]);
					i++;
					if (buf[i] != '\"' && buf[i] != '0' && buf[i] != 'n' && buf[i] != 't' && buf[i] != '\\' && buf[i] != '\'') {
						std::cerr << "������'\\', �Ƿ��ַ�" << std::endl;
						exit(0);
					}
					else {
						str.append(1, buf[i]);
						i++;
					}
				}
				else {
					str.append(1, buf[i]);
					i++;
				}
			}
			if (i < buf.size() && buf[i] == '\"') {
				v.push_back(token(STRING, str));
				i++;
			}
			else {
				std::cerr << "�Ƿ����ַ���" << std::endl;
				exit(0);
			}
		}
		// ע�� | ����
		else if (buf[i] == '/') {
			i++;
			if (i < buf.size() && buf[i] == '/') {
				while (i < buf.size() && buf[i] != '@') {
					i++;
				}
			}
			else {
				v.push_back(token(DIV, "/"));
			}
		}
		// ��ֵ= | ����==
		else if (buf[i] == '=') {
			i++;
			if (i < buf.size() && buf[i] != '=') {
				v.push_back(token(ASSIGN, "="));
			}
			else {
				i++;
				v.push_back(token(EQ, "=="));
			}
		}
		// �Ӻ� | ����
		else if (buf[i] == '+') {
			i++;
			v.push_back(token(ADD, "+"));
		}
		// ���� | ����
		else if (buf[i] == '-') {
			i++;
			v.push_back(token(MINUS, "-"));
		}
		// �߼���
		else if (buf[i] == '!') {
			i++;
			v.push_back(token(NOT, "!"));
		}
		// < | <= | <>
		else if (buf[i] == '<') {
			i++;
			if (i < buf.size()) {
				if (buf[i] == '=') {
					i++;
					v.push_back(token(LE, "<="));
				}
				else if (buf[i] == '>') {
					i++;
					v.push_back(token(NE, "<>"));
				}
				else {
					v.push_back(token(LT, "<"));
				}
			}
		}
		// > | >=
		else if (buf[i] == '>') {
			i++;
			if (i < buf.size() && buf[i] == '=') {
				i++;
				v.push_back(token(GE, ">="));
			}
			else {
				v.push_back(token(GT, ">"));
			}
		}
		// �߼��� ||
		else if (buf[i] == '|') {
			i++;
			if (i >= buf.size() || buf[i] != '|') {
				std::cerr << "����|��" << std::endl;
				exit(0);
			}
			else {
				i++;
				v.push_back(token(OR, "||"));
			}
		}
		// �߼��� &&
		else if (buf[i] == '&') {
			i++;
			if (i >= buf.size() || buf[i] != '&') {
				std::cerr << "����&��" << std::endl;
				exit(0);
			}
			else {
				i++;
				v.push_back(token(AND, "&&"));
			}
		}
		// ģ
		else if (buf[i] == '%') {
			i++;
			v.push_back(token(MOD, "%"));
		}
		// �˺�
		else if (buf[i] == '*') {
			i++;
			v.push_back(token(MULTI, "*"));
		}
		// �˷�
		else if (buf[i] == '^') {
			i++;
			v.push_back(token(POW, "^"));
		}
		// �ֺ�
		else if (buf[i] == ';') {
			i++;
			v.push_back(token(FEN, ";"));
		}
		else if (buf[i] == ',') {
			i++;
			v.push_back(token(DOU, ","));
		}
		else if (buf[i] == '{') {
			std::stack<char> s;
			std::string str = "";
			s.push('{');
			i++;
			while (i < buf.size() && !s.empty()) {
				if (buf[i] == '{') {
					s.push('{');
					i++;
					str.append(1, '{');
				}
				else if (buf[i] == '}') {
					s.pop();
					if (!s.empty()) {
						str.append(1, '}');
					}
					i++;
				}
				else {
					str.append(1, buf[i]);
					i++;
				}
			}
			if (!s.empty()) {
				std::cerr << "�����Ų�ƥ��" << std::endl;
				exit(0);
			}
			v.push_back(token(CBRACKET, str));
		}
		// ������
		else if (buf[i] == '[' || buf[i] == ']') {
			std::string str = "";
			str.append(1, buf[i]);
			i++;
			v.push_back(token(SBRACKET, str));
		}
		// Բ����
		else if (buf[i] == '(' || buf[i] == ')') {
			std::string str = "";
			str.append(1, buf[i]);
			i++;
			v.push_back(token(RBRACKET, str));
		}
		// �ַ���ɾ��
		else if (buf[i] == '#') {
			i++;
			v.push_back(token(STRDEL, "#"));
		}
		// �ַ�������
		else if (buf[i] == '$') {
			i++;
			v.push_back(token(STRLINK, "$"));
		}
		else {
			// �Ƿ��ַ� �׳�����
			std::cerr << "�Ƿ��ַ�" << std::endl;
			exit(0);
		}
	}
	return v;
}

inline std::string scan::toString()
{
	std::string ret = "";
	for (int i = 0; i < v.size(); i++) {
		if (v[i].type == STRING) {
			ret = ret + "\"" + v[i].value + "\"";
		}
		else if (v[i].type == CBRACKET) {
			ret = ret + "{" + v[i].value + "}";
		}
		else if (v[i].type == NEXTLINE) {
			ret = ret + "@" + v[i].value + "@";
		}
		else if (v[i].value == "int" || v[i].value == "real" || v[i].value == "string" || v[i].value == "while" || v[i].value == "if" || v[i].value == "else" || v[i].value == "do" || v[i].value == "until" || v[i].value == "return" || v[i].value == "in" || v[i].value == "out" || v[i].value == "break" || v[i].value == "continue") {
			ret = ret + v[i].value + " ";
		}
		else {
			ret = ret + v[i].value + " ";
		}
	}
	return ret;
}

inline std::string scan::firstToString(std::string s)
{
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == '@') {
			std::cerr << "�Ƿ��ַ�@" << std::endl;
			exit(0);
		}
	}
	std::string str = "";
	int line = 1;
	str.append(1, '@');
	str = str + token().toString(line);
	str.append(1, '@');
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == '\n') {
			line++;
			str.append(1, '@');
			str = str + token().toString(line);
			str.append(1, '@');
		}
		else {
			str.append(1, s[i]);
		}
	}
	return str;
}

#endif
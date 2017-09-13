#ifndef _FILE_H_
#define _FILE_H_

#include "variable.h"

variable vardb;
datastack stk;
dataqueue que;

class calculate {
public:
	token solve(std::string buf) {
		return solve(scan(buf).v);
	}
	token solve(std::vector<token> buf) {
		for (std::vector<token>::iterator it = buf.begin(); it != buf.end(); it++) {
			while (it != buf.end() && (*it).type == NEXTLINE) {
				it = buf.erase(it);
			}
			if (it == buf.end())
				break;
		}
		if (buf.size() == 0)
			return token(None, "");
		if (buf[0].value == "int" && buf[0].type == KEY) {
			std::vector<token>::iterator it = buf.begin();
			buf.erase(it);
			return declare(buf, INT);
		}
		else if (buf[0].value == "real" && buf[0].type == KEY) {
			std::vector<token>::iterator it = buf.begin();
			buf.erase(it);
			return declare(buf, REAL);
		}
		else if (buf[0].value == "string" && buf[0].type == KEY) {
			std::vector<token>::iterator it = buf.begin();
			buf.erase(it);
			return declare(buf, STRING);
		}
		else if (buf[0].value == "return" && buf[0].type == KEY) {
			std::vector<token>::iterator it = buf.begin();
			buf.erase(it);
			token temp = solve(buf);
			if (temp.type == INT) {
				return token(RINT, temp.value);
			}
			else if (temp.type == REAL) {
				return token(RREAL, temp.value);
			}
			else if (temp.type == STRING) {
				return token(RSTRING, temp.value);
			}
		}
		else if (buf[0].value == "in" && buf[0].type == KEY) {
			return token(INT, tostring(in(buf)));
		}
		else if (buf[0].value == "out" && buf[0].type == KEY) {
			return token(INT, tostring(out(buf)));
		}
		else if (buf[0].value == "break" && buf[0].type == KEY) {
			return token(BREAK, "1");
		}
		else if (buf[0].value == "continue" && buf[0].type == KEY) {
			return token(CONTINUE, "1");
		}
		else {
			token ret = solve_kuohao(buf);
			return ret;
		}
	}
private:
	// ���ȴ�������
	token solve_kuohao(std::vector<token> buf) {
		std::vector<token> nex;
		for (int i = 0; i < buf.size(); i++) {
			std::vector<token> temp;
			std::stack<char> s;
			if (buf[i].value == "(") {
				s.push('(');
				i++;
				while (i < buf.size() && !s.empty()) {
					if (buf[i].value == "(") {
						temp.push_back(buf[i]);
						s.push('(');
						i++;
					}
					else if (buf[i].value == ")") {
						s.pop();
						if (!s.empty()) {
							temp.push_back(buf[i]);
							i++;
						}
					}
					else {
						temp.push_back(buf[i]);
						i++;
					}
				}
				if (!s.empty()) {
					std::cout << "Բ���Ų�ƥ��" << std::endl;
					exit(0);
				}
				nex.push_back(solve(temp));
			}
			else {
				nex.push_back(buf[i]);
			}
		}
		token ret = solve_fuzhi(nex);
		return ret;
	}
	// ����ֵ
	token solve_fuzhi(std::vector<token> buf) {
		int i;
		token cur;
		for (i = buf.size() - 1; i >= 0; i--) {
			if (buf[i].type == ASSIGN) {
				if (i - 1 < 0 || buf[i - 1].type != VARIABLE) {
					std::cout << "�Ⱥ���߲��ǿ��޸ĵ���ֵ" << std::endl;
					exit(0);
				}
				std::vector<token> _right;
				for (int j = i + 1; j < buf.size(); j++) {
					_right.push_back(buf[j]);
				}
				cur = assign(buf[i - 1], _right);
				break;
			}
		}

		if (i != -1) {
			std::vector<token> temp;
			for (int j = 0; j < i - 1; j++) {
				temp.push_back(buf[j]);
			}
			temp.push_back(cur);
			return solve_fuzhi(temp);
		}
		else {
			token ret = solve_zhengfu(buf);
			return ret;
		}
	}

	bool isCal(token x) {
		if (x.type >= 17 && x.type <= 30) return true;
		return false;
	}
	// ����������
	token solve_zhengfu(std::vector<token> buf) {
		std::vector<token> nex;
		int flag = 1;
		for (int i = 0; i < buf.size(); i++) {
			if (flag) {
				if (buf[i].type == ADD) {
					flag = 0;
					if (i + 1 >= buf.size()) {
						std::cout << "������+��" << std::endl;
						exit(0);
					}
					continue;
				}
				else if (buf[i].type == MINUS) {
					flag = 0;
					i++;
					if (i >= buf.size()) {
						std::cout << "������-��" << std::endl;
						exit(0);
					}
					token cur;
					if (buf[i].type == VARIABLE) {
						cur = var(buf[i]);
					}
					else if (buf[i].type == FUNCTION) {
						cur = fun(buf[i]);
					}
					else {
						cur = buf[i];
					}
					nex.push_back(token(INT, "0") - cur);
				}
				else {
					nex.push_back(buf[i]);
					flag = isCal(buf[i]);
					/*if (flag) {
					std::cout << "�����������" << std::endl;
					exit(0);
					}*/
				}
			}
			else {
				nex.push_back(buf[i]);
				flag = isCal(buf[i]);
			}
		}
		token ret = solve_fei(nex);
		return ret;
	}
	// �����߼���
	token solve_fei(std::vector<token> buf) {
		int i;
		token cur;
		for (i = buf.size() - 1; i >= 0; i--) {
			if (buf[i].type == NOT) {
				if (i + 1 >= buf.size() || (buf[i + 1].type != VARIABLE && buf[i + 1].type != FUNCTION && buf[i + 1].type != INT && buf[i + 1].type != REAL)) {
					std::cout << "�ǺŴ���" << std::endl;
					exit(0);
				}
				int j = i + 1;
				if (buf[j].type == VARIABLE) {
					cur = var(buf[j]);
				}
				else if (buf[j].type == FUNCTION) {
					cur = fun(buf[j]);
				}
				else {
					cur = buf[j];
				}
				cur = !cur;
				break;
			}
		}

		if (i != -1) {
			std::vector<token> temp;
			for (int j = 0; j < i; j++) {
				temp.push_back(buf[j]);
			}
			temp.push_back(cur);
			for (int j = i + 2; j < buf.size(); j++) {
				temp.push_back(buf[j]);
			}
			return solve_fei(temp);
		}
		else {
			token ret = solve_jing(buf);
			return ret;
		}
	}
	// �����ַ���ɾ��
	token solve_jing(std::vector<token> buf) {
		std::vector<token> nex;
		int i;
		for (i = 0; i < buf.size(); i++) {
			if (buf[i].type == STRDEL) {
				if (nex.size() == 0) {
					std::cout << "�Ƿ���#����" << std::endl;
					exit(0);
				}
				token cur = nex[nex.size() - 1];
				if (i <= 0 || cur.type != STRING) {
					std::cout << "�Ƿ���#����" << std::endl;
					exit(0);
				}
				else {
					if (nex[nex.size() - 1].value.size() == 0) {
						std::cout << "�Ƿ���#����:�ַ����Ѿ�Ϊ��" << std::endl;
						exit(0);
					}
					int si = nex[nex.size() - 1].value.size();
					if (nex[nex.size() - 1].value[si - 1] == '\"' || nex[nex.size() - 1].value[si - 1] == '0' || nex[nex.size() - 1].value[si - 1] == 'n' || nex[nex.size() - 1].value[si - 1] == 't' || nex[nex.size() - 1].value[si - 1] == '\\' || nex[nex.size() - 1].value[si - 1] == '\'') {
						int j = si - 2, cnt = 0;
						while (nex[nex.size() - 1].value[j] == '\\') {
							j--;
							cnt++;
						}
						if (cnt % 2) {
							nex[nex.size() - 1].value.resize(si - 2);
						}
						else {
							nex[nex.size() - 1].value.resize(si - 1);
						}
					}
				}
				break;
			}
			else {
				token cur;
				if (buf[i].type == VARIABLE) {
					cur = var(buf[i]);
				}
				else if (buf[i].type == FUNCTION) {
					cur = fun(buf[i]);
				}
				else {
					cur = buf[i];
				}
				nex.push_back(cur);
			}
		}
		if (i < buf.size()) {
			for (int j = i + 1; j < buf.size(); j++) {
				nex.push_back(buf[j]);
			}
			return solve_jing(nex);
		}
		else {
			token ret = solve_pow(nex);
			return ret;
		}
	}
	// ����˷�
	token solve_pow(std::vector<token> buf) {
		token x;
		int i = -1;
		for (i = buf.size() - 1; i >= 0; i--) {
			token cur = buf[i];
			if (buf[i].type == VARIABLE) {
				cur = var(buf[i]);
			}
			else if (buf[i].type == FUNCTION) {
				cur = fun(buf[i]);
			}
			else if (buf[i].value == "^") {
				if (i - 1 >= 0 && i + 1 < buf.size()) {
					token l = buf[i - 1], r = buf[i + 1];
					x = l ^ r;
					break;
				}
				else {
					std::cout << "�Ƿ��ĳ˷�����" << std::endl;
					exit(0);
					return token();
				}
			}
		}

		if (i != -1) {
			std::vector<token> nex;
			for (int j = 0; j < i - 1; j++) {
				nex.push_back(buf[j]);
			}
			nex.push_back(x);
			for (int j = i + 2; j < buf.size(); j++) {
				nex.push_back(buf[j]);
			}
			return solve_pow(nex);
		}
		else {
			token ret = solve_multidivmod(buf);
			return ret;
		}
	}
	// ����˳�ȡģ
	token solve_multidivmod(std::vector<token> buf) {
		std::vector<token> nex;
		std::deque<token> q;
		for (int i = 0; i < buf.size(); i++) {
			token cur = buf[i];
			if (buf[i].type == VARIABLE) {
				cur = var(buf[i]);
			}
			else if (buf[i].type == FUNCTION) {
				cur = fun(buf[i]);
			}
			if (!q.empty()) {
				if (q.front().value == "*") {
					q.pop_front();
					token pre = q.front();
					q.pop_front();
					q.push_front(pre * cur);
				}
				else if (q.front().value == "/") {
					q.pop_front();
					token pre = q.front();
					q.pop_front();
					q.push_front(pre / cur);
				}
				else if (q.front().value == "%") {
					q.pop_front();
					token pre = q.front();
					q.pop_front();
					q.push_front(pre % cur);
				}
				else {
					q.push_front(cur);
				}
			}
			else {
				q.push_front(cur);
			}
		}
		while (!q.empty()) {
			nex.push_back(q.back());
			q.pop_back();
		}
		return solve_addminus(nex);
	}
	// ����Ӽ�
	token solve_addminus(std::vector<token> buf) {
		std::vector<token> temp;
		std::deque<token> q;
		for (int i = 0; i < buf.size(); i++) {
			token cur = buf[i];
			if (buf[i].type == VARIABLE) {
				cur = var(buf[i]);
			}
			else if (buf[i].type == FUNCTION) {
				cur = fun(buf[i]);
			}
			if (!q.empty()) {
				if (q.front().value == "+") {
					q.pop_front();
					token pre = q.front();
					q.pop_front();
					q.push_front(pre + cur);
					if (pre.type == STRING && cur.type == STRING) {
						std::cout << "�ַ���������+����" << std::endl;
						exit(0);
					}
				}
				else if (q.front().value == "-") {
					q.pop_front();
					token pre = q.front();
					q.pop_front();
					q.push_front(pre - cur);
				}
				else if (q.front().value == "$") {
					q.pop_front();
					token pre = q.front();
					q.pop_front();
					if (pre.type == STRING && cur.type == STRING) {
						q.push_front(pre + cur);
					}
					else {
						std::cout << "$����ֻ�����ַ���" << std::endl;
						exit(0);
					}
				}
				else {
					q.push_front(cur);
				}
			}
			else {
				q.push_front(cur);
			}
		}
		while (!q.empty()) {
			temp.push_back(q.back());
			q.pop_back();
		}
		return solve_xiaoyu(temp);
	}
	// ����С�� ���� С�ڵ��� ���ڵ���
	token solve_xiaoyu(std::vector<token> buf) {
		std::vector<token> temp;
		std::deque<token> q;
		for (int i = 0; i < buf.size(); i++) {
			token cur = buf[i];
			if (buf[i].type == VARIABLE) {
				cur = var(buf[i]);
			}
			else if (buf[i].type == FUNCTION) {
				cur = fun(buf[i]);
			}
			if (!q.empty()) {
				if (q.front().value == ">") {
					q.pop_front();
					token pre = q.front();
					q.pop_front();
					q.push_front(pre > cur);
				}
				else if (q.front().value == ">=") {
					q.pop_front();
					token pre = q.front();
					q.pop_front();
					q.push_front(pre >= cur);
				}
				else if (q.front().value == "<") {
					q.pop_front();
					token pre = q.front();
					q.pop_front();
					q.push_front(pre < cur);
				}
				else if (q.front().value == "<=") {
					q.pop_front();
					token pre = q.front();
					q.pop_front();
					q.push_front(pre <= cur);
				}
				else {
					q.push_front(cur);
				}
			}
			else {
				q.push_front(cur);
			}
		}
		while (!q.empty()) {
			temp.push_back(q.back());
			q.pop_back();
		}
		return solve_eq(temp);
	}
	// ������ڲ�����
	token solve_eq(std::vector<token> buf) {
		std::vector<token> temp;
		std::deque<token> q;
		for (int i = 0; i < buf.size(); i++) {
			token cur = buf[i];
			if (buf[i].type == VARIABLE) {
				cur = var(buf[i]);
			}
			else if (buf[i].type == FUNCTION) {
				cur = fun(buf[i]);
			}
			if (!q.empty()) {
				if (q.front().value == "==") {
					q.pop_front();
					token pre = q.front();
					q.pop_front();
					q.push_front(pre == cur);
				}

				else if (q.front().value == "<>") {
					q.pop_front();
					token pre = q.front();
					q.pop_front();
					q.push_front(pre != cur);
				}
				else {
					q.push_front(cur);
				}
			}
			else {
				q.push_front(cur);
			}
		}
		while (!q.empty()) {
			temp.push_back(q.back());
			q.pop_back();
		}
		return solve_yuhuo(temp);
	}
	// �������
	token solve_yuhuo(std::vector<token> buf) {
		std::deque<token> q;
		for (int i = 0; i < buf.size(); i++) {
			token cur = buf[i];
			if (buf[i].type == VARIABLE) {
				cur = var(buf[i]);
			}
			else if (buf[i].type == FUNCTION) {
				cur = fun(buf[i]);
			}
			if (!q.empty()) {
				if (q.front().value == "||") {
					q.pop_front();
					token pre = q.front();
					q.pop_front();
					q.push_front(pre || cur);
				}
				else if (q.front().value == "&&") {
					q.pop_front();
					token pre = q.front();
					q.pop_front();
					q.push_front(pre && cur);
				}
				else {
					q.push_front(cur);
				}
			}
			else {
				q.push_front(cur);
			}
		}
		if (q.size() == 1) {
			return q.front();
		}
		else {
			std::cout << "���ǺϷ�����ʽ" << std::endl;
			exit(0);
		}
	}
	// ��ֵ
	token assign(myint &x, std::vector<token> buf)
	{
		token right = solve(buf);
		if (right.type == INT) {
			x = toint(right.value);
		}
		else if (right.type == REAL) {
			x = tofloat(right.value);
			std::cout << "warn:floatת����int" << std::endl;
		}
		else {
			std::cout << "���Ͳ�ƥ�䡣" << std::endl;
			exit(0);
		}
		return right;
	}
	// ��ֵ
	token assign(float & x, std::vector<token> buf)
	{
		token right = solve(buf);
		if (right.type == INT) {
			x = toint(right.value);
		}
		else if (right.type == REAL) {
			x = tofloat(right.value);
		}
		else {
			std::cout << "���Ͳ�ƥ�䡣" << std::endl;
			exit(0);
		}
		return right;
	}
	// ��ֵ
	token assign(std::string & x, std::vector<token> buf)
	{
		token right = solve(buf);
		if (right.type == STRING) {
			x = right.value;
		}
		else {
			std::cout << "���Ͳ�ƥ�䡣" << std::endl;
			exit(0);
		}
		return right;
	}
	// ��ֵ
	token assign(token x, std::vector<token> buf, int type = INT) {
		int flag = 0;
		token cur;
		if (x.type != VARIABLE) {
			std::cout << "�Ⱥ���߲��ǿ��޸ĵ���ֵ��" << std::endl;
			exit(0);
		}
		else {
			mydata* temp_data = vardb.find(name(x.value));
			if (temp_data == NULL) {
				vardb.newdata(name(x.value), type);
				temp_data = vardb.find(name(x.value));
				flag = 1;
			}
			if (temp_data->type == INT) {
				myint &t = temp_data->toint();
				cur = assign(t, buf);
			}
			else if (temp_data->type == REAL) {
				float &t = temp_data->toreal();
				cur = assign(t, buf);
			}
			else {
				std::string &t = temp_data->tostring();
				cur = assign(t, buf);
			}
		}
		return cur;
	}

	int toint(std::string x)
	{
		std::stringstream ss;
		ss << x;
		int ret;
		ss >> ret;
		return ret;
	}

	double tofloat(std::string x)
	{
		std::stringstream ss;
		ss << x;
		double ret;
		ss >> ret;
		return ret;
	}

	std::string tostring(int x)
	{
		std::stringstream ss;
		ss << x;
		return ss.str();
	}

	std::string tostring(double x)
	{
		std::stringstream ss;
		ss << x;
		return ss.str();
	}
	// ת������Ϊ����
	token var(token _var)
	{
		// �ѱ���(�ͺ���)ת���ɳ���
		mydata* curdata = vardb.find(name(_var.value));
		if (curdata == NULL) {
			std::cout << "��δ�����ı�����" << std::endl;
			exit(0);
		}
		else {
			int temp_data = curdata->type;
			if (temp_data == INT)
				return token(INT, tostring(curdata->toint()));
			else if (temp_data == REAL)
				return token(REAL, tostring(curdata->toreal()));
			else
				return token(STRING, curdata->tostring());
		}
	}
	// ת������Ϊ����
	token fun(token _fun) {
		std::string funName = "", p = "";
		std::vector<int> par;
		std::vector<token> temp;

		scan sc = scan(_fun.value);
		token cur = sc.next();

		int i;
		for (i = 0; i < cur.value.size(); i++) {
			if (cur.value[i] != '(') {
				funName.append(1, cur.value[i]);
			}
			else {
				break;
			}
		}

		for (int j = i + 1; j < cur.value.size() - 1; j++) {
			p.append(1, cur.value[j]);
		}

		sc = scan(p);
		cur = sc.next();
		temp.clear();
		int flag = 0;
		while (!sc.isSpace(cur)) {
			if (cur.value == ",") {
				flag = 1;
				cur = solve(temp);
				if (cur.value == "") {
					std::cout << "�����������ݴ���" << std::endl;
					exit(0);
				}
				else if (cur.type == INT) {
					par.push_back(INT);
					que.add(cur);
				}
				else if (cur.type == REAL) {
					par.push_back(REAL);
					que.add(cur);
				}
				else if (cur.type == STRING) {
					par.push_back(STRING);
					que.add(cur);
				}
				temp.clear();
				cur = sc.next();
				continue;
			}
			else {
				temp.push_back(cur);
			}
			if (sc.end()) {
				break;
			}
			cur = sc.next();
		}
		cur = solve(temp);
		if (cur.value != "") {
			if (cur.type == INT) {
				par.push_back(INT);
				que.add(cur);
			}
			else if (cur.type == REAL) {
				par.push_back(REAL);
				que.add(cur);
			}
			else if (cur.type == STRING) {
				par.push_back(STRING);
				que.add(cur);
			}
		}
		else if (flag) {
			std::cout << "�����������ݴ���" << std::endl;
			exit(0);
		}
		mydata* x = vardb.find(name(funName));
		if (x == NULL) {
			std::cout << "δ�����ĺ�����" << std::endl;
			exit(0);
		}
		if (x->type != FUNCTION) {
			std::cout << "�����Ʋ��Ǻ���" << std::endl;
			exit(0);
		}
		function *f = x->tofunction().find(par);
		if (f == NULL) {
			std::cout << "�����������Ͳ�ƥ��" << std::endl;
			exit(0);
		}
		/*for (int i = 0; i < parparpar.size(); i++) {
		que.add(parparpar[i]);
		}*/
		f->run();
		token y = stk.pop();
		return y;
	}

	// �����ʾ��
	void strout(std::string str) {
		for (int i = 0; i < str.size(); i++) {
			if (str[i] == '\\') {
				if (i == str.size() - 1) {
					std::cout << "illegal string" << std::endl;
					exit(0);
				}
				if (str[i + 1] == 'n') {
					std::cout << '\n';
					i++;
				}
				else if (str[i + 1] == 't') {
					std::cout << '\t';
					i++;
				}
				else if (str[i + 1] == '0') {
					break;
				}
				else if (str[i + 1] == '\\') {
					std::cout << '\\';
					i++;
				}
				else if (str[i + 1] == '\"') {
					std::cout << '\"';
					i++;
				}
				else if (str[i + 1] == '\'') {
					std::cout << '\'';
					i++;
				}
				else {
					std::cout << "illegal string" << std::endl;
					exit(0);
				}
			}
			else
				std::cout << str[i];
		}
	}

	int in(std::vector<token> buf)
	{
		if (buf[buf.size() - 1].value == "\n")
			buf.resize(buf.size() - 1);
		int len = buf.size(), i;
		std::string temp, s;
		if (len == 4 || len == 2)//�ж϶�̬���鳤�� �����뷽ʽΪ in+���� ����in+��ʾ�ַ�+����+���� 
		{

			if (len == 2) //�ж���������λ�� ��in+�������� 
				temp = buf[1].value;
			if (len == 4)//�жϱ�������λ�� 
			{
				temp = buf[3].value;
				strout(buf[1].value);//�����ʾ��Ϣ 
			}
			std::cin >> s;// ���û��������ַ�����ʽ����
			name a = name(temp);//����һ������Ϊnex���������ַ����ı��� 
			if (vardb.find(a) != NULL)//�����ѱ����� 
			{
				mydata* aa = vardb.find(a);
				int typ = aa->type;
				if (typ == STRING)//�ַ����͵ı��� 
				{
					std::string &t = aa->tostring();
					t = s;//�ַ����Ļ���ֵ
					return 1;
				}
				if (typ == REAL) //real�͵ı��� 
				{
					int pointflag = 0; //С������ 		 	
					int isize = s.size();
					for (i = 0; i < isize; i++)//�ж�������Ƿ���һ���������������ֻ��Ϊ���֣�'-'��'.'; 
					{
						if (s[0] == '.')
						{
							printf("���ݸ�ʽ����\n");
							exit(0);
						}

						if (s[0] == '-')// Ϊ���� 
							continue;
						if (s[i] == '.')
						{
							pointflag++;
							if (pointflag > 1)//�������ϵ�С���㱨�� 
							{
								printf("���ݸ�ʽ����");
								exit(0);
							}
							else
								continue;
						}
						else if (s[i] >= '0'&&s[i] <= '9') //���ڷ����� ֱ��exit 
							continue;
						else
							exit(0);
					}
					std::stringstream sstr(s);
					float x;//xΪת�� 
					sstr >> x;
					float &t = aa->toreal();
					t = x;
					return 1;
				}
				if (typ == INT)//int�ͱ��� 
				{
					int isize = s.size();
					for (i = 0; i < isize; i++)//�ж�������Ƿ���һ���������������ֻ��Ϊ���֣�'-'; ����012��Ϊ12 
					{
						if (s[0] == '-')   //Ϊ���� 
							continue;
						if (s[i] <= '9'&&s[i] >= '0')
						{
							continue;
						}
						else//���ڷ����� ֱ��exit
						{
							printf("���ݸ�ʽ����\n"); exit(0);
						}
					}
					std::stringstream sstr(s);
					int x;//xΪת�� 
					sstr >> x;
					myint &t = aa->toint();
					t = x;
				}

			}
			else
			{
				printf("����δ����\n");
				exit(0); //����δ���� 
			}
		}
		else {
			printf("���ݸ�ʽ����\n");
			exit(0);//����Υ������		
		}
	}

	/*
	out��vector���ȷ����ֺϷ��÷���
	2:	out <������>|"<��ʾ��>";
	4:	out (<���α���>|<���ͳ���>|"<��ʾ��>"), (<������>|"<��ʾ��>");
	6:	out (<���α���>|<���ͳ���>), "<��ʾ��>", (<������>|"<��ʾ��>");

	ptΪ���һ������������

	��Чת�Ʒ���'\n', '\t', '\0', '\\', '\��', '\��'

	���б�����Ҫ��ǰ���帳ֵ��
	*/
	
	int out(std::vector<token> buf) {
		std::vector<token> nex;
		nex.push_back(buf[0]);
		for (int i = 1; i < buf.size(); i++)
		{
			std::vector<token> temp;
			while (i < buf.size() && buf[i].value != ",") {
				temp.push_back(buf[i]);
				i++;
			}
			nex.push_back(solve(temp));
			if (i < buf.size())
				nex.push_back(buf[i]);
		}
		return out_(nex);
	}
	int out_(std::vector<token> buf) {
		int pt = 1;
		switch (buf.size()) {
		case 2:
			strout(buf[1].value);
			break;

		case 4:
			switch (buf[1].type) {
			case INT:
				pt = std::stoi(buf[1].value); break;
			case STRING:
				strout(buf[1].value); break;
			case REAL:
				std::cout << "�ظ������������Ϊ������" << std::endl;
				exit(0);
				break;
			case VARIABLE:
				mydata* temp_data1 = vardb.find(name(buf[1].value));
				if (temp_data1 == NULL || temp_data1->type != INT) {
					std::cout << "illegal variable" << std::endl;
					exit(0);
				}
				myint &t = temp_data1->toint();
				pt = t;
			}
			if (pt < 0) {
				std::cout << "�޷����������" << std::endl;
				exit(0);
			}
			for (int i = 0; i < pt; i++)
				strout(buf[3].value);
			break;

		case 6:
			if (buf[1].type == INT)
				pt = std::stoi(buf[1].value);
			else {
				mydata* temp_data1 = vardb.find(name(buf[1].value));
				if (temp_data1 == NULL || temp_data1->type != INT) {
					std::cout << "illegal variable" << std::endl;
					exit(0);
				}
				myint &t = temp_data1->toint();
				pt = t;
			}

			strout(buf[3].value);

			if (pt < 0) {
				std::cout << "�޷����������" << std::endl;
				exit(0);
			}

			for (int i = 0; i < pt; i++)
				std::cout << buf[5].value;
		default:
			std::cout << "���ǺϷ���������" << std::endl;
			exit(0);
			break;
		}
		return 1;
	}

	token declare(std::vector<token> buf, int type = INT) {
		for (int i = 0; i < buf.size(); i++) {
			std::vector<token> temp;
			if (buf[i].type == VARIABLE) {
				vardb.newdata(name(buf[i].value), type);
				temp.push_back(buf[i]);
			}
			else {
				std::cout << "�����Ĳ��Ǳ���" << std::endl;
				exit(0);
			}
			i++;
			while (i < buf.size() && buf[i].value != ",") {
				temp.push_back(buf[i]);
				i++;
			}
			solve(temp);
		}
		return token(INT, "1");
	}
};

calculate cal;

class breakpoint {
	bool brp[200];
	int last[10000], tot;
public:
	breakpoint() :tot(0) {}
	void add() {
		last[++tot] = -1;
	}
	void pop() {
		tot--;
	}
	void debug(int line) {
		if (!brp[line] || last[tot] == line)
			return;
		last[tot] = line;
		std::string temp;
		mydata* x;
		while (std::cin >> temp) {
			if (temp == "return")
				return;
			x = vardb.find(temp);
			if (x == NULL) {
				std::cout << "����δ����" << std::endl;
			}
			else {
				x->out();
			}
		}
	}
	void reset() {
		std::memset(brp, 0, sizeof(brp));
	}
	void reset(int x) {
		if (x < 0 || x >= 200) {
			std::cout << "ɾ���ϵ�ʧ��" << std::endl;
			exit(0);
		}
		brp[x] = 0;
	}
	void set(int x) {
		if (x < 0 || x >= 200) {
			std::cout << "���Ӷϵ�ʧ��" << std::endl;
			exit(0);
		}
		brp[x] = 1;
	}
	void brpinit() {
		int n, k;
		std::cin >> n;
		reset();
		for (int i = 0; i < n; i++) {
			std::cin >> k;
			set(k);
		}
	}
};
breakpoint debug;

class file{
private:
	char buf[100000];
	
	void declare_fun(std::vector<token> buf) {
		for (std::vector<token>::iterator it = buf.begin(); it != buf.end(); it++) {
			while (it != buf.end() && (*it).type == NEXTLINE) {
				it = buf.erase(it);
			}
			if (it == buf.end())
				break;
		}
		if (buf.size() != 3 && buf.size() != 2) {
			std::cout << "������������" << std::endl;
			exit(0);
		}
		else if (buf.size() != 3) {
			if (buf[0].type != FUNCTION || buf[1].type != CBRACKET) {
				std::cout << "������������" << std::endl;
				exit(0);
			}
			else {
				std::vector<int> pra;	// ��������
				std::string function_name, str;	// str: �����ڵ��ַ���
				int i = 0;
				while (buf[0].value[i] != '(') {
					function_name.append(1, buf[0].value[i]);
					i++;
				}
				i++;
				while (buf[0].value[i] != ')') {
					str.append(1, buf[0].value[i]);
					i++;
				}
				scan sc = scan(str);
				while (!sc.end()) {
					token cur;
					cur = sc.next();
					if (cur.value == "int") {
						pra.push_back(INT);
					}
					else if (cur.value == "real") {
						pra.push_back(REAL);
					}
					else if (cur.value == "string") {
						pra.push_back(STRING);
					}
					else {
						std::cout << "������������" << std::endl;
						exit(0);
					}

					if (sc.end()) {
						std::cout << "������������" << std::endl;
						exit(0);
					}
					cur = sc.next();
					if (cur.type != VARIABLE) {
						std::cout << "������������" << std::endl;
						exit(0);
					}

					if (sc.end()) {
						break;
					}
					cur = sc.next();
					if (cur.value != ",") {
						std::cout << "������������" << std::endl;
						exit(0);
					}
				}
				for (int i = 0; i < str.size(); i++) {
					if (str[i] == ',')
						str[i] = ';';
				}
				str.append(1, ';');
				vardb.newdata(name(function_name), pra, str + buf[1].value, INT);
			}
		}
		else if ((buf[0].value != "int") && (buf[0].value != "real") && (buf[0].value != "string") || buf[1].type != FUNCTION || buf[2].type != CBRACKET) {
			std::cout << "������������" << std::endl;
			exit(0);
		}
		else {
			std::vector<int> pra;	// ��������
			std::string function_name, str;	// str: �����ڵ��ַ���
			int i = 0;
			while (buf[1].value[i] != '(') {
				function_name.append(1, buf[1].value[i]);
				i++;
			}
			i++;
			while (buf[1].value[i] != ')') {
				str.append(1, buf[1].value[i]);
				i++;
			}
			scan sc = scan(str);
			while (!sc.end()) {
				token cur;
				cur = sc.next();
				if (cur.value == "int") {
					pra.push_back(INT);
				}
				else if (cur.value == "real") {
					pra.push_back(REAL);
				}
				else if (cur.value == "string") {
					pra.push_back(STRING);
				}
				else {
					std::cout << "������������" << std::endl;
					exit(0);
				}

				if (sc.end()) {
					std::cout << "������������" << std::endl;
					exit(0);
				}
				cur = sc.next();
				if (cur.type != VARIABLE) {
					std::cout << "������������" << std::endl;
					exit(0);
				}

				if (sc.end()) {
					break;
				}
				cur = sc.next();
				if (cur.value != ",") {
					std::cout << "������������" << std::endl;
					exit(0);
				}
			}
			for (int i = 0; i < str.size(); i++) {
				if (str[i] == ',')
					str[i] = ';';
			}
			if (str.size() != 0)
				str.append(1, ';');
			if (buf[0].value == "int") {
				vardb.newdata(name(function_name), pra, str + buf[2].value, INT);
			}
			else if (buf[0].value == "real") {
				vardb.newdata(name(function_name), pra, str + buf[2].value, REAL);
			}
			else {
				vardb.newdata(name(function_name), pra, str + buf[2].value, STRING);
			}
		}
	}

public:
	std::string str;
	int tot;
	file(std::string fname) :tot(0){
		vardb.newfloor();
		std::ifstream in;
		in.open(fname, std::ios::in);
		str.clear();
		while (!in.eof()) {
			in.getline(buf, 100);
			str = str + std::string(buf) + "\n";
		}
		in.close();
		str = scan().firstToString(str);
		}

	void declare() {
		scan sc = scan(str);
		std::vector<token> temp;
		for (int i = 0; i < sc.v.size(); i++) {
			if (sc.v[i].type == FEN) {
				cal.solve(temp);
				temp.clear();
			}
			else if (sc.v[i].type == CBRACKET) {
				temp.push_back(sc.v[i]);
				declare_fun(temp);
				temp.clear();
			}
			else {
				temp.push_back(sc.v[i]);
			}
		}
	}
	void runmain() {
		function fun;
		std::vector<int> pra;
		mydata* y = vardb.find(name("main"));
		fun = *y->tofunction().find(pra);
		fun.run();
	}
	~file() {
		vardb.deletefloor();
	}
};

inline int mywhile::run() {
	int ans = None;
	while (1) {
		debug.add();
		if (!judge.judge()) {
			debug.pop();
			break;
		}
		vardb.newfloor();
		for (int i = 0; i < sta.size(); i++) {
			ans = sta[i].run();
			if (ans == CONTINUE || ans == BREAK || ans == RETURN)
				break;
		}
		vardb.deletefloor();
		debug.pop();
		if (ans == CONTINUE) {
			ans = None;
		}
		else if (ans == BREAK) {
			ans = None;
			break;
		}
		else if (ans == RETURN) {
			break;
		}
	}
	return ans;
}

inline int dountil::run() {
	int ans = None;
	do {
		debug.add();
		vardb.newfloor();
		for (int i = 0; i < sta.size(); i++) {
			ans = sta[i].run();
			if (ans == CONTINUE || ans == BREAK || ans == RETURN)
				break;
		}
		vardb.deletefloor();
		if (ans == CONTINUE) {
			ans = None;
		}
		else if (ans == BREAK) {
			ans = None;
			debug.pop();
			break;
		}
		else if (ans == RETURN) {
			debug.pop();
			break;
		}
		if (judge.judge()) {
			debug.pop();
			break;
		}
		debug.pop();
	} while (1);
	return ans;
}

inline int ifelse::run() {
	int ans = None;
	if (judge.judge()) {
		vardb.newfloor();
		for (int i = 0; i < first.size(); i++) {
			ans = first[i].run();
			if (ans == CONTINUE || ans == BREAK || ans == RETURN)
				break;
		}
	}
	else {
		vardb.newfloor();
		for (int i = 0; i < second.size(); i++) {
			ans = second[i].run();
			if (ans == CONTINUE || ans == BREAK || ans == RETURN)
				break;
		}
	}
	vardb.deletefloor();
	return ans;
}

inline int function::run() {
	int ans = 0;
	debug.add();
	vardb.newfloor();
	for (int i = 0; i < pnum; i++)
		sta[i].init();
	for (int i = pnum; i < sta.size(); i++) {
		ans = sta[i].run();
		if (ans == CONTINUE || ans == BREAK || ans == RETURN)
			break;
	}
	vardb.deletefloor();
	debug.pop();
	if (ans == None) {
		std::cout << "�����޷���ֵ" << std::endl;
		exit(0);
	}
	if (ans == CONTINUE) {
		std::cout << "continue�޶�Ӧ��ƥ��" << std::endl;
		exit(0);
	}
	if (ans == BREAK) {
		std::cout << "break�޶�Ӧ��ƥ��" << std::endl;
		exit(0);
	}
	token t = stk.pop();
	if (t.type != type) {
		if (t.type == STRING ^type == STRING) {
			std::cout << "�������������޷�ת��" << std::endl;
			stk.add(t);
			exit(0);
		}
		t.type = type;
	}
	stk.add(t);
	return None;
}

inline bool minstatement::judge() {		//���ݽ����õ�token�ж���䷵��ֵ�Ƿ�Ϊ��
	debug.debug(line);
	return cal.solve(tok).value == "1";
}

inline int minstatement::run() {		//ִ�����,����return���ͣ���return����ѹ��ջ�ڲ�����RETURN������continue������CONTINUE������break����BREAK,���򷵻�None��
	debug.debug(line);
	token t = cal.solve(tok);
	if (t.type == RINT) {
		stk.add(token(INT, t.value));
		return RETURN;
	}
	else if (t.type == RREAL) {
		stk.add(token(REAL, t.value));
		return RETURN;
	}
	else if (t.type == RSTRING) {
		stk.add(token(STRING, t.value));
		return RETURN;
	}
	else if (t.type == BREAK) {
		return BREAK;
	}
	else if (t.type == CONTINUE) {
		return CONTINUE;
	}
	return None;
}

inline void minstatement::runinit() {
	debug.debug(line);
	token t = que.pop(), a;
	for (int i = 0; i < tok.size(); i++) {
		if (tok[i].type == VARIABLE) {
			a = tok[i];
		}
	}
	std::vector<token> temp;
	temp.push_back(a);
	temp.push_back(token(ASSIGN, "="));
	temp.push_back(t);
	cal.solve(tok);
	cal.solve(temp);
}

inline void strtotoken(std::string str, std::vector<token> &tok) {		//��һ���ַ�������Ϊtoken��
	tok = scan(str).v;
	int x = 0;
}

inline void strtosta(std::string str, std::vector<statement> &sta) {		//��һ���ַ�������Ϊ�����
	scan sc = scan(str);
	/*for (std::vector<token>::iterator it = sc.v.begin(); it != sc.v.end(); it++) {
		while (it != sc.v.end() && (*it).type == NEXTLINE) {
			it = sc.v.erase(it);
		}
		if (it == sc.v.end())
			break;
	}*/
	int i = 0, line = 0;
	while (i < sc.v.size()) {
		if (sc.v[i].type == NEXTLINE) {
			line = token()._toInt(sc.v[i].value);
			i++;
		}
		else if (sc.v[i].value == "if") {
			int j = i + 1;
			std::vector<token> temp;
			while (j < sc.v.size() && sc.v[j].type != CBRACKET) {
				temp.push_back(sc.v[j]);
				j++;
			}
			if (j + 2 < sc.v.size() && sc.v[j].type == CBRACKET && sc.v[j + 1].value == "else" && sc.v[j + 2].type != CBRACKET) {
				std::cout << "else����Ĩ�ʹ�����" << std::endl;
				exit(0);
			}
			else if (j + 2 < sc.v.size() && sc.v[j].type == CBRACKET && sc.v[j + 1].value == "else" && sc.v[j + 2].type == CBRACKET) {
				sta.push_back(statement(line, scan(temp).toString(), sc.v[j].value, sc.v[j + 2].value));
				i = j + 3;
			}
			else if (j < sc.v.size() && sc.v[j].type == CBRACKET) {
				sta.push_back(statement(line, scan(temp).toString(), sc.v[j].value, ""));
				i = j + 1;
			}
			else {
				std::cout << "if������" << std::endl;
				exit(0);
			}
		}
		else if (sc.v[i].value == "while") {
			int j = i + 1;
			std::vector<token> temp;
			while (j < sc.v.size() && sc.v[j].type != CBRACKET) {
				temp.push_back(sc.v[j]);
				j++;
			}
			if (j < sc.v.size() && sc.v[j].type == CBRACKET) {
				sta.push_back(statement(line, scan(temp).toString(), sc.v[j].value, MYWHILE));
				i = j + 1;
			}
			else {
				std::cout << "while������" << std::endl;
				exit(0);
			}
		}
		else if (sc.v[i].value == "do") {
			if (i + 2 < sc.v.size() && sc.v[i + 1].type == CBRACKET && sc.v[i + 2].value == "until") {
				int j = i + 3;
				std::vector<token> temp;
				while (sc.v[j].value != ";") {
					temp.push_back(sc.v[j]);
					j++;
				}
				sta.push_back(statement(line, sc.v[i + 1].value, scan(temp).toString(), DOUNTIL));
				i = j + 1;
			}
			else {
				std::cout << "do until������" << std::endl;
				exit(0);
			}
		}
		else {
			std::vector<token> temp;
			while (i < sc.v.size() && sc.v[i].value != ";") {
				temp.push_back(sc.v[i]);
				i = i + 1;
			}
			i = i + 1;
			sta.push_back(statement(scan(temp).toString(), line));
		}
	}
}

#endif
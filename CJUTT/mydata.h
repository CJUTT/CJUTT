#ifndef _MYDATA_H
#define _MYDATA_H_

#include "scan.h"

typedef std::string string;
typedef float real;
const int RAMMAX = 1e4;

real ramreal[RAMMAX];
int realtot;
string ramstring[RAMMAX];
int stringtot;

class myint {
private:
	unsigned char l;
	char r;
public:
	myint() {
		l = r = 0;
	}
	myint(int x) {
		x %= 65536;
		//cout << x << endl;
		l = x / 256;
		r = x % 256;
		if (r > 0 && x < 0) {
			l--;
		}
		if (r < 0 && x > 0) {
			l++;
		}
	}
	operator int() {
		int x = l * 256 + r;
		if (l > 128 || (l == 128 && r >= 0)) {
			x -= 65536;
		}
		return x;
	}
};
myint rammyint[RAMMAX];
int myinttot;

bool strisempty(std::string &str) {
	int i;
	for (i = 0; i < str.size(); i++) {
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\0')
			break;
	}
	return i == str.size();
}

void strtotoken(std::string str, std::vector<token> &tok);		//��һ���ַ�������Ϊtoken��

class minstatement{
private:
	int line;
	std::vector<token> tok;
public:
	bool judge();		//�ж���䷵��ֵ�Ƿ�Ϊ��
	void runinit();		//ִ�����������Ӷ�����ȡֵ��ʼ��
	minstatement(){}
	minstatement(std::string str,int _line) {
		memset(this, 0, sizeof(minstatement));
		line = _line;
		strtotoken(str, tok);
	}
	int run();
};
minstatement ramminstatement[RAMMAX];
int minstatementtot;

class statement{
private:
	int type;
	int p;
public:
	statement() {};
	statement(std::string one,int _line) {
		type = MINSTATEMENT;
		p = minstatementtot++;
		ramminstatement[p] = minstatement(one, _line);
	}
	statement(int _line, std::string one, std::string two, int _type);
	statement(int _line, std::string one, std::string two, std::string three);
	void init() {		//ִ�����������Ӷ�����ȡֵ��ʼ��
		if (type != MINSTATEMENT) {
			std::cerr << "ִ������������Ͳ���" << std::endl;
			exit(0);
		}
		ramminstatement[p].runinit();
	}
	int run();
};
statement ramstatement[RAMMAX];
int statementtot;

void strtosta(std::string str, int sta);	//��һ���ַ�������Ϊ�����

struct link {
	int v, next;
	link() { v = next = -1; }
};
link ramlink[RAMMAX];
int linktot;

class mywhile{
private:
	int judge;
	int sta;
public:
	mywhile(){}
	mywhile(std::string _judge, std::string s,int _line) {
		if (strisempty(_judge)) {
			std::cerr << "�ж�ʽ����Ϊ��" << std::endl;
			exit(0);
		}
		judge = minstatementtot++;
		ramminstatement[judge] = minstatement(_judge, _line);
		sta = linktot++;
		ramlink[sta] = link();
		strtosta(s, sta);
	}
	int run();
};
mywhile rammywhile[RAMMAX];
int mywhiletot;

class dountil {
private:
	int sta;
	int judge;
public:
	dountil(){}
	dountil(std::string s, std::string _judge,int _line) {
		sta = linktot++;
		ramlink[sta] = link();
		strtosta(s, sta);
		if (strisempty(_judge)) {
			std::cerr << "�ж�ʽ����Ϊ��" << std::endl;
			exit(0);
		}
		judge = minstatementtot++;
		ramminstatement[judge] = minstatement(_judge, _line);
	}
	int run();
};
dountil ramdountil[RAMMAX];
int dountiltot;

class ifelse{
private:
	int judge;
	int first, second;
public:
	ifelse(){}
	ifelse(std::string _judge,int _line, std::string one, std::string two){
		if (strisempty(_judge)) {
			std::cerr << "�ж�ʽ����Ϊ��" << std::endl;
			exit(0);
		}
		judge = minstatementtot++;
		ramminstatement[judge] = minstatement(_judge, _line);
		first = linktot++;
		ramlink[first] = link();
		strtosta(one, first);
		second = linktot++;
		ramlink[second] = link();
		strtosta(two, second);
	}
	int run();
};
ifelse ramifelse[RAMMAX];
int ifelsetot;

class function{				//INT��REAL��STRING��type������0,1,2,None��ʾ��
private:
	function* next[3];
public:
	int type;
	int pnum;		//��������
	int sta;
	function() {
		type = None;
		pnum = 0;
		sta = linktot++;
		ramlink[sta] = link();
		for (int i = 0; i < 3; i++)
			next[i] = NULL;
	}
	function* newfunction();
	void add(std::vector<int>&a, std::string &s, int ty = INT) {			//���غ����������б��������ݣ������������ͣ�
		function *temp = this;
		for (int i = 0; i < a.size(); i++) {
			if (temp->next[a[i]] == NULL) {
				temp->next[a[i]] = newfunction();
			}
			temp = temp->next[a[i]];
		}
		if (temp->type == None) {
			temp->type = ty;
			temp->pnum = a.size();
			strtosta(s, temp->sta);
		}
		else {
			//�����ض���
			exit(0);
		}
	}
	int run();
	function *find(std::vector<int>&a) {		//���ݲ����б��ѯ����
		function *temp = this;
		for (int i = 0; i < a.size(); i++) {
			if (temp->next[a[i]] == NULL) {
				//����δ����
				return NULL;
			}
			temp = temp->next[a[i]];
		}
		return temp;
	}
	void clear() {
		for (int i = 0; i < 3; i++) {
			if (next[i] == NULL)
				continue;
			next[i]->clear();
		}
	}
	~function() {
		clear();
	}
};
function ramfunction[RAMMAX];
int functiontot;

class name {		//��ѯ��ʶ/��һ�ؼ���
	int l;				//��ʶ�ַ�������
	char v[9];			//��ʶ�ַ���
public:
	name() { l = 0; v[0] = '\0'; }
	name(char* s) {
		set(s);
	}
	name(std::string s) {
		for (l = 0; l < s.size() && l < 8; l++)
			v[l] = s[l];
		v[l] = '\0';
	}
	void set(char* s) {			//���ñ�ʶ��
		for (l = 0; l < 8 && s[l] != '\0'; l++)
			v[l] = s[l];
		v[l] = '\0';
	}
	bool operator <(const name q)const {
		char len = l < q.l ? l : q.l;
		for (char i = 0; i < len; i++) {
			if (v[i] != q.v[i])
				return v[i] < q.v[i];
		}
		return l < q.l;
	}
};

struct mydata {		//���ݴ洢�ṹ
	name key;	//��ѯ��ʶ/�ؼ���
	int type;	//��������:0Ϊint,1Ϊfloat��2Ϊchar*
	int deep;	//���ڲ���
	int p;	//��ǰָ��
	mydata *f;	//ͬ���ϼ�ָ��
	mydata *next;	//ͬ����ָ��
	mydata() { type = None; p = None; }
	mydata(name &n, int &now, mydata* &head, int ty = INT) {		//���������ݽڵ�
		key = n;
		type = ty;
		if (type == INT) {
			p = myinttot++;
			rammyint[p] = myint();
		}
		else if (type == REAL) {
			p = realtot++;
			ramreal[p] = real();
		}
		else if (type == STRING) {
			p = stringtot++;
			ramstring[p] = string();
		}
		else if (type == FUNCTION) {
			p = functiontot++;
			ramfunction[p] = function();
		}
		deep = now;
		next = head;
		head = this;
	}
	//����type��ͬ���ò�ͬto������������������
	myint &toint() {
		return rammyint[p];
	}
	real &toreal() {
		return ramreal[p];
	}
	string &tostring() {
		return ramstring[p];
	}
	function &tofunction() {
		return ramfunction[p];
	}
	void out() {
		if (type == INT) {
			std::cout << rammyint[p] << std::endl;
		}
		else if (type == REAL) {
			std::cout << ramreal[p] << std::endl;
		}
		else if (type == STRING) {
			std::cout << ramstring[p] << std::endl;
		}
		else if (type == FUNCTION) {
			std::cerr << "�����޷���ʾ" << std::endl;
		}
	}
};

class datastack {						//ջ�����غ���ǰ��������ֵѹ��ջ�У����غ󵯳�
#define SIZE 1000
private:
	token data[SIZE];
	int tot;
public:
	datastack() :tot(0) {};
	bool empty() {
		return tot == 0;
	}
	void add(token a) {
		if (tot < SIZE) {
			data[tot++] = a;
		}
	}
	token pop() {
		if (tot > 0)
			return data[--tot];
		else
			return token();
	}
	token top() {
		if (tot > 0)
			return data[tot - 1];
		else
			return token();
	}
	~datastack() {
		while (!empty()) {
			pop();
		}
	}
#undef SIZE
};

class dataqueue {						//���У����ú���ǰ�������ݲ���˳���������У����ú�˳�����
#define SIZE 1000
private:
	token data[SIZE];
	int st, ed;
public:
	dataqueue() :st(0), ed(0) {};
	bool empty() {
		return st == ed;
	}
	void add(token a) {
		data[ed++] = a;
		ed %= SIZE;
	}
	token pop() {
		token temp;
		if (st != ed) {
			temp = data[st++];
			st %= SIZE;
		}
		return temp;
	}
	~dataqueue() {
		while (!empty()) {
			pop();
		}
	}
#undef SIZE
};

void totinit() {
	myinttot = 0;
	realtot = 0;
	stringtot = 0;
	functiontot = 0;
	minstatementtot = 0;
	statementtot = 0;
	ifelsetot = 0;
	mywhiletot = 0;
	dountiltot = 0;
	linktot = 0;
}

inline function* function::newfunction() {
	int now = functiontot++;
	ramfunction[now] = function();
	return &ramfunction[now];
}

inline int statement::run() {
	if (type == MINSTATEMENT)
		return ramminstatement[p].run();
	else if (type == MYWHILE)
		return rammywhile[p].run();
	else if (type == IFELSE)
		return ramifelse[p].run();
	else if(type==DOUNTIL)
		return ramdountil[p].run();
}

inline statement::statement(int _line, std::string one, std::string two, int _type = MYWHILE) {
	type = _type;
	if (type == MYWHILE) {
		p = mywhiletot++;
		rammywhile[p] = mywhile(one, two, _line);
	}
	else if (type == DOUNTIL) {
		p = dountiltot++;
		ramdountil[p] = dountil(one, two, _line);
	}
	else {
		std::cerr << "������Ͳ���" << std::endl;
		exit(0);
	}
}

inline statement::statement(int _line, std::string one, std::string two, std::string three) {
	type = IFELSE;
	p = ifelsetot++;
	ramifelse[p] = ifelse(one, _line, two, three);
}

#endif

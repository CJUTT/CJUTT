#ifndef _MYDATA_H
#define _MYDATA_H_

#include "scan.h"

void strtotoken(std::string str, std::vector<token> &tok);		//��һ���ַ�������Ϊtoken��

class minstatement{
private:
	std::vector<token> tok;
public:
	minstatement() :tok(){}
	minstatement(std::string str) {
		strtotoken(str, tok);
	}
	bool judge();		//�ж���䷵��ֵ�Ƿ�Ϊ��
	void init();		//ִ�����������Ӷ�����ȡֵ��ʼ��
	int run();
};

class statement{
private:
	int type;
	void *p;
public:
	statement(std::string one) {
		type = MINSTATEMENT;
		p = new minstatement(one);
	}
	statement(std::string one, std::string two, int _type);
	statement(std::string one, std::string two, std::string three);
	void init() {		//ִ�����������Ӷ�����ȡֵ��ʼ��
		((minstatement*)p)->init();
	}
	int run();
};

void strtosta(std::string str, std::vector<statement> &sta);	//��һ���ַ�������Ϊ�����

class mywhile{
private:
	minstatement judge;
	std::vector<statement>sta;
public:
	mywhile(std::string _judge, std::string s) :judge(_judge) {
		strtosta(s, sta);
	}
	int run();
};

class dountil {
private:
	std::vector<statement>sta;
	minstatement judge;
public:
	dountil(std::string s, std::string _judge) {
		strtosta(s, sta);
		judge = minstatement(_judge);
	}
	int run();
};

class ifelse{
private:
	minstatement judge;
	std::vector<statement>first, second;
public:
	ifelse(std::string _judge, std::string one, std::string two) :judge(_judge) {
		strtosta(one, first);
		strtosta(two, second);
	}
	int run();
};

class function{				//INT��REAL��STRING��type������0,1,2,None��ʾ��
private:
	function* next[3];
public:
	int type;
	int pnum;		//��������
	std::vector<statement> sta;
	function() :type(None),pnum(0) {
		for (int i = 0; i < 3; i++)
			next[i] = NULL;
	}
	void add(std::vector<int>&a, std::string &s, int ty = INT) {			//���غ����������б��������ݣ������������ͣ�
		function *temp = this;
		for (int i = 0; i < a.size(); i++) {
			if (temp->next[a[i]] == NULL) {
				temp->next[a[i]] = new function();
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
	~function() {
		for (int i = 0; i < 3; i++)
			delete next[i];
	}
};

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
	/*union {
		myint _int;
		float _real;
		std::string _str;
		function fun;
	};*/
	void *p;	//��ǰָ��
	mydata *f;	//ͬ���ϼ�ָ��
	mydata *next;	//ͬ����ָ��
	mydata() { type = None; p = NULL; }
	mydata(name &n, int &now, mydata* &head, int ty = INT) {		//���������ݽڵ�
		key = n;
		type = ty;
		if (type == INT)
			p = new int;
		else if (type == REAL)
			p = new float;
		else if (type == STRING)
			p = new std::string;
		else if (type == FUNCTION)
			p = new function();
		deep = now;
		next = head;
		head = this;
	}
	//����type��ͬ���ò�ͬto������������������
	int &toint() {
		return *((int*)p);
	}
	float &toreal() {
		return *((float*)p);
	}
	std::string &tostring() {
		return *((std::string*)p);
	}
	function &tofunction() {
		return *((function*)p);
	}
	void out() {
		if (type == INT) {
			std::cout << toint() << std::endl;
		}
		else if (type == REAL) {
			std::cout << toreal() << std::endl;
		}
		else if (type == STRING) {
			std::cout << tostring() << std::endl;
		}
		else if (type == FUNCTION) {
			std::cout << "�����޷���ʾ" << std::endl;
		}
	}
	~mydata() {
		if (type == INT)
			delete (int*)p;
		else if (type == REAL)
			delete (float*)p;
		else if (type == STRING)
			delete (std::string *)p;
		else if (type == FUNCTION)
			delete (function *)p;
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

inline int statement::run() {
	if (type == MINSTATEMENT)
		return ((minstatement*)p)->run();
	else if (type == MYWHILE)
		return ((mywhile*)p)->run();
	else if (type == IFELSE)
		return ((ifelse*)p)->run();
	else if(type==DOUNTIL)
		return ((dountil*)p)->run();
}

inline statement::statement(std::string one, std::string two, int _type = MYWHILE) {
	type = _type;
	if (type == MYWHILE)
		p = new mywhile(one, two);
	else
		p = new dountil(one, two);
}

inline statement::statement(std::string one, std::string two, std::string three) {
	type = IFELSE;
	p = new ifelse(one, two, three);
}


#endif

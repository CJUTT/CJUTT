#ifndef _MYDATA_H
#define _MYDATA_H_

#include "scan.h"

void strtotoken(std::string str, std::vector<token> &tok);		//将一句字符串解析为token组

class minstatement{
private:
	int line;
	std::vector<token> tok;
public:
	minstatement(std::string str) {
		strtotoken(str, tok);
	}
	bool judge();		//判断语句返回值是否为真
	void init();		//执行声明，并从队列中取值初始化
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
	statement(std::string one, std::string two);
	statement(std::string one, std::string two, std::string three);
	void init() {		//执行声明，并从队列中取值初始化
		((minstatement*)p)->init();
	}
	int run();
};

void strtosta(std::string str, std::vector<statement> &sta);	//将一段字符串解析为语句组

class mywhile{
private:
	minstatement judge;
	std::vector<statement>sta;
public:
	mywhile(std::string jud, std::string s) :judge(jud) {
		strtosta(s, sta);
	}
	int run();
};

class ifelse{
private:
	minstatement judge;
	std::vector<statement>first, second;
public:
	ifelse(std::string jud, std::string one, std::string two) :judge(jud) {
		strtosta(one, first);
		strtosta(two, second);
	}
	int run();
};

class function{				//INT，REAL，STRING的type必须是0,1,2,None表示空
private:
	function* next[3];
public:
	int type;
	int pnum;		//参数个数
	std::vector<statement> sta;
	function() :type(None),pnum(0) {
		for (int i = 0; i < 3; i++)
			next[i] = NULL;
	}
	void add(std::vector<int>&a, std::string &s, int ty = INT) {			//重载函数（参数列表，函数内容，函数返回类型）
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
			//函数重定义
			exit(0);
		}
	}
	int run();
	function *find(std::vector<int>&a) {		//根据参数列表查询函数
		function *temp = this;
		for (int i = 0; i < a.size(); i++) {
			if (temp->next[a[i]] == NULL) {
				//函数未定义
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

class name {		//查询标识/第一关键字
	int l;				//标识字符串长度
	char v[9];			//标识字符串
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
	void set(char* s) {			//设置标识符
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

struct mydata {		//数据存储结构
	name key;	//查询标识/关键字
	int type;	//数据类型:0为int,1为float，2为char*
	int deep;	//所在层数
	/*union {
		myint _int;
		float _real;
		std::string _str;
		function fun;
	};*/
	void *p;	//当前指针
	mydata *f;	//同名上级指针
	mydata *next;	//同层后继指针
	mydata() { type = None; p = NULL; }
	mydata(name &n, int &now, mydata* &head, int ty = INT) {		//创建新数据节点
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
	//根据type不同调用不同to函数，返回数据引用
	int &toint() {
		return *((int*)p);
	}
	float &tofloat() {
		return *((float*)p);
	}
	std::string &tostring() {
		return *((std::string*)p);
	}
	function &tofunction() {
		return *((function*)p);
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

class datastack {						//栈，返回函数前，将返回值压入栈中，返回后弹出
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

class dataqueue {						//队列，调用函数前，将传递参数顺序加入队列中，调用后顺序调出
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
}

inline statement::statement(std::string one, std::string two) {
	type = MYWHILE;
	p = new mywhile(one, two);
}

inline statement::statement(std::string one, std::string two, std::string three) {
	type = IFELSE;
	p = new ifelse(one, two, three);
}


#endif

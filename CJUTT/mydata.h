#ifndef _MYDATA_H
#define _MYDATA_H_

#include "scan.h"

typedef std::string string;
typedef float real;

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

const int sizeof_int = sizeof(myint);
const int sizeof_real = sizeof(real);
const int sizeof_string = sizeof(string);

#define RAMMAX 100000
char ram[RAMMAX];
int ramtot = 0;

bool strisempty(std::string &str) {
	int i;
	for (i = 0; i < str.size(); i++) {
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\0')
			break;
	}
	return i == str.size();
}

void strtotoken(std::string str, std::vector<token> &tok);		//将一句字符串解析为token组

class minstatement{
private:
	std::vector<token> tok;
public:
	bool judge();		//判断语句返回值是否为真
	void runinit();		//执行声明，并从队列中取值初始化
	void init(std::string str) {
		strtotoken(str, tok);
	}
	int run();
	~minstatement() {
		tok.clear();
	}
};

class statement{
private:
	int type;
	int p;
public:
	statement(std::string one) {
		type = MINSTATEMENT;
		ram[ramtot++] = 1;
		p = ramtot;
		ramtot += sizeof(minstatement);
		((minstatement*)(ram + p))->init(one);
	}
	statement(std::string one, std::string two, int _type);
	statement(std::string one, std::string two, std::string three);
	void init() {		//执行声明，并从队列中取值初始化
		if (type != MINSTATEMENT) {
			std::cout << "执行声明语句类型不符" << std::endl;
			exit(0);
		}
		((minstatement*)(ram + p))->runinit();
	}
	int run();
};

void strtosta(std::string str, std::vector<statement> &sta);	//将一段字符串解析为语句组

class mywhile{
private:
	minstatement judge;
	std::vector<statement>sta;
public:
	void init(std::string _judge, std::string s) {
		if (strisempty(_judge)) {
			std::cout << "判断式不可为空" << std::endl;
			exit(0);
		}
		sta.clear();
		judge.init(_judge);
		strtosta(s, sta);
	}
	int run();
};

class dountil {
private:
	std::vector<statement>sta;
	minstatement judge;
public:
	void init(std::string s, std::string _judge) {
		strtosta(s, sta);
		if (strisempty(_judge)) {
			std::cout << "判断式不可为空" << std::endl;
			exit(0);
		}
		judge.init(_judge);
	}
	int run();
};

class ifelse{
private:
	minstatement judge;
	std::vector<statement>first, second;
public:
	void init(std::string _judge, std::string one, std::string two){
		if (strisempty(_judge)) {
			std::cout << "判断式不可为空" << std::endl;
			exit(0);
		}
		judge.init(_judge);
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
	void init() {
		type = None;
		pnum = 0;
		sta.clear();
		for (int i = 0; i < 3; i++)
			next[i] = NULL;
	}
	function* newfunction() {
		ram[ramtot++] = 1;
		int pnow = ramtot;
		ramtot += sizeof(function);
		((function*)(ram + pnow))->init();
		return (function*)(ram + pnow);
	}
	void add(std::vector<int>&a, std::string &s, int ty = INT) {			//重载函数（参数列表，函数内容，函数返回类型）
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
	void clear() {
		for (int i = 0; i < 3; i++) {
			if (next[i] == NULL)
				continue;
			next[i]->clear();
		}
		sta.clear();
	}
	~function() {
		clear();
	}
};

const int sizeof_function = sizeof(function);

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
	int p;	//当前指针
	mydata *f;	//同名上级指针
	mydata *next;	//同层后继指针
	mydata() { type = None; p = None; }
	mydata(name &n, int &now, mydata* &head, int ty = INT) {		//创建新数据节点
		key = n;
		type = ty;
		if (type == INT) {
			ram[ramtot++] = 1;
			p = ramtot;
			ramtot += sizeof_int;
			*((myint*)(ram + p)) = 0;
		}
		else if (type == REAL) {
			ram[ramtot++] = 1;
			p = ramtot;
			ramtot += sizeof_real;
			*((real*)(ram + p)) = 0.0;
		}
		else if (type == STRING) {
			ram[ramtot++] = 1;
			p = ramtot;
			ramtot += sizeof_string;
			*((string*)(ram + p)) = "";
		}
		else if (type == FUNCTION) {
			ram[ramtot++] = 1;
			p = ramtot;
			ramtot += sizeof_function;
			((function*)(ram + p))->init();
		}
		deep = now;
		next = head;
		head = this;
	}
	//根据type不同调用不同to函数，返回数据引用
	myint &toint() {
		return *((myint*)(ram + p));
	}
	real &toreal() {
		return *((real*)(ram + p));
	}
	string &tostring() {
		return *((std::string*)(ram + p));
	}
	function &tofunction() {
		return *((function*)(ram + p));
	}
	void out() {
		if (type == INT) {
			std::cout << *((myint*)(ram + p)) << std::endl;
		}
		else if (type == REAL) {
			std::cout << *((real*)(ram + p)) << std::endl;
		}
		else if (type == STRING) {
			std::cout << *((string*)(ram + p)) << std::endl;
		}
		else if (type == FUNCTION) {
			std::cout << "函数无法显示" << std::endl;
		}
	}
	~mydata() {
		if (type == INT) {
			*((myint*)(ram + p)) = 0;
		}
		else if (type == REAL) {
			*((real*)(ram + p)) = 0.0;
		}
		else if (type == STRING) {
			*((string*)(ram + p)) = "";
		}
		else if (type == FUNCTION) {
			((function*)(ram + p))->clear();
		}
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
		return ((minstatement*)(ram + p))->run();
	else if (type == MYWHILE)
		return ((mywhile*)(ram + p))->run();
	else if (type == IFELSE)
		return ((ifelse*)(ram + p))->run();
	else if(type==DOUNTIL)
		return ((dountil*)(ram + p))->run();
}

inline statement::statement(std::string one, std::string two, int _type = MYWHILE) {
	type = _type;
	if (type == MYWHILE) {
		ram[ramtot++] = 1;
		p = ramtot;
		ramtot += sizeof(mywhile);
		((mywhile*)(ram + p))->init(one, two);
	}
	else if (type == DOUNTIL) {
		ram[ramtot++] = 1;
		p = ramtot;
		ramtot += sizeof(dountil);
		((dountil*)(ram + p))->init(one, two);
	}
	else {
		std::cout << "语句类型不符" << std::endl;
		exit(0);
	}
}

inline statement::statement(std::string one, std::string two, std::string three) {
	type = IFELSE;
	ram[ramtot++] = 1;
	p = ramtot;
	ramtot += sizeof(IFELSE);
	((ifelse*)(ram + p))->init(one, two, three);
}


#endif

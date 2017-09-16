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

void strtotoken(std::string str, std::vector<token> &tok);		//将一句字符串解析为token组

class minstatement{
private:
	int line;
	std::vector<token> tok;
public:
	bool judge();		//判断语句返回值是否为真
	void runinit();		//执行声明，并从队列中取值初始化
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
	void init() {		//执行声明，并从队列中取值初始化
		if (type != MINSTATEMENT) {
			std::cerr << "执行声明语句类型不符" << std::endl;
			exit(0);
		}
		ramminstatement[p].runinit();
	}
	int run();
};
statement ramstatement[RAMMAX];
int statementtot;

void strtosta(std::string str, int sta);	//将一段字符串解析为语句组

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
			std::cerr << "判断式不可为空" << std::endl;
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
			std::cerr << "判断式不可为空" << std::endl;
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
			std::cerr << "判断式不可为空" << std::endl;
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

class function{				//INT，REAL，STRING的type必须是0,1,2,None表示空
private:
	function* next[3];
public:
	int type;
	int pnum;		//参数个数
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
	}
	~function() {
		clear();
	}
};
function ramfunction[RAMMAX];
int functiontot;

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
	//根据type不同调用不同to函数，返回数据引用
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
			std::cerr << "函数无法显示" << std::endl;
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
		std::cerr << "语句类型不符" << std::endl;
		exit(0);
	}
}

inline statement::statement(int _line, std::string one, std::string two, std::string three) {
	type = IFELSE;
	p = ifelsetot++;
	ramifelse[p] = ifelse(one, _line, two, three);
}

#endif

#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include "mydata.h"

class variable {
private:
	mydata *temp;
	mydata *head[1000];	//各层头指针
	std::map<name, mydata*>valmap;		//数据查询器
	std::map<name, mydata*>::iterator it;
public:
	int now;		//当前层数
	void newdata(name n, int type = INT) {			//声明新变量
		if (now == 0) {
			std::cout << "无作用域" << std::endl;
			exit(0);
		}
		if (type != INT&&type != REAL&&type != STRING) {
			std::cout << "变量类型错误" << std::endl;
			exit(0);
		}
		it = valmap.find(n);
		if (it == valmap.end()) {
			temp = new mydata(n, now, head[now], type);
			temp->f = NULL;
			valmap[n] = temp;
		}
		else {
			if (it->second->deep >= now) {
				std::cout << "变量重定义" << std::endl;
				exit(0);
			}
			else {
				temp = new mydata(n, now, head[now], type);
				temp->f = it->second;
				valmap[n] = temp;
			}
		}
	}
	void newdata(name n, std::vector<int>a, std::string s, int type = INT) {		//声明新函数
		if (now == 0) {
			std::cout << "无作用域" << std::endl;
			exit(0);
		}
		if (type != INT&&type != REAL&&type != STRING) {
			std::cout << "//变量类型错误" << std::endl;
			exit(0);
		}
		if (now > 1) {
			std::cout << "//此处不可声明函数" << std::endl;
			exit(0);
		}
		temp = find(n);
		if (temp == NULL) {
			temp = new mydata(n, now, head[now], FUNCTION);
			temp->f = NULL;
			valmap[n] = temp;
		}
		else {
			if (temp->type != FUNCTION) {
				std::cout << "重定义，函数与变量不兼容" << std::endl;
				exit(0);
			}
		}
		temp->tofunction().add(a, s, type);
	}
	mydata* find(name n) {		//变量查询
		it = valmap.find(n);
		if (it == valmap.end()) {
			return NULL;
		}
		else {
			return it->second;
		}
	}
	void newfloor() {		//进入新作用域，文件开头调用，每次遇到 { 时调用
		now++;
		head[now] = NULL;
	}
	void deletefloor() {		//退出作用域，文件结束调用，每次遇到 } 时调用
		while (head[now] != NULL) {
			temp = head[now];
			head[now] = temp->next;
			if (temp->f == NULL) {
				valmap.erase(temp->key);
			}
			else {
				valmap[temp->key] = temp->f;
			}
			delete temp;
		}
		now--;
	}
	variable() { now = 0; head[now] = NULL;}
	~variable() {			//清空变量
		while (now) {
			deletefloor();
		}
	}
};


#endif
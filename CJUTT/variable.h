#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include "mydata.h"

class variable {
private:
	mydata *temp;
	mydata *head[1000];	//����ͷָ��
	std::map<name, mydata*>valmap;		//���ݲ�ѯ��
	std::map<name, mydata*>::iterator it;
public:
	int now;		//��ǰ����
	void newdata(name n, int type = INT) {			//�����±���
		if (now == 0) {
			std::cout << "��������" << std::endl;
			exit(0);
		}
		if (type != INT&&type != REAL&&type != STRING) {
			std::cout << "�������ʹ���" << std::endl;
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
				std::cout << "�����ض���" << std::endl;
				exit(0);
			}
			else {
				temp = new mydata(n, now, head[now], type);
				temp->f = it->second;
				valmap[n] = temp;
			}
		}
	}
	void newdata(name n, std::vector<int>a, std::string s, int type = INT) {		//�����º���
		if (now == 0) {
			std::cout << "��������" << std::endl;
			exit(0);
		}
		if (type != INT&&type != REAL&&type != STRING) {
			std::cout << "//�������ʹ���" << std::endl;
			exit(0);
		}
		if (now > 1) {
			std::cout << "//�˴�������������" << std::endl;
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
				std::cout << "�ض��壬���������������" << std::endl;
				exit(0);
			}
		}
		temp->tofunction().add(a, s, type);
	}
	mydata* find(name n) {		//������ѯ
		it = valmap.find(n);
		if (it == valmap.end()) {
			return NULL;
		}
		else {
			return it->second;
		}
	}
	void newfloor() {		//�������������ļ���ͷ���ã�ÿ������ { ʱ����
		now++;
		head[now] = NULL;
	}
	void deletefloor() {		//�˳��������ļ��������ã�ÿ������ } ʱ����
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
	~variable() {			//��ձ���
		while (now) {
			deletefloor();
		}
	}
};


#endif
#ifndef _ALL_H_
#define _ALL_H_

#include <map>
#include <vector>
#include <cmath>
#include <string>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <stack>
#include <queue>
#include <iostream>
#include <sstream>
#include <fstream>

#define None -1

// 基本类型
#define INT 0
#define REAL 1
#define STRING 2
#define FUNCTION 3
#define VARIABLE 4

// 返回类型
#define RINT 5
#define RREAL 6
#define RSTRING 7

// 关键字
#define KINT 8
#define KREAL 9
#define KSTRING 10
#define KIN 11
#define KOUT 12
#define KIF 13
#define KELSE 14
#define KWHILE 15
#define KRETURN 16
#define KDO 52
#define KUNTIL 53
#define KEY 41

// 运算符
#define ADD 17
#define MINUS 18
#define MULTI 19
#define DIV 20
#define MOD 21
#define AND 22
#define OR 23
#define NOT 24
#define EQ 25
#define GT 26
#define LT 27
#define GE 28
#define LE 29
#define NE 30
#define ASSIGN 31
#define STRLINK 42
#define STRDEL 43

// 分隔符
#define SPACE 32
#define TAB 33
#define NEXTLINE 34
#define DOU 35
#define FEN 36
#define YIN 37
#define RBRACKET 38
#define SBRACKET 39
#define CBRACKET 40

#define KUOHAO 44
#define RETURN 45
#define BREAK 46
#define CONTINUE 47

#define MINSTATEMENT 48
#define MYWHILE 49
#define IFELSE 50
#define DOUNTIL 51

const std::string __type__[] = { "INT", "REAL", "STRING", "FUNCTION", "", "", "", "", "", "", "", "逗号", "分号", "关键字", "变量", "", "换行符", "加号或者正号", "减号或者负号", "乘号", "除号", "取模", "逻辑非", "小于号", "小于等于号", "不等于", "大于", "大于等于", "逻辑或", "逻辑与", "等于", "赋值", "括号" };


#endif

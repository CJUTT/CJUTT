//c++
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<queue>
#include<iostream>
#include<cmath>
#include<map>
//#define LJ(x,y) x##y
//#define ZFC(x) #x
#define LL long long
#define LINT __int64
#define LSON l,m,x<<1
#define RSON m+1,r,x<<1|1
#define MEM(a,b) memset(a,b,sizeof(a))
#define FF(i,n) for(int i=0;i<n;i++)
using namespace std;
const double PAI = 4 * atan(1.0);
const int INF = 0x3f3f3f3f;
const int MAXINT = 0x7fffffff;
const LINT mod = 1e9 + 7;
const LINT LINF = 0x3f3f3f3f3f3f3f3f;
const LINT MAXLINT = 0x7fffffffffffffff;
const int MAX = 1e5 + 50;
/******1100000011101110******1101001111101110******1110101011001101******/
int N, M, T;
int ran[MAX];
LINT val[MAX];
int root;
LINT sh[MAX];
int Lowbit(int x) {			//Lowbit(x)：求二进制下整数x最低位1的值
	return x&(-x);
}
void Zj(int x, LINT val) {	//Zj(x, val):x点的值加上val
	while (x < MAX) {
		sh[x] += val;
		x += Lowbit(x);
	}
}
LINT Qjcx(int x) {		//Qjcx(x):求区间[1, x]的和
	LINT sum = 0;
	while (x > 0) {
		sum += sh[x];
		x -= Lowbit(x);
	}
	return sum;
}
struct node {
	int v, next;
}edge[MAX * 2];
int head[MAX], tot;
LINT dis[MAX];
void init() {					//初始化
	tot = 0;
	memset(head, -1, sizeof(head));
}
void add(int u, int v) {		//添加一条双向边
	edge[tot].v = v;
	edge[tot].next = head[u];
	head[u] = tot++;
	edge[tot].v = u;
	edge[tot].next = head[v];
	head[v] = tot++;
}
void Dfs(int u, int f) {
	Zj(ran[u], val[u]);
	dis[u] = Qjcx(ran[u] - 1);
	for (int i = head[u]; i != -1; i = edge[i].next) {
		int v = edge[i].v;
		if (v == f)continue;
		Dfs(v, u);
	}
	dis[u] = Qjcx(ran[u] - 1) - dis[u];
}

int main() {
	//freopen("in.txt", "r", stdin);
	//freopen("out.txt", "w", stdout);
	int n, m, t;
	int i, j, k;
	double p;
	char c;
	int u, v, w;
	while (~scanf("%d", &N)) {
		init();
		for (i = 1; i <= N; i++) {
			scanf("%d%d%d", &u, &ran[i], &val[i]);
			if (u != -1)
				add(i, u);
			else
				root = i;
		}
		MEM(sh, 0);
		Dfs(root, -1);
		for (i = 1; i <= N; i++) {
			printf("%I64d\n", dis[i]);
		}
	}
	return 0;
}
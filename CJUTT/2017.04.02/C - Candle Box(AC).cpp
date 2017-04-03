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
const int MAX = 1e6 + 50;
/******1100000011101110******1101001111101110******1110101011001101******/
int N, M, T;
int d;
int Fun(int x) {
	return ((x + 4)*(x - 3) + (x + 3 - d)*(x - 2 - d)) / 2;
}
int Ef(int l, int r, int v) {
	int da = -1;
	while (l <= r) {
		int m = (l + r) / 2;
		if (Fun(m) <= v) {
			da = m;
			l = m + 1;
		}
		else
			r = m - 1;
	}
	return da;
}
int main() {
	//freopen("in.txt", "r", stdin);
	//freopen("out.txt", "w", stdout);
	int n, m, t;
	int i, j, k;
	double p;
	char c;
	int u, v;
	while (~scanf("%d", &d)) {
		scanf("%d%d", &u, &v);
		k = Ef(0, 100, u + v);
		printf("%d\n", u - (k + 4)*(k - 3) / 2);
	}
	return 0;
}
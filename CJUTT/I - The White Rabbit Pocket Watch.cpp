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
const int MAX = 210 + 50;
/******1100000011101110******1101001111101110******1110101011001101******/
int N, M, T;
int s, e, z;
int a[550][MAX];
struct NODE {
	int v, i;
}b[510*800];
int tot;
int u[MAX], v[MAX];
bool cmp(NODE q, NODE w) {
	return q.v < w.v;
}
void Gsxy() {
	int m = 13;
	int ny[13];
	for (int i = 1; i < m; i++) {
		for (int j = m-1; j >= 1; j--) {
			if (i*j%m == 1)
				ny[i] = j;
		}
	}
	for (int i = 1; i <= M&&i < tot; i++) {
		int j = i;
		while (a[j][i] == 0 && j <= M)
			j++;
		if (j > M)
			continue;
		swap(a[j], a[i]);
		int d = ny[a[i][i]];
		a[i][0] = a[i][0] * d % m;
		for (j = i; j < tot; j++)
			a[i][j] = a[i][j] * d % m;
		for (j = 1; j <= M; j++) {
			if (j == i)
				continue;
			d = a[j][i];
			for (int k = i; k < tot; k++)
				a[j][k] = (a[j][k] - d*a[i][k] % m + m) % m;
			a[j][0] = (a[j][0] - d*a[i][0] % m + m) % m;
		}
	}
}
int dis[210][210];
int main() {
	//freopen("in.txt", "r", stdin);
	//freopen("out.txt", "w", stdout);
	int n, m, t;
	int i, j, k;
	double p;
	char c;
	while (~scanf("%d%d%d%d", &N,&s,&e,&M)) {
		MEM(a, 0);
		tot = 0;
		for (i = 1; i <= M; i++) {
			scanf("%d%d", &a[i][0], &t);
			scanf("%d", &n);
			for (j = 1; j < t; j++) {
				scanf("%d", &m);
				b[tot].i = i;
				b[tot++].v = min(n, m)*(N + 1) + max(n, m);
				n = m;
			}
		}
		sort(b, b + tot, cmp);
		j = 1;
		for (i = 0; i < tot; i++) {
			a[b[i].i][j]++;
			a[b[i].i][j] %= 13;
			u[j] = b[i].v / (N + 1);
			v[j] = b[i].v % (N + 1);
			while (i < tot - 1 && b[i + 1].v == b[i].v) {
				i++;
				a[b[i].i][j]++;
				a[b[i].i][j] %= 13;
			}
			j++;
		}
		tot = j;
		Gsxy();
		MEM(dis, INF);
		for (i = 1; i <= N; i++)
			dis[i][i] = 0;
		for (i = 1; i < tot; i++)
			dis[u[i]][v[i]] = dis[v[i]][u[i]] = a[i][0];
		for (i = 1; i <= N; i++) {
			for (j = 1; j <= N; j++) {
				for (k = 1; k <= N; k++) {
					dis[j][k] = min(dis[j][k], dis[j][i] + dis[i][k]);

				}
			}
		}
		printf("%d\n", dis[s][e]);
	}
	return 0;
}
//c++
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<queue>
#include<iostream>
#include<cmath>
#include<map>
#include<set>
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
struct NODE {
	LINT v;
	int a[40];
}dp[2][MAX];
int a[40];
int z;
int tot[2];
bool Vis(int x) {	//a´ó
	for (int i = 1; i <= N; i++) {
		if (a[i] > dp[z ^ 1][x].a[i])
			return 1;
		else if (a[i] < dp[z ^ 1][x].a[i])
			return 0;
	}
	return 1;
}
int Ef(int l, int r) {
	int da = -1;
	while (l <= r) {
		int m = (l + r) / 2;
		if (Vis(m)) {
			da = m;
			r = m - 1;
		}
		else
			l = m + 1;
	}
	return da;
}
void Dfs(int x, int y, int f) {
	if (x == N) {
		a[x] = y;
		int temp = tot[z ^ 1] - 1;
		dp[z][tot[z]].v = 0;
		int li = 0;
		a[0] = -1;
		for (int i = 1; i <= N; i++) {
			if (a[i] == 0)
				continue;
			while (i < N&&a[i] == a[i + 1])
				i++;
			a[i]--;
			temp = Ef(0, temp);
			dp[z][tot[z]].v += dp[z ^ 1][temp].v*(i - li);
			li = i;
			a[i]++;
		}
		for (int i = 1; i <= N; i++)
			dp[z][tot[z]].a[i] = a[i];
		tot[z]++;
		return;
	}
	for (int i = min(y, f); (N - x + 1)*i >= y; i--) {
		a[x] = i;
		Dfs(x + 1, y - i, i);
	}
}
set<LINT>se;
int main() {
	//freopen("in.txt", "r", stdin);
	//freopen("out.txt", "w", stdout);
	int n, m, t;
	int i, j, k;
	double p;
	char c;
	int u, v;
	while (~scanf("%d%d", &N, &M)) {
		z = 0;
		tot[0] = 1;
		MEM(dp[0][0].a, 0);
		dp[0][0].v = 1;
		MEM(a, 0);
		for (i = 1; i < M; i++) {
			z ^= 1;
			tot[z] = 0;
			Dfs(1, i, INF);
		}
		se.clear();
		for (i = 0; i < tot[z]; i++) {
			se.insert(dp[z][i].v);
		}
		set<LINT>::iterator it;
		for (it = se.begin(); it != se.end(); it++) {
			printf("%I64d\n", *it);
		}
	}
	return 0;
}
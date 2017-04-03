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
const LINT mod = 1e6 + 3;
const LINT LINF = 0x3f3f3f3f3f3f3f3f;
const LINT MAXLINT = 0x7fffffffffffffff;
const int MAX = 1e6 + 50;
/******1100000011101110******1101001111101110******1110101011001101******/
int N, M, T;
const int ACBMAX = 3e4 + 10;
const int ACZFMAX = 26;
char s[30];
LINT dp[30][ACBMAX][2][2][2];
int zz[10];
struct Trie {
	int next[ACBMAX][ACZFMAX], fail[ACBMAX], end[ACBMAX];
	int root, L;
	int newnode() {				//建边
		for (int i = 0; i < ACZFMAX; i++)
			next[L][i] = -1;
		end[L++] = 0;
		return L - 1;
	}
	void init() {				//初始化
		L = 0;
		root = newnode();
	}
	void insert(char buf[]) {			//插入
		int len = strlen(buf);
		int now = root;
		for (int i = 0; i < len; i++) {
			if (next[now][buf[i] - 'a'] == -1)
				next[now][buf[i] - 'a'] = newnode();
			now = next[now][buf[i] - 'a'];
		}
		end[now] = 1;
	}
	void build() {				//构建fail
		queue<int>Q;
		fail[root] = root;
		for (int i = 0; i < ACZFMAX; i++)
			if (next[root][i] == -1)
				next[root][i] = root;
			else {
				fail[next[root][i]] = root;
				Q.push(next[root][i]);
			}
			while (!Q.empty()) {
				int now = Q.front();
				Q.pop();
				for (int i = 0; i < ACZFMAX; i++)
					if (next[now][i] == -1)
						next[now][i] = next[fail[now]][i];
					else {
						fail[next[now][i]] = next[fail[now]][i];
						end[next[now][i]] += end[fail[next[now][i]]];
						Q.push(next[now][i]);
					}
			}
	}
	LINT query() {
		LINT da = 0;
		MEM(dp, 0);
		dp[0][0][0][0][0] = 1;
		for (int i = 0; i <= M; i++) {
			for (int j = 0; j < L; j++) {
				if (end[j] > 0)
					continue;
				if (i >= N) {
					da += dp[i][j][1][1][1];
					da %= mod;
				}
				for (int a = 0; a < 2; a++) {
					for (int b = 0; b < 2; b++) {
						for (int c = 0; c < 2; c++) {
							for (int k = 0; k < ACZFMAX; k++) {
								dp[i + 1][next[j][k]][1][b][c] += dp[i][j][a][b][c];
								dp[i + 1][next[j][k]][1][b][c] %= mod;
								dp[i + 1][next[j][k]][a][1][c] += dp[i][j][a][b][c];
								dp[i + 1][next[j][k]][a][1][c] %= mod;
							}
							for (int k = 0; k < 10; k++) {
								if (k == 0 || k == 1 || k == 3 || k == 5 || k == 7) {
									dp[i + 1][next[j][zz[k]]][a][b][1] += dp[i][j][a][b][c];
									dp[i + 1][next[j][zz[k]]][a][b][1] %= mod;
								}
								else {
									dp[i + 1][0][a][b][1] += dp[i][j][a][b][c];
									dp[i + 1][0][a][b][1] %= mod;
								}
							}
						}
					}
				}
			}
		}
		return da;
	}
}ac;
int main() {
	//freopen("in.txt", "r", stdin);
	//freopen("out.txt", "w", stdout);
	int n, m, t;
	int i, j, k;
	double p;
	char c;
	zz[0] = 'o' - 'a';
	zz[1] = 'i' - 'a';
	zz[3] = 'e' - 'a';
	zz[5] = 's' - 'a';
	zz[7] = 't' - 'a';
	while (~scanf("%d%d", &N,&M)) {
		ac.init();
		scanf("%d", &k);
		for (i = 0; i < k; i++) {
			cin >> s;
			ac.insert(s);
		}
		ac.build();
		printf("%I64d\n", ac.query());
	}
	return 0;
}
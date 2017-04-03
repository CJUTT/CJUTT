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
const int MAX = 1 << 20;
/******1100000011101110******1101001111101110******1110101011001101******/
int N, M, T;
int a[20], b[20];
int ta, tb, tc;
double dp[55][55][55];
double zh[55][55];					////组合数表
void Zh(int x) {
	for (int i = 0; i <= x; i++)
		zh[i][0] = zh[i][i] = 1;
	for (int i = 2; i <= x; i++)
		for (int j = 1; j < i; j++)
			zh[i][j] = zh[i - 1][j - 1] + zh[i - 1][j];
}
int main() {
	//freopen("in.txt", "r", stdin);
	//freopen("out.txt", "w", stdout);
	int n, m, t;
	int i, j, k;
	double p;
	char c;
	int u, v;
	Zh(51);
	while (~scanf("%d%d%d", &N, &M, &T)) {
		for (i = 0; i < T; i++)
			scanf("%d", &a[i]);
		for (i = 0; i < T; i++)
			scanf("%d", &b[i]);
		sort(a, a + T);
		sort(b, b + T);
		n = m = 0;
		a[T] = b[T] = INF;
		ta = tb = tc = 0;
		for (i = 0; i < 20; i++) {
			if (n == m&&n == T)
				break;
			if (a[n] == b[m]) {
				n++;
				m++;
				tc++;
			}
			else if (a[n] < b[m]) {
				ta++;
				n++;
			}
			else {
				tb++;
				m++;
			}
		}
		MEM(dp, 0);
		for (i = 0; i <= ta; i++) {
			for (j = 0; j <= tb; j++) {
				for (k = 0; k <= tc; k++) {
					if ((i == 0 || j == 0) && (k == 0))
						continue;
					for (int ti = i; ti >= 0; ti--) {
						for (int tj = j; tj >= 0; tj--) {
							for (int tk = k; tk >= 0; tk--) {
								m = ti + tj + tk;
								if (m > M || m == 0)
									continue;
								dp[i][j][k] += dp[i - ti][j - tj][k - tk] * zh[i][ti] * zh[j][tj] * zh[k][tk] * zh[N - i - j - k][M - m] / zh[N][M];
							}
						}
					}
					if (N - i - j - k >= M)
						dp[i][j][k] = (dp[i][j][k] + 1)*zh[N][M] / (zh[N][M] - zh[N - i - j - k][M]);
					else
						dp[i][j][k] += 1;
				}
			}
		}
		printf("%lf\n", dp[ta][tb][tc]);
	}
	return 0;
}
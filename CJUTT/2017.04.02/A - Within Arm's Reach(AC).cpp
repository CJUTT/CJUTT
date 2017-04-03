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
double a[30];
const double eps = 1e-8;
int sgn(double x) {
	if (x <= eps) {
		if (x >= -eps)
			return 0;
		return -1;
	}
	return 1;
}
struct DIA {
	double x, y;
	bool operator ==(const DIA &q) {
		return !(sgn(x - q.x) || sgn(y - q.y));
	}
}d[30], en;
double Dis(DIA q, DIA w) {		//(x1,y1),(x2,y1)º‰æ‡¿Î
	return sqrt((q.x - w.x)*(q.x - w.x) + (q.y - w.y)*(q.y - w.y));
}
double p(double x) {
	return x*x;
}
double l[30], r[30], dis[30], jj[30];
int main() {
	//freopen("in.txt", "r", stdin);
	//freopen("out.txt", "w", stdout);
	int n, m, t;
	int i, j, k;
	char c;
	double u, v, w;
	while (~scanf("%d", &N)) {
		for (i = 1; i <= N; i++)
			scanf("%lf", &a[i]);
		scanf("%lf%lf", &en.x, &en.y);
		d[0].x = d[0].y = 0;
		dis[0] = Dis(d[0], en);
		l[0] = r[0] = dis[0];
		for (i = 1; i <= N; i++) {
			l[i] = l[i - 1] + a[i];
			if (r[i - 1] >= a[i])
				r[i] = r[i - 1] - a[i];
			else {
				if (l[i - 1] >= a[i])
					r[i] = 0;
				else
					r[i] = a[i] - l[i - 1];
			}
		}
		dis[N] = r[N];
		for (i = N - 1; i > 0; i--) {
			u = dis[i + 1] + a[i + 1];
			if (dis[i + 1] >= a[i + 1])
				v = dis[i + 1] - a[i + 1];
			else
				v = a[i + 1] - dis[i + 1];
			dis[i] = (min(l[i], u) + max(r[i], v)) / 2;
		}
		if (!sgn(en.x)) {
			if (en.y >= 0)
				jj[0] = -PAI / 2;
			else
				jj[0] = PAI / 2;
		}
		else if (en.x > 0)
			jj[0] = atan(en.y / en.x) + PAI;
		else
			jj[0] = atan(en.y / en.x);
		for (i = 1; i <= N; i++) {
			if (!(sgn(dis[i]) && sgn(dis[i - 1])))
				jj[i] = 0;
			else {
				u = (p(dis[i]) + p(dis[i - 1]) - p(a[i])) / (2.0*dis[i] * dis[i - 1]);
				if (u > 1)
					u = 1;
				else if (u < -1)
					u = -1;
				jj[i] = jj[i - 1] + acos(u);
			}
			d[i].x = en.x + dis[i] * cos(jj[i]);
			d[i].y = en.y + dis[i] * sin(jj[i]);
			printf("%.3lf %.3lf\n", d[i].x, d[i].y);
			//printf("%.3lf\n", Dis(d[i], d[i - 1]));
		}
	}
	return 0;;;
}
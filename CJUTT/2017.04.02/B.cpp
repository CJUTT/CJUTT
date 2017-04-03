#include<iostream>
#include<algorithm>
#include<cmath>
#define maria p[0]
using namespace std;
const int maxn = 1e5 + 7;
const double eps = 1e-7;

int n, top_pos, low_pos;

struct phone {
	double x1, x2;
	double dx1, dx2, _0;
}p[maxn];

int cmp1(phone a, phone b) {    // sort by x1
	return a.x1 > b.x1;
}

int cmp2(phone a, phone b) {    // sort by zero point
	return a._0 < b._0;
}

void init() {
	top_pos = maxn;
	low_pos = 1;
	for (int i = 1; i < n; i++) {
		p[i].dx1 = p[i].x1 - p[0].x1;
		p[i].dx2 = p[i].x2 - p[0].x2;
		if (abs(p[i].dx1 - 0) <= eps) {
			p[i]._0 = -1;
		}
		else
			p[i]._0 = (-p[i].dx2) / p[i].dx1;
	}
}

int main() {
	while (cin >> n) {
		for (int i = 0; i < n; i++) {
			cin >> p[i].x1 >> p[i].x2;
		}
		init();
		// w2 = 0
		sort(p + 1, p + n, cmp1);
		int pos = 1;
		while (pos < n && p[pos].dx1 - 0 > eps) pos++;
		if (pos < top_pos) top_pos = pos;
		if (pos > low_pos) low_pos = pos;
		while (pos < n && abs(p[pos].dx1 - 0) <= eps) pos++;
		if (pos < top_pos) top_pos = pos;
		if (pos > low_pos) low_pos = pos;
		// w2 != 0
		sort(p + 1, p + n, cmp2);
		pos = 1;
		int big = 0, small = 0, equal = 0, flag = 1;
		for (int i = 1; i < n; i++) {
			if (0 - p[i]._0 > eps || abs(0 - p[i]._0) < eps) {
				if (p[i].dx1 - 0 > eps) {
					big++;
				}
				else if (abs(p[i].dx1 - 0) <= eps) {
					equal++;
				}
				else {
					small++;
				}
				flag = i + 1;
			}
			else {
				if (p[i].dx1 - 0 > eps) {
					small++;
				}
				else if (abs(p[i].dx1 - 0) <= eps) {
					return 1;
				}
				else {
					big++;
				}
			}
		}
		if (big + 1 < top_pos) top_pos = big + 1;;
		if (big + equal + 1 > low_pos) low_pos = big + equal + 1;

		for (int i = flag; i < n; i++) {
			if (p[i].dx1 - 0 > eps) {
				small--;
				equal++;
				if (big + 1 < top_pos) top_pos = big + 1;;
				if (big + equal + 1 > low_pos) low_pos = big + equal + 1;
				equal--;
				big++;
				if (big + 1 < top_pos) top_pos = big + 1;;
				if (big + equal + 1 > low_pos) low_pos = big + equal + 1;
			}
			else if (abs(p[i].dx1 - 0) <= eps) {
				return 1;
			}
			else {
				big--;
				equal++;
				if (big + 1 < top_pos) top_pos = big + 1;;
				if (big + equal + 1 > low_pos) low_pos = big + equal + 1;
				equal--;
				small++;
				if (big + 1 < top_pos) top_pos = big + 1;;
				if (big + equal + 1 > low_pos) low_pos = big + equal + 1;
			}
		}

		cout << top_pos << " " << low_pos << endl;
	}
	return 0;
}
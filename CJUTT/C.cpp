#include<bits/stdc++.h>
using namespace std;

const int maxn = 2e5 + 7;
const int maxm = 1e5 + 7;

int parent[maxn], _rank[maxn], _size[maxn];
int find(int x)
{
	if (parent[x] == x)	return x;
	else return parent[x] = find(parent[x]);   //返回的是find(parent[x])！！！    
}
int merge(int x, int y)
{
	x = find(x), y = find(y);    //寻找x,y的根 
	if (x == y)	return _size[x];	//如果是一个根不用操作，否则根据秩的大小进行合并操作	
	if (_rank[x] >= _rank[y])
	{
		parent[y] = x;
		_size[x] += _size[y];
		if (_rank[x] == _rank[y]) _rank[x]++;
		return _size[x];
	}
	else
	{
		parent[x] = y;
		_size[y] += _size[x];
		return _size[y];
	}
}

struct ARMS {
	int x, y;
	int l, r;
}arms[maxm];

int cmp(ARMS a, ARMS b) {
	return a.x < b.x;
}

int ans[maxn];

int main() {
	freopen("in.txt", "r", stdin);
	int n, m;
	while (scanf("%d %d", &n, &m) != EOF) {
		for (int i = 0; i <= n; i++) {
			ans[i] = 1;
			_size[i] = 1;
			parent[i] = i;
			_rank[i] = 0;
		}
		for (int i = 0; i < m; i++) {
			scanf("%d %d", &arms[i].x, &arms[i].y);
			arms[i].l = arms[i].r = i + 1;
		}
		sort(arms, arms + m, cmp);
		for (int i = 0; i < m; i++) {
			int temp = merge(arms[i].y, arms[i].y + 1);
			ans[arms[i].y] = ans[arms[i].y + 1] = temp;
		}
		for (int i = 1; i < n; i++) {
			printf("%d ", ans[i]);
		}
		printf("%d\n", ans[n]);
	}
	return 0;
}
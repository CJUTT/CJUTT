#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <set>
#include <vector>
#include <map>
#include <iostream>
#define maxn 100005
using namespace std;
struct node {
	int x, y, z;
	bool operator < (const node & s) const
	{
		if (x == s.x)
		{

			if (y == s.y)
				return z<s.z;
			return y<s.y;
		}
		return x<s.x;
	}
};
struct point {
	int x, y;
	bool operator < (const point & s) const
	{
		if (x == s.x)
			return y<s.y;
		return x<s.x;
	}

};
map<node, int>mymap;
map<point, int>mymap1;
set<int>ve[maxn];
set<int>ve1[maxn];
int vis[maxn];
int vis1[maxn];
int flag1 = 0;
int flag = 0;
void DFS(int now, int fa)
{
	set<int>::iterator iter = ve[now].begin();
	for (; iter != ve[now].end(); iter++)
	{

		int next = *iter;
		if (next == fa)
			continue;
		if (vis[next])
		{
			flag = 1;
			return;
		}
		else {
			vis[next] = 1;
			DFS(next, now);
		}
	}
}
void DFS1(int now, int fa)
{
	set<int>::iterator iter = ve1[now].begin();
	for (; iter != ve1[now].end(); iter++)
	{

		int next = *iter;
		if (next == now)
			continue;
		if (next == fa)
			continue;
		if (vis1[next])
		{
			flag1 = 1;
			return;
		}
		else {
			vis1[next] = 1;
			DFS1(next, now);
		}
	}
}
int main()
{
	int k;

	while (scanf("%d", &k) != EOF)
	{
		int tmp = 1;
		int tmp1 = 1;
		for (int i = 1; i <= k; i++)
		{
			node now1, now2;
			scanf("%d%d%d", &now1.x, &now1.y, &now1.z);
			if (mymap.find(now1) != mymap.end())
			{

			}
			else mymap[now1] = tmp++;
			scanf("%d%d%d", &now2.x, &now2.y, &now2.z);
			if (mymap.find(now2) != mymap.end())
			{

			}
			else mymap[now2] = tmp++;
			ve[mymap[now2]].insert(mymap[now1]);
			ve[mymap[now1]].insert(mymap[now2]);

			point now11, now12;
			now11.x = now1.x;
			now11.y = now1.y;
			if (mymap1.find(now11) != mymap1.end())
			{

			}
			else mymap1[now11] = tmp1++;
			// scanf("%d%d%d",&now2.x,&now2.y,&now2.z);
			now12.x = now2.x;
			now12.y = now2.y;

			if (mymap1.find(now12) != mymap1.end())
			{

			}
			else 
				mymap1[now12] = tmp1++;
			ve1[mymap1[now12]].insert(mymap1[now11]);
			ve1[mymap1[now11]].insert(mymap1[now12]);
		}
		flag1 = 0;
		flag = 0;
		for (int i = 1; i<tmp; i++)
		{
			if (vis[i] == 0)
			{
				vis[i] = 1;
				DFS(i, 0);
			}
		}
		for (int i = 1; i<tmp1; i++)
		{
			if (vis1[i] == 0)
			{

				vis1[i] = 1;
				DFS1(i, 0);
			}
		}
		// cout<<flag1<<flag<<endl;
		if (flag == 0)
			cout << "No true closed chains" << endl;
		else
			cout << "True closed chains" << endl;
		if (flag1 == 1)
			cout << "Floor closed chains" << endl;
		else
			cout << "No floor closed chains" << endl;
	}
}
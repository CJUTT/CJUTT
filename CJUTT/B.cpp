#include<bits/stdc++.h>
using namespace std;

int table[10][10];
int check[100000];

bool fun(int abcd) {
	int a = abcd / 1000;
	int b = (abcd % 1000) / 100;
	int c = (abcd % 100) / 10;
	int d = abcd % 10;
	int e = table[table[table[table[0][a]][b]][c]][d];
	int temp;
	for (int i = 0; i < 10; i++) {
		if (i == a) continue;
		temp = abcd - a * 1000 + i * 1000;
		if (check[temp * 10 + e] == 0) 
			return true;
	}
	for (int i = 0; i < 10; i++) {
		if (i == b) continue;
		temp = abcd - b * 100 + i * 100;
		if (check[temp * 10 + e] == 0)
			return true;
	}
	for (int i = 0; i < 10; i++) {
		if (i == c) continue;
		temp = abcd - c * 10 + i * 10;
		if (check[temp * 10 + e] == 0)
			return true;
	}
	for (int i = 0; i < 10; i++) {
		if (i == d) continue;
		temp = abcd - d + i;
		if (check[temp * 10 + e] == 0)
			return true;
	}
	for (int i = 0; i < 10; i++) {
		if (i == e) continue;
		temp = abcd;
		if (check[temp * 10 + i] == 0)
			return true;
	}
	temp = b * 1000 + a * 100 + c * 10 + d;
	if (a != b && check[temp * 10 + e] == 0)
		return true;
	temp = a * 1000 + c * 100 + b * 10 + d;
	if (b != c && check[temp * 10 + e] == 0)
		return true;
	temp = a * 1000 + b * 100 + d * 10 + c;
	if (c != d && check[temp * 10 + e] == 0)
		return true;
	temp = a * 1000 + b * 100 + c * 10 + e;
	if (d != e && check[temp * 10 + d] == 0)
		return true;
	return false;
}

int main() {
	/*freopen("in.txt", "r", stdin);
	int t = 4;
	while (t--) {*/
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				scanf("%d", &table[i][j]);
			}
		}
		// get every abcde
		for (int a = 0; a < 10; a++) {
			for (int b = 0; b < 10; b++) {
				for (int c = 0; c < 10; c++) {
					for (int d = 0; d < 10; d++) {
						for (int e = 0; e < 10; e++) {
							check[a * 10000 + b * 1000 + c * 100 + d * 10 + e] = table[table[table[table[table[0][a]][b]][c]][d]][e];
						}
					}
				}
			}
		}
		int ans = 0;
		for (int i = 0; i < 10000; i++) {
			if (fun(i)) {
				ans++;
			}
		}
		printf("%d\n", ans);
	
	return 0;
}
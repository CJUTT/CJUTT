#include<bits/stdc++.h>
using namespace std;

set<string> s;

int main() {
	//freopen("in.txt", "r", stdin);
	string s1, s2;
	while (cin >> s1 >> s2) {
		s.clear();
		for (int i = 0; i < s1.size(); i++) {
			int a[26];
			memset(a, 0, sizeof(a));
			for (int j = i; j < s1.size(); j++) {
				a[s1[j] - 'a']++;
				string str = "";
				for (int k = 0; k < 26; k++) {
					int q = a[k] / 1000;
					int w = (a[k] % 1000) / 100;
					int e = (a[k] % 100) / 10;
					int r = a[k] % 10;
					str.append(1, q + '0');
					str.append(1, w + '0');
					str.append(1, e + '0');
					str.append(1, r + '0');
				}
				s.insert(str);
			}
		}
		int ans = 0;
		for (int i = 0; i < s2.size(); i++) {
			int a[26];
			memset(a, 0, sizeof(a));
			for (int j = i; j < s2.size(); j++) {
				a[s2[j] - 'a']++;
				string str = "";
				for (int k = 0; k < 26; k++) {
					int q = a[k] / 1000;
					int w = (a[k] % 1000) / 100;
					int e = (a[k] % 100) / 10;
					int r = a[k] % 10;
					str.append(1, q + '0');
					str.append(1, w + '0');
					str.append(1, e + '0');
					str.append(1, r + '0');
				}
				if (s.find(str) != s.end())
				{
					ans = max(ans, j - i + 1);
				}
			}
		}
		cout << ans << endl;
	}
	return 0;
}
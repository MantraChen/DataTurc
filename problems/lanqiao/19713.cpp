#include <iostream>
#include <algorithm>
using namespace std;

#define maxn 1000001
long long a[maxn];
long long all[maxn];
int asize;

int main() {
	int n;
	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> a[i];
	}
	asize = 1;
	for (int i = 0; i < n; ++i) {
		long long s = 0;
		for (int j = i; j < n; ++j) {
			s += a[j];
			all[asize++] = s;
		}
	}
	sort(all + 1, all + asize);
	long long ans = 1000000000;
	for (int i = 2; i < asize; ++i) {
		ans = min(ans, all[i] - all[i - 1]);
	}
	cout << ans << endl;
	return 0;
}
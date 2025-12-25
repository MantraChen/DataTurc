#include <bits/stdc++.h>
using namespace std;
using i128 = __int128_t;
static const long long MOD = 998244353;

// a^2 mod
long long sq(long long x) { return (i128)x * x % MOD; }

// sum k   = n(n+1)/2
long long sum1(long long n) {
	return (i128)n * (n + 1) / 2 % MOD;
}

// sum k^2 = n(n+1)(2n+1)/6
long long sum2(long long n) {
	return (i128)n * (n + 1) % MOD * (2 * n + 1) % MOD * 166374059 % MOD;
	// 166374059 = inv(6) mod 998244353
}

long long isqrt(long long x) {
	long long r = sqrtl((long double)x);
	while ((r + 1) * (r + 1) <= x) r++;
	while (r * r > x) r--;
	return r;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int T;
	cin >> T;
	while (T--) {
		long long X;
		cin >> X;

		// KA: k^2 <= X
		long long KA = isqrt(X);

		// KB: k^2 + k <= X  -> k = floor( (-1 + sqrt(1+4X))/2 )
		long long KB = (isqrt(1 + 4 * (__int128)X) - 1) / 2;

		// KC: k^2 + 2k <= X -> k = floor( sqrt(X+1) - 1 )
		long long KC = isqrt(X + 1) - 1;

		long long ans = 0;

		// S0 = sum k^2 for k ¡Ü KA
		ans = (ans + sum2(KA)) % MOD;

		// S1 = sum (k^2 + k) = sum k^2 + sum k
		ans = (ans + sum2(KB) + sum1(KB)) % MOD;

		// S2 = sum (k^2 + 2k) = sum k^2 + 2 sum k
		ans = (ans + sum2(KC) + 2 * sum1(KC) % MOD) % MOD;

		cout << ans << "\n";
	}
}

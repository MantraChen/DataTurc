//https://leetcode.cn/problems/cnHoX6/description/

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

class Solution {
	int n;
	long long P;
	int Target;
	vector<int> gems_val;
	vector<int> gems_len;
	vector<long long> p10;

	vector<vector<int>> dp;
	vector<int> mask_total_len;

	const int CACHE_LIMIT = 6;
	bool singular;
	long long inv10;
	long long power(long long base, long long exp) {
		long long res = 1;
		base %= P;
		while (exp > 0) {
			if (exp % 2 == 1)
				res = (res * base) % P;
			base = (base * base) % P;
			exp /= 2;
		}
		return res;
	}
	long long modInverse(long long n) { return power(n, P - 2); }

	long long countWays(int mask, int target_rem) {
		if (__builtin_popcount(mask) <= CACHE_LIMIT) {
			const auto& v = dp[mask];
			auto range = equal_range(v.begin(), v.end(), target_rem);
			return distance(range.first, range.second);
		}

		long long total = 0;
		for (int s = (mask - 1) & mask; s > 0; s = (s - 1) & mask) {
			int other = mask ^ s;

			long long Ls = mask_total_len[s];
			long long Lo = mask_total_len[other];

			long long C = (p10[Ls + Lo + 1] + 9) % P;
			long long M_s = p10[Lo + 1];

			int small_mask, large_mask;
			bool smallIsLeft;

			if (__builtin_popcount(s) <= __builtin_popcount(other)) {
				small_mask = s;
				large_mask = other;
				smallIsLeft = true;
			}
			else {
				small_mask = other;
				large_mask = s;
				smallIsLeft = false;
			}

			const auto& small_vec = dp[small_mask];

			for (int v_small : small_vec) {
				long long rhs;
				long long needed_v_large;

				if (smallIsLeft) {
					long long current = (C + (long long)v_small * M_s) % P;
					rhs = (target_rem - current + P) % P;

					if (singular) {
						if (rhs == 0)
							total += countAllStructure(large_mask);
					}
					else {
						needed_v_large = (rhs * inv10) % P;
						total += countWays(large_mask, needed_v_large);
					}
				}
				else {
					long long M_large = p10[mask_total_len[small_mask] + 1];

					long long term_r = (long long)v_small * 10 % P;
					long long current = (C + term_r) % P;
					rhs = (target_rem - current + P) % P;

					if (singular) {
						if (rhs == 0)
							total += countAllStructure(large_mask);
					}
					else {
						long long invM = modInverse(M_large);
						needed_v_large = (rhs * invM) % P;
						total += countWays(large_mask, needed_v_large);
					}
				}
			}
		}
		return total;
	}
	long long countAllStructure(int mask) {
		if (__builtin_popcount(mask) <= CACHE_LIMIT) {
			return dp[mask].size();
		}
		long long total = 0;
		for (int s = (mask - 1) & mask; s > 0; s = (s - 1) & mask) {
			total += countAllStructure(s) * countAllStructure(mask ^ s);
		}
		return total;
	}

public:
	int treeOfInfiniteSouls(vector<int>& gem, int p, int target) {
		n = gem.size();
		P = p;
		Target = target;

		gems_val.resize(n);
		gems_len.resize(n);

		int total_digits = 0;
		for (int i = 0; i < n; ++i) {
			gems_val[i] = gem[i] % P;
			string s = to_string(gem[i]);
			gems_len[i] = s.length();
			total_digits += gems_len[i];
		}

		int max_len = total_digits + 4 * n + 20;
		p10.resize(max_len + 1);
		p10[0] = 1;
		for (int i = 1; i <= max_len; ++i) {
			p10[i] = (p10[i - 1] * 10) % P;
		}

		if (P == 2 || P == 5) {
			singular = true;
			inv10 = 0;
		}
		else {
			singular = false;
			inv10 = modInverse(10);
		}

		int limit = 1 << n;
		mask_total_len.assign(limit, 0);
		dp.assign(limit, {});
		for (int mask = 1; mask < limit; ++mask) {
			int k = 0;
			int digits = 0;
			for (int i = 0; i < n; ++i) {
				if ((mask >> i) & 1) {
					k++;
					digits += gems_len[i];
				}
			}
			if (k > 0) {
				mask_total_len[mask] = digits + 4 * k - 2;
			}
		}
		vector<vector<int>> masks_by_pc(n + 1);
		for (int m = 1; m < limit; ++m) {
			masks_by_pc[__builtin_popcount(m)].push_back(m);
		}

		int limit_k = min(n, CACHE_LIMIT);
		for (int k = 1; k <= limit_k; ++k) {
			for (int mask : masks_by_pc[k]) {
				if (k == 1) {
					int idx = __builtin_ctz(mask);
					long long term1 = p10[gems_len[idx] + 1];
					long long term2 = (long long)gems_val[idx] * 10 % P;
					long long val = (term1 + term2 + 9) % P;
					dp[mask].push_back(val);
				}
				else {
					for (int s = (mask - 1) & mask; s > 0; s = (s - 1) & mask) {
						int other = mask ^ s;

						long long Ls = mask_total_len[s];
						long long Lo = mask_total_len[other];

						long long C = (p10[Ls + Lo + 1] + 9) % P;
						long long M_s = p10[Lo + 1];

						const auto& left_vals = dp[s];
						const auto& right_vals = dp[other];

						for (int v_l : left_vals) {
							long long base = (C + (long long)v_l * M_s) % P;
							for (int v_r : right_vals) {
								long long res =
									(base + (long long)v_r * 10) % P;
								dp[mask].push_back(res);
							}
						}
					}
					sort(dp[mask].begin(), dp[mask].end());
				}
			}
		}
		int full_mask = (1 << n) - 1;
		if (n <= CACHE_LIMIT) {
			const auto& res = dp[full_mask];
			auto range = equal_range(res.begin(), res.end(), Target);
			return distance(range.first, range.second);
		}
		else {
			return countWays(full_mask, Target);
		}
	}
};
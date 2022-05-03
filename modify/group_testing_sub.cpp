#include <iostream>
#include <cassert>
#include <iomanip>
#include <math.h>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
using namespace std;

const double e = 2.71828182846;

long long C(int n, int m) {
	long long res = 1;
	for (int i = m + 1; i <= n; ++i)
		res *= i;
	for (int i = 1; i <= n - m; ++i)
		res /= i;
	return res;
}

auto SEED = chrono::steady_clock::now().time_since_epoch().count();
mt19937 rng(SEED);

int getRand(int l, int r) { // get a random number in range [l, r]
	return uniform_int_distribution<int>(l, r)(rng);
}

int main () {
	freopen("result_1000.txt", "w", stdout);
	for (int n = 3; n <= 11; ++n) for (int k = 1; k + 1 <= n && k <= 3; ++k) for (int l = 1; l + k <= n; ++l) {
    double tt = clock();
	// calculate
	long long E = C(n, k + 1) * (k + 1);
	long long d = (k + 1) * (C(n, k + 1) - (n - k - 1 >= k + 1 ? C(n - k - 1, k + 1) : 0)) - 1;
	vector <int> possible_mask;
	for (int s = 0; s < 1 << n; ++s) {
		if (__builtin_popcount(s) >= l)
			possible_mask.push_back(s);
	}
	double pp = 0, p = 1;
	for (int s : possible_mask) {
		if ((s & ((1 << k + 1) - 1)) == (1 << k))
			pp += 1;
	}
	pp /= int(possible_mask.size());
	pp = 1.0 - pp;

	// find m
	double now = min(4.0 * d, e * (d + 1));
	int m;
	for (m = 1; ; ++m) {
		now *= pp, p *= pp;
		if (now <= 1)
			break;
	}

	cout << "n = " << n << ", k = " << k << ", l = " << l << ", m = " << m << endl;
	cout << "Expect Resample Times = " << E * p / (1.0 - p) << endl;

	// Algorithm Start: 

	int real = 0;

	// Find all subset with size = t
	vector <vector <int>> subsets;
	subsets.push_back({});
	for (int i = 0; i < n; ++i) {
		vector <vector <int>> nxt = subsets;
		for (vector <int> &a : subsets) {
			a.push_back(i);
			nxt.push_back(a);
		}
		subsets = nxt, nxt.clear();
		for (vector <int> &a : subsets) {
			if (a.size() <= k + 1 && a.size() + n - i - 1 >= k + 1)
				nxt.push_back(a);
		}
		subsets = nxt;
	}

	for (int t = 0; t < 1000; ++t) {
		// Get a Random Table
		vector <int> ans(m);
		for (int i = 0; i < m; ++i)
			ans[i] = possible_mask[getRand(0, int(possible_mask.size()) - 1)];

		while (true) {
			vector <int> bad_id;
			for (int i = 0; i < subsets.size(); ++i) {
				int found = 0;
				for (int row = 0; row < m; ++row) {
					int num = 0, cnt = 0;
					for (int j = 0; j <= k; ++j) {
						if (ans[row] >> subsets[i][j] & 1)
							num |= 1 << j, cnt++;
					}
					if (cnt == 1)
						found |= num;
				}
				for (int j = 0; j <= k; ++j) if (!(found >> j & 1))
					bad_id.push_back(i);
			}
			if (bad_id.empty())
				break;
			real++;
			int id = bad_id[getRand(0, bad_id.size() - 1)];
			vector <bool> in_current_subset(n, false);
			for (int j : subsets[id])
				in_current_subset[j] = true;
			for (int row = 0; row < m; ++row) {
				vector <int> nxt_mask;
				for (int s : possible_mask) {
					bool ok = true;
					for (int i = 0; i < n; ++i) if (!in_current_subset[i])
						ok &= (s >> i & 1) == (ans[row] >> i & 1);
					if (ok)
						nxt_mask.push_back(s);
				}
				assert(!nxt_mask.empty());
				int nxtid = getRand(0, nxt_mask.size() - 1);
				ans[row] = nxt_mask[nxtid];
			}
		}
	}
	
	cout << "Real = " << (double)real / 1000 << '\n';
	cout << "Time = " << (clock() - tt) / 1000 << "s\n";
	}
}
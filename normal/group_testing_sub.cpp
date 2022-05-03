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
	for (int n = 18; n <= 20; ++n) for (int k = 1; k + 1 <= n; ++k) {
    string file_name = "result_n=" + to_string(n) + ",k=" + to_string(k) + ".txt";
    freopen(file_name.c_str(), "w", stdout);
	double tt = clock();
	// calculate
	long long E = C(n, k + 1) * (k + 1);
	long long d = (k + 1) * (C(n, k + 1) - (n - k - 1 >= k + 1 ? C(n - k - 1, k + 1) : 0)) - 1;
	double pp = 1.0 - pow(0.5, k + 1), p = 1;

	// find m
	double now = min(4.0 * d, e * (d + 1));
	int m;
	for (m = 1; ; ++m) {
		now *= pp, p *= pp;
		if (now <= 1)
			break;
	}

	cout << "n = " << n << ", k = " << k << ", m = " << m << endl;
	cout << "Expect Resample Times = " << E * p / (1.0 - p) << endl;

	// Algorithm Start: 

	// Get a Random Table
	vector <int> ans(m);
	for (int i = 0; i < m; ++i)
		ans[i] = getRand(0, (1 << n) - 1);

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

	int real = 0;

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
		for (int row = 0; row < m; ++row) {
			for (int j : subsets[id]) {
				if (ans[row] >> j & 1)
					ans[row] ^= 1 << j;
				if (getRand(0, 1))
					ans[row] |= 1 << j;
			}
		}
	}
	cout << "Real = " << real << '\n';
	cout << "Result:\n";
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			cout << (ans[i] >> j & 1);
		}
		cout << endl;
	}
	cout << "Time = " << (clock() - tt) / 1000 << "s\n";}
}
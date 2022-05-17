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
    freopen("B.txt", "w", stdout);
	for (int k = 2; k <= 15; ++k) for (int n = max(k + 1, 9); n <= 16; ++n) {
	double tt = clock();
	// calculate
	long long E = C(n, k + 1) * (k + 1);
	long long d = (k + 1) * (C(n, k + 1) - (n - k - 1 >= k + 1 ? C(n - k - 1, k + 1) : 0)) - 1;
	double pp = 1.0 - 1.0 / (k + 1) * pow(1.0 * k / (k + 1), k), p = 1;

	// find m
	double now = min(4.0 * d, e * (d + 1));
	int m;
	for (m = 1; ; ++m) {
		now *= pp, p *= pp;
		if (now <= 1)
			break;
	}
	cout << m << " \n"[n == 16];}
}
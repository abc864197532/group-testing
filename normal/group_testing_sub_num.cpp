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
    freopen("A.txt", "w", stdout);
	for (int n = 10; n <= 18; ++n) for (int k = 1; k + 1 <= n; ++k) {
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
	cout << m << " \n"[k + 1 == n];}
}
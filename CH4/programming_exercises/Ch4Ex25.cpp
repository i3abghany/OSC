#include <omp.h>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

int main()
{
	int N; std::cin >> N;
	int res = 0;

	std::uniform_real_distribution<double> urd(-1, 1);
	std::default_random_engine re;
	std::vector<std::pair<double, double>> pts(N);

	auto t1 = std::chrono::high_resolution_clock::now();
#pragma omp parallel for
	for (int i = 0; i < N; i++) {
		double x = urd(re);
		double y = urd(re);
		
		pts[i] = { x, y };
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	// Huge speedup with OpenMP. I think the gap between the Java and the C++ code
	// is that random number generation in C++ is (maybe) DREADFULLY slower?
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << '\n';

	for (int i = 0; i < N; i++) {
		double x = pts[i].first;
		double y = pts[i].second;
		if (sqrt(x * x + y * y) <= 1.0)
			res++;
	}

	std::cout << 4 * ((double)res / N);
	return 0;
}
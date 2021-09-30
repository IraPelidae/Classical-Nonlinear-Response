#pragma once
#include <random>

class random_wrapper {
public:

    random_wrapper();

    double gaussian(double sigma);

    double uniform(double a, double b);

private:

    std::random_device rand_dev;
    std::mt19937 generator;

};
//Numerical Recipes

//struct Ranq1 {
//	unsigned long long int v;
//	Ranq1(unsigned long long int j) : v(4101842887655102017LL) {
//		v ^= j;
//		v = int64();
//	}
//	inline unsigned long long int int64() {
//		v ^= v >> 21; v ^= v << 35; v ^= v >> 4;
//		return v * 2685821657736338717LL;
//	}
//	inline double doub() { return 5.42101086242752217E-20 * int64(); }
//	inline unsigned int int32() { return (unsigned int)int64(); }
//};
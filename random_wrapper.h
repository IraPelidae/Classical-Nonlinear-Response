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
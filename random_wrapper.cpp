#include "random_wrapper.h"

random_wrapper::random_wrapper() : generator(rand_dev()) {}

double random_wrapper::gaussian(double sigma) {

    std::normal_distribution<double> dis(0, sigma);
    return dis(generator);

    }

double random_wrapper::uniform(double a, double b) {

std::uniform_real_distribution<double> dis(a, b);
return dis(generator);

}


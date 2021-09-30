#pragma once
#include <vector>
#include <boost/histogram.hpp> // make_histogram, regular, weight, indexed

using namespace boost::histogram;

std::vector<double> rho_dist(std::vector<double> x);

std::vector<double> cell_velocity_dist(std::vector<double> v, std::vector<double> x);

std::vector<std::vector<double>> rho_j_dist(std::vector<double> x, std::vector<double> v);

auto averaged_rho_dist(std::vector<std::vector<double>> x_matrix);



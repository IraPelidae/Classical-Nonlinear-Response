#pragma once
#include <vector>
#include <boost/histogram.hpp> // make_histogram, regular, weight, indexed
#include <list>

using namespace boost::histogram;

std::list<double> init_bins(int cell_no);

std::vector<double> rho_dist(std::vector<double> x);

std::vector<double> cell_velocity_dist(std::vector<double> v, std::vector<double> x);

std::vector<std::vector<double>> rho_j_dist(std::vector<double> x, std::vector<double> v, std::list<double> bins_list);

auto averaged_rho_dist(std::vector<std::vector<double>> x_matrix);



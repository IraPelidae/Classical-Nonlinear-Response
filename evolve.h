#pragma once
#include <utility>
#include <vector>

std::pair<double, int> min_collision_time(std::vector<double> x, std::vector<double> v);
std::vector<std::vector<double>> old_evolve_to(double stop_time, std::vector<double> x_0, std::vector<double> v_0);

std::vector<std::vector<double>> evolve_to(double stop_time, std::vector<double> x_init, std::vector<double> y_init);

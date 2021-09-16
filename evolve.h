#pragma once
#include <utility>
#include <vector>

std::pair<double, int> min_collision_time(std::vector<double> x, std::vector<double> v);
void evolve_to(double stop_time);
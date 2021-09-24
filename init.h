#ifndef __init_h__
#define __init_h__

#include <vector>
#include <string>

std::vector<double> init_velocities();

std::vector<double> init_uniform_positions();

std::vector<double> init_densityjump_positions(int l_weight, int r_weight);

std::vector <std::vector<double>> init_velocities_matrix();

std::vector <std::vector<double>> init_positions_matrix(std::string posn_init);

std::vector<double> velocity_kick(double location, double V, double width, std::vector<double> v0, std::vector<double> x0);

std::vector<std::vector<double>> kick_velocity_matrix(std::vector <std::vector<double>> v0_matrix, std::vector <std::vector<double>> x0_matrix);

#endif /* __init_h__ */
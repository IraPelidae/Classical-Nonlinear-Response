#ifndef __init_h__
#define __init_h__

#include <vector>
#include <string>

//extern std::vector<double> x_init; // Store the initial left extrema of all rods. It should remain sorted at all times
//extern std::vector<double> v_init; // Store the initial velocity of all the rods (stored in the same order as x).

std::vector<double> init_velocities();

std::vector<double> init_uniform_positions();

std::vector<double> init_densityjump_positions(int l_weight, int r_weight);

std::vector<double> init_debug_positions();



std::vector <std::vector<double>> init_velocities_matrix();

std::vector <std::vector<double>> init_positions_matrix(std::string posn_init);

#endif /* __init_h__ */
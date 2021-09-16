#ifndef __init_h__
#define __init_h__

#include <vector>

extern std::vector<double> x; // Store the left extrema of all rods. It should remain sorted at all times
extern std::vector<double> v; // Store the velocity of all the rods (stored in the same order as x).

void init_velocities();

void init_uniform_positions();

void init_densityjump_positions(int l_weight, int r_weight);

#endif /* __init_h__ */
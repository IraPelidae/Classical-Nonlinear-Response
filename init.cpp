#include <algorithm>
#include <vector>
#include <numeric>
#include "init.h"
#include "vars.h"
#include "random_wrapper.h"

std::vector<double> x(N); // Store the left extrema of all rods. It should remain sorted at all times
std::vector<double> v(N); // Store the velocity of all the rods (stored in the same order as x).

random_wrapper rand_num_gen;

/**
 * Initialise each rod's velocity v[i] by sampling independently from Gaussian distribution.
 */
void init_velocities() {

    //std::cout << "\nDisplaying velocities: ";

    for (int i = 0; i < N; i++) {
        v[i] = rand_num_gen.gaussian(T);
        //std::cout << v[i] << " "; 
    }
    //std::cout << "\n_________________________________";
}

/**
 * Left extrema of each rod x[i] is sampled independently from a uniform distribution of (system_length - N*rod_length). 
 */
void init_uniform_positions() {

    for (int i = 0; i < N; i++) {
        x[i] = rand_num_gen.uniform(0, L - N * rod_length);

    }

    std::sort(x.begin(), x.end());

    //std::cout << "\n\nDisplaying positions: ";
    for (int i = 0; i < N; i++) {
        x[i] += rod_length * i; // Make sure that none of the rods are overlapping
       // std::cout << x[i] << " "; 
    }

}
/*
 * Initialises system where init_positions are sampled uniformly apart from a density jump at L/2.
 * 
 * @param l_weight, r_weight provide relative ratios for the number of rods in each half of the system
 * e.g. l_weight = 100, r_weight = 50 means 2/3 of the rods begin in the LH side of the system. Scaling of l_weight and r_weight is arbitrary.
 */
void init_densityjump_positions(int l_weight, int r_weight) {

    int no_LH_rods = N * (l_weight) / (l_weight + r_weight); // integer division introduces small rounding error 
    std::vector<double> x_LHS(no_LH_rods);
    std::vector<double> x_RHS(N - no_LH_rods);

    //initalise system LHS
    for (int i = 0; i < no_LH_rods; i++) {
        x_LHS[i] = rand_num_gen.uniform(0, L/2 - no_LH_rods * rod_length);
    }

    std::sort(x_LHS.begin(), x_LHS.end());

    for (int i = 0; i < no_LH_rods; i++) {
        x_LHS[i] += rod_length * i; // Make sure that none of the rods are overlapping  
    }

    //initalise system RHS
    for (int i = 0; i < (N-no_LH_rods); i++) {
        x_RHS[i] = rand_num_gen.uniform(L/2, L - (N-no_LH_rods) * rod_length);
    }
    std::sort(x_RHS.begin(), x_RHS.end());
    
    for (int i = 0; i < (N-no_LH_rods); i++) {
        x_RHS[i] += rod_length * i; // Make sure that none of the rods are overlapping  
    }
    
    x_LHS.insert(x_LHS.end(), x_RHS.begin(), x_RHS.end()); //concatenate both vectors
    x = x_LHS;
}

#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>
#include <stdexcept> //std::invalid_argument
#include "init.h"
#include "vars.h"
#include "random_wrapper.h"

random_wrapper rand_num_gen;

/**
 * Initialise each rod's velocity v_init[i] by sampling independently from Gaussian distribution.
 */
std::vector<double> init_velocities() {

    std::vector<double> v_init(N); // Store the velocity of all the rods (stored in the same order as x).
    //std::cout << "\nDisplaying velocities: ";
    for (int i = 0; i < N; i++) {
        v_init[i] = rand_num_gen.gaussian(T);
        //std::cout << v[i] << " "; 
    }
    return v_init;
    //std::cout << "\n_________________________________";
}

/**
 * Left extrema of each rod x_init[i] is sampled independently from a uniform distribution of (system_length - N*rod_length). 
 */
std::vector<double> init_uniform_positions() {

    std::vector<double> x_init(N); // Store the initial LH extrema of all rods. 

    for (int i = 0; i < N; i++) {
        x_init[i] = rand_num_gen.uniform(0, L - N * rod_length);
    }

    std::sort(x_init.begin(), x_init.end());

    //std::cout << "\n\nDisplaying positions: ";
    for (int i = 0; i < N; i++) {
        x_init[i] += rod_length * i; // Make sure that none of the rods are overlapping
       // std::cout << x[i] << " "; 
    }
    return x_init;
}
/*
 * Initialises system where init_positions are sampled uniformly apart from a density jump at L/2.
 * 
 * @param l_weight, r_weight provide relative ratios for the number of rods in each half of the system
 * e.g. l_weight = 100, r_weight = 50 means 2/3 of the rods begin in the LH side of the system. Scaling of l_weight and r_weight is arbitrary.
 */
std::vector<double> init_densityjump_positions(int l_weight, int r_weight) {

    std::vector<double> x_init(N); // Store the initial LH extrema of all rods. 

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
    x_init = x_LHS;
    
    return x_init;
}

std::vector<double> init_debug_positions() {

}

/*
 * @returns a matrix containing 'no_samples' initialised velocity vectors. Matrix has shape 'no_samples' * 'no_rods'.
 */
std::vector <std::vector<double>> init_velocities_matrix() {

    std::vector <std::vector<double>> init_vel_matrix = {};

    for (int i = 0; i < no_samples; i++) {
        std::vector<double> v_init_sample = init_velocities();
        init_vel_matrix.push_back(v_init_sample);
    }

    return init_vel_matrix;
}

/*
 * @returns a matrix containing 'no_samples' initialised position vectors. Matrix has shape 'no_samples' * 'no_rods'.
 */
std::vector <std::vector<double>> init_positions_matrix(std::string posn_init) {
    
    std::vector <std::vector<double>> init_posn_matrix = {};

    try {
        if (posn_init == "init_uniform") {
            for (int i = 0; i < no_samples; i++) {
                std::vector<double> x_init_sample = init_uniform_positions();
                init_posn_matrix.push_back(x_init_sample);
            }
        }

        if (posn_init == "init_densityjump") {
            for (int i = 0; i < no_samples; i++) {
                std::vector<double> x_init_sample = init_densityjump_positions(l_weight, r_weight);
                init_posn_matrix.push_back(x_init_sample);
            }
        }      
        
    }
    catch (std::invalid_argument const& ex) {
        std::cout << ex.what() << "\n";
    }
    return init_posn_matrix; 
}
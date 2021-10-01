#include "evolve.h"
#include "vars.h"
#include "init.h"
#include <algorithm> //std::sort
#include <iostream>

/*
 * Computes min_collision time of the system of N rods and returns the index 
 * 
 * @param x is the vector of sorted rod LH extrema positions
 * @param v is the vector of rod velocities
 * @return a pair of <min_collision_time, index of LH rod associated with min_collision_time>
 */
std::pair<double, int> min_collision_time(std::vector<double> x, std::vector<double> v) {

    std::vector<double> times = {};
    std::vector<double> indices = {};

    for (int i = 0; i < N; i++) {
        //Check if rod[0] collides with LH wall or rod[1]
        if (i == 0 && v[0] < 0) {            
            times.push_back(-1 * x[0] / v[0]);
            indices.push_back(-1); // index -1 means that rod[0] collides with wall
            if (v[0] > v[1]) {
                times.push_back((x[i + 1] - (x[i] + rod_length)) / (v[i] - v[i + 1]));
                indices.push_back(i);
            }
        }
        //Check if collision occurs between rod[i] and rod[i+1] for 0 < i < N-1
        else if (i < N - 1 && v[i] > v[i + 1]) {
            times.push_back((x[i + 1] - (x[i] + rod_length)) / (v[i] - v[i + 1]));
            indices.push_back(i);
        }
        //Check if rod[N-1] collide with RH wall
        else if (i == N - 1 && v[N - 1] > 0) {
            
            times.push_back((L - (x[N - 1] + rod_length)) / v[N - 1]);
            indices.push_back(i);
        }
    }
    double min_time = *std::min_element(times.begin(), times.end());

    int min_time_index = indices[std::distance(times.begin(), std::min_element(times.begin(), times.end()))];

     /*std::cout << "\n\nDisplaying coll_index: ";

     for (int i = 0; i < indices.size(); i++) {
             std::cout << indices[i] << " ";
     }
     std::cout << "\nDisplaying coll_times: ";

     for (int i = 0; i < times.size(); i++) {
         std::cout << times[i] << " ";
     }

     std::cout << "\n\nDisplaying min_time: " << min_time;
     std::cout << "\nDisplaying min_time_index: " << min_time_index;

     */
    return std::make_pair(min_time, min_time_index); 
}

/*
 * Evolves both x and v until a given stop_time by tracking and updating x and v after each elastic collision. Slow because uses two for loops: O(N^2).
 * 
 * @param double stop_time is the time to which the system is evolved
 * @return a matrix of {x_evolved, v_evolved} where x_evolved is sorted from smallest to largest and v_evolved[i] is the corresponding velocity to x_evolved[i]
 */
std::vector<std::vector<double>> old_evolve_to(double stop_time, std::vector<double> x_0, std::vector<double> v_0) {
    double sys_time = 0;
    int no_collisions = 0;

    //initialise x and v vectors which track the positions and velocities of each particle after each collision
    std::vector<double> x_tracker = x_0;
    std::vector<double> v_tracker = v_0;

    while (sys_time < stop_time) {
        //Currently sys_time can exceed stop time by 1 collision because times are updated after the check - negligible for many particle system        
        auto mct = min_collision_time(x_tracker, v_tracker);
        double min_time = mct.first;
        int min_time_index = mct.second;

        sys_time += min_time;
        no_collisions += 1;

        //Update positions        
        for (int j = 0; j < N; j++) {
            x_tracker[j] += v_tracker[j] * min_time; 
        }
        //Updating velocities

        //hard wall BCs at 0 and L

        //if a rod collides with LH wall
        if (min_time_index == -1) { 
            v_tracker[0] = -1 * v_tracker[0];
        }
        //if a rod collides with RH wall
        else if (min_time_index == N - 1) { 
            v_tracker[N - 1] = -1 * v_tracker[N - 1];
        }
        // if a rod collies with a rod
        else if (min_time_index < N - 1) {
            std::iter_swap(v_tracker.begin() + min_time_index, v_tracker.begin() + min_time_index + 1); //swap v[j] and v[j+1]
        }

        //Print out positions and velocities after each collision
        /*
        std::cout << "\n\nAfter collision " << no_collisions;
        std::cout << "\nTime Elapsed: " << sys_time;
        std::cout << "\n\nDisplaying positions: ";
        for (int i = 0; i < N; i++) {
            std::cout << x[i] << " ";
        }
        std::cout << "\nDisplaying velocities: ";

        for (int i = 0; i < N; i++) {
            std::cout << v[i] << " ";
        }
        std::cout << "\n_________________________________"; */ 
    }  
    return { x_tracker, v_tracker };
}

/*
 * Evolves x and v until a given stop_time by updating x and v at stop_time only. Uses the fact that rods are identical and collisions between rods are elastic.
 * 
 * @param double stop_time is the time at which evolve_to stops running
 * @param std::vector<double> x_0 is the vector of the rod's initial LH extrema
 * @param std::vector<double> v_0 is the vector of the rod's initial velocities (v[0] corresponds to x[0], v[1] corresponds to x[1] etc.)
 * @return a matrix {x_evolved,v_evolved} where x_evolved is sorted from smallest to largest and v_evolved[i] is the corresponding velocity to x_evolved[i]
 */
std::vector<std::vector<double>> evolve_to(double stop_time, std::vector<double> &x_0, std::vector<double> &v_0) {

    //std::vector<double> y = x_0; //declare local vector y for x_to_y mapping
    //std::vector<double> w = v_0; //declare local velocity vector w for mutation of v_init

    double L_prime = L - N * rod_length; 

    for (int j = 0; j < N; j++) {

        x_0[j] -= rod_length * j;
        //y[j] = x_0[j] - rod_length * j; // x_to_y(x)
        x_0[j] += v_0[j] * stop_time; //Update positions to stop_time

        //Fold rod positions y[j] back over L_prime + update w[j] due to reflection from hard wall
        if (x_0[j] < 0) {
            x_0[j] = -x_0[j];  
            v_0[j] = -v_0[j];  
        }
        if (x_0[j] > L_prime) {
            x_0[j] = std::fmod(x_0[j], (2 * L_prime)); //return x[j] mod 2*L_prime
            if (x_0[j] > L_prime) {
                x_0[j] = L_prime - (x_0[j] - L_prime);
                v_0[j] = -v_0[j];
            }
        }
    }
    //sort vector y from smallest to largest & reshuffle v[j] according using the same permutation.
    
    //initalise 'index' vector of size N
    std::vector<int> index(x_0.size(), 0); 
    for (int i = 0; i != index.size(); i++) {
        index[i] = i;
    } 

    //permute indices of 'index' using the permutation that sorts y from smallest to largest
    sort(index.begin(), index.end(),
        [&](const int& a, const int& b) {
            return (x_0[a] < x_0[b]);
        }
    );
    
    //initialise copies of presorted y and w to sort (otherwise y and w will change on each iteration of the for loop)
    std::vector<double> y_copy = x_0;
    std::vector<double> w_copy = v_0;

    //permute y and w using the 'sorted' index
    for (int i = 0; i != index.size(); i++) {
        x_0[i] = y_copy[index[i]];
        v_0[i] = w_copy[index[i]];
    }
    //TODO deallocate memory from vector copies - N.B. both of these options make runtime longer
    //y_copy.clear(); //y_copy.shrink_to_fit();
    //w_copy.clear(); //w_copy.shrink_to_fit();

    //y_to_x(y) (mutate y and don't modify x_init)
    for (int j = 0; j < N; j++) {
       x_0[j] += rod_length * j;
    }
    /*std::cout << "\nTime Elapsed: " << stop_time;
    std::cout << "\n\nDisplaying positions: ";
    for (int i = 0; i < N; i++) {
        std::cout << y[i] << " ";
    }*/
    return { x_0, v_0};
    
}
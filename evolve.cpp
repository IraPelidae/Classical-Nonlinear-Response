#include "evolve.h"
#include "vars.h"
#include "init.h"
#include <algorithm>

/*
 * Computes min_collision time of the system of N rods and returns the index 
 * 
 * @param x is the vector of sorted rod LH extrema positions
 * @param v is the vector of rod velocities
 * @return pair of <min_collision_time, index of (LH) rod associated with min_collision_time>
 */
std::pair<double, int> min_collision_time(std::vector<double> x, std::vector<double> v) {

    std::vector<double> times = {};
    std::vector<double> indices = {};

    for (int i = 0; i < N; i++) {
        //Does rod[0] collide with LH wall or rod[1]
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
 * Evolves x and v until a given stop_time by updating x and v after each elastic collision. 
 */
void evolve_to(double stop_time) {
    double sys_time = 0;
    int no_collisions = 0;

    while (sys_time < stop_time) {
        //Currently sys_time can still exceed stop time for 1 collision because times are updated after the check - negligible for many particle system
        
        auto mct = min_collision_time(x, v);
        double min_time = mct.first;
        int min_time_index = mct.second;

        sys_time += min_time;
        no_collisions += 1;

        
        for (int j = 0; j < N; j++) {
            x[j] += v[j] * min_time; //Update positions
        }

        //Updating velocities

        //if a rod collides with LH wall
        if (min_time_index == -1) { 
            v[0] = -1 * v[0];
        }
        //if a rod collides with RH wall
        else if (min_time_index == N - 1) { 
            v[N - 1] = -1 * v[N - 1];
        }
        // if a rod collies with a rod
        else if (min_time_index < N - 1) {
            std::iter_swap(v.begin() + min_time_index, v.begin() + min_time_index + 1); //swap v[j] and v[j+1]
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
}
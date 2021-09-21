#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <string>
#include "init.h"
#include "vars.h"
#include "evolve.h"
#include <cmath>
namespace fs = std::filesystem;

/*
 * Runs hard rod gas simulation a number of times given by no_samples.
 * Saves the final position vectors of each simulation run in one csv file.
 */
int main() {

    std::cout << "Parameters chosen:\n";
    std::cout << " No_Samples: " << no_samples << " No_Rods: " << N << " Temp: " << T << " Sys_Length: " << L << " Rod_Length: " << std::to_string(rod_length) << "\n\n";

    std::string folder_name = "Sim_Data"; //Name of directory in which you store data file
    std::string file_name = "Time" + std::to_string(std::round(max_time)) + "_Temp" + std::to_string(T) + "_Samples" + std::to_string(no_samples) + "_Rods" + std::to_string(N) +"_Sys_Length" + std::to_string(std::round(L)) +"_RodLength" + std::to_string(rod_length); //Name of data file
    
    if (fs::is_directory(folder_name)) { std::cout << "N.B. Directory with this name already exists.\n\n"; }
    else { fs::create_directory(folder_name); }

    //Check if file_name doesn't already exist inside folder_name 

    if (fs::exists(".\\" + folder_name + "\\" + file_name)){
        std::cout << "File name already exists - please rename. Exiting";
        return 0;
    }

    else {
        std::ofstream mycsvfile;
        mycsvfile.open(".\\" + folder_name + "\\" + file_name + ".csv"); //Create csv file in which position data is stored

        std::cout << "Beginning simulation:" << "\n";
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < no_samples; i++) {

            std::cout << "\nRun " << i << " is in progress...";
            auto iter_start = std::chrono::high_resolution_clock::now();

            //Call position initialisation and sys_evolution functions
            init_densityjump_positions(l_weight, r_weight);
            //init_uniform_positions();
            init_velocities();
            
            evolve_to(max_time);

            // Print time data
            auto iter_stop = std::chrono::high_resolution_clock::now();
            auto iter_duration = std::chrono::duration_cast<std::chrono::microseconds>(iter_stop - iter_start);
            auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(iter_stop - start);

            std::cout << " and has completed after " << iter_duration.count() << "mu_s. Total runtime is " << time_elapsed.count() << "ms.";
            
            //Entering data to csv file
            if (mycsvfile.is_open()) {

                for (int j = 0; j < N-1; j++) {
                    mycsvfile << x[j] << ","; 
                }
                mycsvfile << x[N-1] << "\n"; //last entry on row doesn't require a comma
            }
            else {
                std::cout << "\nError opening file.";
            }
        }
        mycsvfile.close();
    }
}


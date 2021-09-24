#include <iostream> //std::cout
#include <chrono> //std::chrono::high_resolution_clock, std::chrono::duration_cast
#include <fstream> // std::ofstream 
#include <filesystem> //fs::is_directory, fs::create_directory
#include <iomanip> //std::set_precision
#include "init.h"
#include "vars.h"
#include "evolve.h"

namespace fs = std::filesystem;

void print_parameters();
void sample_xv_in_csv(std::string folder, std::string file, float time, std::vector <std::vector<double>> init_vel_matrix, std::vector <std::vector<double>> init_pos_matrix, bool exporting_vel);

/*
 * Runs hard rod gas simulation a number of times given by no_samples for each sampling_time, up until max_time. Set ALL parameters in vars.h
 * Saves the final position vectors of each simulation run in one csv file.
 */
int main() {

    print_parameters();

    //Create a directory if there isn't one       
    if (fs::is_directory(folder_name)) { std::cout << "N.B. Directory with this name already exists.\n\n"; }
    else { fs::create_directory(folder_name); }
        
    // Start clock
    std::cout << "Beginning simulation:" << "\n";
    auto start = std::chrono::high_resolution_clock::now();

    //Call init.cpp functions
    std::cout << "Initialisation in progress...";
    std::vector <std::vector<double>> init_vel_matrix = init_velocities_matrix();
    std::vector <std::vector<double>> init_posn_matrix = init_positions_matrix(posn_init_method);

    if (implementing_v_kick) { init_vel_matrix = kick_velocity_matrix(init_vel_matrix, init_posn_matrix); }

    auto init_stop = std::chrono::high_resolution_clock::now();
    auto init_duration = std::chrono::duration_cast<std::chrono::milliseconds>(init_stop - start);
    std::cout << " and has completed after " << init_duration.count() << "ms. Total runtime is " << init_duration.count()/1000 << "s.";

    //Call evolve.cpp functions + export to csv
    std::cout << "\nBegin evolving + sampling:" << "\n";

    //For loop to run sample_xv_in_csv every 'sampling_time' up until 'max_time'
    for (int i = 0; i < static_cast<int> (max_time / sampling_time) + 1; i++) {

        std::cout << "\nSampling at time t = " << std::to_string(sampling_time * i) << " is in progress...";
        auto iter_start = std::chrono::high_resolution_clock::now();

        //Set name of .csv file
        std::string file_name = "Time" + std::to_string(sampling_time*i) + "_Temp" + std::to_string(T) + "_Samples" + std::to_string(no_samples) \
        + "_Rods" + std::to_string(N) +"_Sys_Length" + std::to_string(L) +"_RodLength" + std::to_string(rod_length); 
                                                                                                                                                                                                                                                                              //Check if file_name doesn't already exist inside folder_name 
        if (fs::exists(".\\" + folder_name + "\\" + file_name)) {
            std::cout << "File name already exists - please rename. Exiting";
            return 0;
        }
        else {
            //Call sample_xv_in_csv
            sample_xv_in_csv(folder_name,file_name,sampling_time*i, init_vel_matrix, init_posn_matrix, export_v); 

            auto iter_stop = std::chrono::high_resolution_clock::now();
            auto iter_duration = std::chrono::duration_cast<std::chrono::milliseconds>(iter_stop - iter_start);
            auto time_elapsed = std::chrono::duration_cast<std::chrono::seconds>(iter_stop - start);
            std::cout << " and has completed after " << iter_duration.count() << "ms. Total runtime is " << time_elapsed.count() << "s.";
        }        
    }   
}

/*
 * Prints parameters in vars.h
 */
void print_parameters() {
    std::cout << "Parameters chosen:\n";
    std::cout << "Max_Time: " << max_time << " Sampling_Time: " << sampling_time << " No_Samples: " << no_samples << "\n";
    std::cout << "No_Rods: " << N << " Temp: " << T << " Sys_Length: " << L << " Rod_Length: " << std::to_string(rod_length) << "\n\n";
    std::cout << "Position sampling method: " << posn_init_method << "\n";

    if (posn_init_method == "init_densityjump") {
        std::cout << "Left_weight: " << l_weight << " Right_weight: " << r_weight << "\n";
    }

    if (implementing_v_kick) {
        std::cout << "Implementing_v_kick: " << implementing_v_kick << "\n";
        std:: cout << "Kick_Centre: " << kick_centre << " Kick_Width: " << kick_width << " V " << V << "\n\n";
    }
    else {
        std::cout << "Implementing_v_kick: " << implementing_v_kick << "\n";
    }
}

/*
 * Creates a .csv file for both x[N] and v[N] after the system is evolved to a particular time. Both .csv files have 'no_rods' columns and 'no_samples' rows.
 * 
 * @param std::string folder is the existing/new folder in which you want to store the .csv file
 * @param std::string file contains the simulation parameter data of the .csv file
 * @param float time is the duration for which evolve_to runs (evolve_to takes 'time' as an argument)
 * @param init_vel_matrix and init_pos_matrix are the matrices containing 'no_samples' intial velocity and position vectors respectively
 * @param bool export_vel exports velocities to a csv file, default value is False.
 */
void sample_xv_in_csv(std::string folder, std::string file, float time, std::vector <std::vector<double>> init_vel_matrix, std::vector <std::vector<double>> init_pos_matrix, bool exporting_vel) {

    std::ofstream positions_csv_file;
    positions_csv_file.open(".\\" + folder + "\\P_" + file + ".csv"); //Create csv file in which position data is stored

    
    std::ofstream velocities_csv_file;
    velocities_csv_file.open(".\\" + folder + "\\V_" + file + ".csv"); //Create csv file in which velocity data is stored
    
    //std::cout << "Beginning sample_run:" << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    //Call evolve functions 'sample_no' times
    for (size_t i = 0; i < init_vel_matrix.size(); i++) {

        //std::cout << "\nRun " << i << " is in progress...";
        auto iter_start = std::chrono::high_resolution_clock::now();

        //Call evolve.cpp function
        auto xv_matrix = evolve_to(time, init_pos_matrix[i], init_vel_matrix[i]); // x_evolved = xv_matrix[0]; v_evolved = xv_matrix[1]        

        // Print time data
        auto iter_stop = std::chrono::high_resolution_clock::now();
        auto iter_duration = std::chrono::duration_cast<std::chrono::microseconds>(iter_stop - iter_start);
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(iter_stop - start);

        //std::cout << " and has completed after " << iter_duration.count() << "mu_s. Total runtime is " << time_elapsed.count() << "ms.";

        //Entering position data to csv file
        if (positions_csv_file.is_open()) {

            for (int j = 0; j < N - 1; j++) {
                positions_csv_file << xv_matrix[0][j] << ",";
            }
            positions_csv_file << xv_matrix[0][N - 1] << "\n"; //last entry on row doesn't require a comma
        }
        else {
            std::cout << "\nError opening file.";
        }

        if (exporting_vel) {
            if (velocities_csv_file.is_open()) {

                for (int j = 0; j < N - 1; j++) {
                    velocities_csv_file << xv_matrix[1][j] << ",";
                }
                velocities_csv_file << xv_matrix[1][N - 1] << "\n"; //last entry on row doesn't require a comma
            }
            else {
                std::cout << "\nError opening file.";
            }
        }

    }
    //Close csv files
    positions_csv_file.close();
    if (exporting_vel) {
        velocities_csv_file.close();
    }
    else {
        velocities_csv_file.close();
        fs::remove(".\\" + folder + "\\V_" + file + ".csv");
    }
}

#include <iostream> //std::cout
#include <chrono> //std::chrono::high_resolution_clock, std::chrono::duration_cast
#include <fstream> // std::ofstream 
#include <filesystem> //fs::is_directory, fs::create_directory
#include <boost/progress.hpp> //boost::progress_display, boost::progress_timer
#include "histogram.h"
#include "init.h"
#include "vars.h"
#include "evolve.h"

namespace fs = std::filesystem;
void print_parameters();

//three function declarations below are no longer in use
std::vector<std::vector<double>> evolve_and_hist(float time, std::vector <std::vector<double>> init_vel_matrix, std::vector <std::vector<double>> init_pos_matrix);
void sample_xv_in_csv(std::string folder, std::string file, float time, std::vector <std::vector<double>> init_vel_matrix, std::vector <std::vector<double>> init_pos_matrix, bool exporting_vel);
void old_main();
/*
 * Runs hard rod gas simulation a number of times given by no_samples for each sampling_time, up until max_time. Set ALL parameters in vars.h
 * Saves the averaged density and current distribution time_series of each simulation run in two csv files.
 */
int main() {

    print_parameters();

    //Create a directory if there isn't one       
    if (fs::is_directory(folder_name)) { std::cout << "N.B. Directory with this name already exists.\n\n"; }
    else { fs::create_directory(folder_name); }
    
    //Initialise csv files for exporting    
    //Set file names
    std::string file_name = "T" + std::to_string(int(max_time)) + "_ST" + std::to_string(int(sampling_time)) + \
        "_V" + std::to_string(float(V)) + "_kickW" + std::to_string(kick_width) + "_temp" + std::to_string(float(T)) + \
        "_samples" + std::to_string(no_samples) \
        + "_rods" + std::to_string(N) + "_sys_length" + std::to_string(int(L)) + "_rodlength" + std::to_string(float(rod_length));

    std::ofstream rho_bar_csvfile;
    rho_bar_csvfile.open(".\\" + folder_name + "\\D_" + file_name + ".csv"); //Create csv file in which density data is stored

    std::ofstream j_bar_csvfile;
    j_bar_csvfile.open(".\\" + folder_name + "\\J_" + file_name + ".csv"); //Create csv file in which current data is stored

    if (fs::exists(".\\" + folder_name + "\\" + file_name)) {
        std::cout << "File name already exists - please rename. exiting";
        return 0;
    }
    else {        

        //Initialise matrices of density + current data for reading to csv

        int no_time_samples = std::round(max_time / sampling_time + 1); 
        int no_cells = std::round(L / cell_length);

        std::vector<std::vector<double>> rho_accumulator(no_time_samples, std::vector<double>(no_cells)); //density matrix size no_time_samples x no_cells
        std::vector<std::vector<double>> j_accumulator(no_time_samples, std::vector<double>(no_cells)); //current matrix size no_time_samples x no_cells

        //Initalise progress bar
        unsigned int sample_size = no_samples;
        boost::progress_display show_progress(sample_size);

        //Start timing
        std::cout << "Beginning simulation:" << "\n";
        boost::progress_timer t;

        for (unsigned int i = 0; i < sample_size; i++) {

            //initialise sample positions + velocities from init.cpp

            std::vector<double> x_0 = init_uniform_positions();

            std::vector<double> v_0 = init_velocities();

            if (implementing_v_kick) { v_0 = velocity_kick(kick_centre, V, kick_width, v_0, x_0); }

            for (int j = 0; j < static_cast<int> (max_time / sampling_time) + 1; j++) {

                std::vector<std::vector<double>> xv_evolved = evolve_to(sampling_time * j, x_0, v_0); // call evolve.cpp function

                std::vector<std::vector<double>> rho_j_dist_matrix = rho_j_dist(xv_evolved[0], xv_evolved[1]); // call histogram.cpp function

                //add to density and current containers
                std::transform(rho_accumulator[j].begin(), rho_accumulator[j].end(), rho_j_dist_matrix[0].begin(), rho_accumulator[j].begin(), std::plus<double>());
                std::transform(j_accumulator[j].begin(), j_accumulator[j].end(), rho_j_dist_matrix[1].begin(), j_accumulator[j].begin(), std::plus<double>());

            }

            ++show_progress;

        }
        int sample_no = no_samples; //no_samples is a const so can't be used in a lambda

        for (int i = 0; i < no_time_samples; i++) {
            //normalise rho and j by sample_no
            std::transform(rho_accumulator[i].begin(), rho_accumulator[i].end(), rho_accumulator[i].begin(), [&sample_no](auto& c) {return c / sample_no;});
            std::transform(j_accumulator[i].begin(), j_accumulator[i].end(), j_accumulator[i].begin(), [&sample_no](auto& c) {return c / sample_no;});
        }
        //write rho_accumulator and j_accumulator matrices to csv files

        if (rho_bar_csvfile.is_open()) {

            for (unsigned i = 0; i < rho_accumulator.size(); i++) {
                for (unsigned j = 0; j < rho_accumulator[i].size() - 1; j++) {
                    rho_bar_csvfile << rho_accumulator[i][j] << ",";
                }
                rho_bar_csvfile << rho_accumulator[i].back() << "\n"; //last entry on row doesn't require a comma
            }
        }
        else {
            std::cout << "\nError opening file.";
        }

        if (j_bar_csvfile.is_open()) {

            for (unsigned i = 0; i < j_accumulator.size(); i++) {
                for (unsigned j = 0; j < j_accumulator[i].size() - 1; j++) {
                    j_bar_csvfile << j_accumulator[i][j] << ",";
                }
                j_bar_csvfile << j_accumulator[i].back() << "\n"; //last entry on row doesn't require a comma
            }
        }
        else {
            std::cout << "\nError opening file.";
        }
        //End of simulation
        std::cout << "\n\n Hard rod simulation has completed with runtime of ";
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
        std:: cout << "Kick_Centre: " << kick_centre << " Kick_Width: " << kick_width << " V: " << V << "\n\n";
    }
    else {
        std::cout << "Implementing_v_kick: " << implementing_v_kick << "\n";
    }
}

/*
* Calls evolve.cpp and hist.cpp functions - returns averaged density and current distributions for each time. 
*/
std::vector<std::vector<double>> evolve_and_hist(float time, std::vector <std::vector<double>> init_vel_matrix, std::vector <std::vector<double>> init_pos_matrix) {

    std::vector<double> rho_accumulator(std::round(L / cell_length)); //initialise vector which adds the density distributions for each sample
    std::vector<double> j_accumulator(std::round(L / cell_length)); //initialise vector which adds the current distributions for each sample

    for (size_t i = 0; i < init_vel_matrix.size(); i++) {

        auto start = std::chrono::high_resolution_clock::now();

        //std::cout << "\nRun " << i << " is in progress...";
        auto iter_start = std::chrono::high_resolution_clock::now();

        //Call evolve.cpp function
        auto xv_matrix = evolve_to(time, init_pos_matrix[i], init_vel_matrix[i]); // x_evolved = xv_matrix[0]; v_evolved = xv_matrix[1]        

        //histogram call
        auto rho_j_dist_matrix = rho_j_dist(xv_matrix[0], xv_matrix[1]);
        

        //add to density and current containers
        std::transform(rho_accumulator.begin(), rho_accumulator.end(), rho_j_dist_matrix[0].begin(), rho_accumulator.begin(), std::plus<double>()); 
        std::transform(j_accumulator.begin(), j_accumulator.end(), rho_j_dist_matrix[1].begin(), j_accumulator.begin(), std::plus<double>());

        // Print time data
        auto iter_stop = std::chrono::high_resolution_clock::now();
        auto iter_duration = std::chrono::duration_cast<std::chrono::microseconds>(iter_stop - iter_start);
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(iter_stop - start);
    }
    //normalise density and current containers by no_samples
    int sample_no = no_samples;
    std::transform(rho_accumulator.begin(), rho_accumulator.end(), rho_accumulator.begin(), [&sample_no](auto& c) {return c / sample_no;});
    std::transform(j_accumulator.begin(), j_accumulator.end(), j_accumulator.begin(), [& sample_no](auto& c) {return c / sample_no;});

    return { rho_accumulator, j_accumulator };
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
        
        //HISTOGRAM CALL
                
        // Print time data
        auto iter_stop = std::chrono::high_resolution_clock::now();
        auto iter_duration = std::chrono::duration_cast<std::chrono::microseconds>(iter_stop - iter_start);
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(iter_stop - start);

        //std::cout << " and has completed after " << iter_duration.count() << "mu_s. Total runtime is " << time_elapsed.count() << "ms.";
        
        

        //Entering position data to csv file
        if (positions_csv_file.is_open()) {

            for (int j = 0; j < N - 1; j++) {
                //positions_csv_file << xv_matrix[0][j] << ",";
            }
            //positions_csv_file << xv_matrix[0][N - 1] << "\n"; //last entry on row doesn't require a comma
        }
        else {
            std::cout << "\nError opening file.";
        }

        if (exporting_vel) {
            if (velocities_csv_file.is_open()) {

                for (int j = 0; j < N - 1; j++) {
                    //velocities_csv_file << xv_matrix[1][j] << ",";
                }
                //velocities_csv_file << xv_matrix[1][N - 1] << "\n"; //last entry on row doesn't require a comma
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

void old_main() {
    print_parameters();

    //Run initialisation
    //Create a directory if there isn't one       
    if (fs::is_directory(folder_name)) { std::cout << "N.B. Directory with this name already exists.\n\n"; }
    else { fs::create_directory(folder_name); }

    //Start clock
    std::cout << "Beginning simulation:" << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    //Call init.cpp functions
    std::cout << "Initialisation in progress...";
    std::vector <std::vector<double>> init_vel_matrix = init_velocities_matrix();
    std::vector <std::vector<double>> init_posn_matrix = init_positions_matrix(posn_init_method);

    if (implementing_v_kick) { init_vel_matrix = kick_velocity_matrix(init_vel_matrix, init_posn_matrix); }

    auto init_stop = std::chrono::high_resolution_clock::now();
    auto init_duration = std::chrono::duration_cast<std::chrono::milliseconds>(init_stop - start);
    std::cout << " and has completed after " << init_duration.count() << "ms. total runtime is " << init_duration.count() / 1000 << "s.";

    std::cout << "\nBegin evolving + sampling:" << "\n";

    //set file_name
    std::string file_name = "T" + std::to_string(std::round(max_time)) + "ST" + std::to_string(std::round(sampling_time)) + \
        "_temp" + std::to_string(std::round(T)) + "_samples" + std::to_string(no_samples) \
        + "_rods" + std::to_string(N) + "_sys_length" + std::to_string(std::round(L)) + "_rodlength" + std::to_string(rod_length);

    //check if file_name doesn't already exist inside folder_name 
    if (fs::exists(".\\" + folder_name + "\\" + file_name)) {
        std::cout << "File name already exists - please rename. exiting";
        //return 0;
    }
    else {
        std::ofstream rho_bar_csvfile;
        rho_bar_csvfile.open(".\\" + folder_name + "\\D_" + file_name + ".csv"); //Create csv file in which density data is stored

        std::ofstream j_bar_csvfile;
        j_bar_csvfile.open(".\\" + folder_name + "\\J_" + file_name + ".csv"); //Create csv file in which current data is stored

        int no_time_samples = std::round(max_time / sampling_time) + 1;

        std::vector<std::vector<double>> rho_t_series(no_time_samples);
        std::vector<std::vector<double>> j_t_series(no_time_samples);

        //for loop to run evolve_and_hist every 'sampling_time' up until 'max_time'
        for (int i = 0; i < static_cast<int> (max_time / sampling_time) + 1; i++) {

            std::cout << "\nSampling at time t = " << std::to_string(sampling_time * i) << " is in progress...";
            auto iter_start = std::chrono::high_resolution_clock::now();

            //call evolve_and_hist
            std::vector<std::vector<double>> averaged_rho_j_dists = evolve_and_hist(sampling_time * i, init_vel_matrix, init_posn_matrix);
            rho_t_series[i] = averaged_rho_j_dists[0];
            j_t_series[i] = averaged_rho_j_dists[1];

            auto iter_stop = std::chrono::high_resolution_clock::now();
            auto iter_duration = std::chrono::duration_cast<std::chrono::milliseconds>(iter_stop - iter_start);
            auto time_elapsed = std::chrono::duration_cast<std::chrono::seconds>(iter_stop - start);
            std::cout << " and has completed after " << iter_duration.count() << "ms. Total runtime is " << time_elapsed.count() << "s.";
        }
        //exporting to csv

        if (rho_bar_csvfile.is_open()) {

            for (unsigned i = 0; i < rho_t_series.size(); i++) {
                for (unsigned j = 0; j < rho_t_series[i].size() - 1; j++) {
                    rho_bar_csvfile << rho_t_series[i][j] << ",";
                }
                rho_bar_csvfile << rho_t_series[i].back() << "\n"; //last entry on row doesn't require a comma
            }
        }
        else {
            std::cout << "\nError opening file.";
        }

        if (j_bar_csvfile.is_open()) {

            for (unsigned i = 0; i < j_t_series.size(); i++) {
                for (unsigned j = 0; j < j_t_series[i].size() - 1; j++) {
                    j_bar_csvfile << j_t_series[i][j] << ",";
                }
                j_bar_csvfile << j_t_series[i].back() << "\n"; //last entry on row doesn't require a comma
            }
        }
        else {
            std::cout << "\nError opening file.";
        }

    }
}
#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <string>
#include "init.h"
#include "vars.h"
#include "evolve.h"
namespace fs = std::filesystem;

/*
 * Runs hard rod gas simulation a number of times given by no_samples.
 * Saves the final position vectors of each simulation run in a csv file.
 */
int main() {

    std::cout << "Parameters chosen:\n";
    std::cout << "No_Rods: " << N << " Temp: " << T << " Sys_Length: " << L << " Rod_Length: " << rod_length << " No_Samples: " << no_samples << "\n\n";

    std::string folder_name = "Sim_DataCSV"; //Name of directory in which you store data file
    std::string file_name = "200Samples_4050rods_RodLength_0.001"; //Name of data file
    
    //Create new directory if folder_name doesn't already exist
    if (fs::is_directory(folder_name)) {std::cout << "N.B. Directory with this name already exists.\n\n";} else {fs::create_directory(folder_name);}

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
            init_densityjump_positions(100, 50);
            //init_uniform_positions();
            init_velocities();
            evolve_to(3);

            // Print time data
            auto iter_stop = std::chrono::high_resolution_clock::now();
            auto iter_duration = std::chrono::duration_cast<std::chrono::seconds>(iter_stop - iter_start);
            auto time_elapsed = std::chrono::duration_cast<std::chrono::minutes>(iter_stop - start);

            std::cout << " and has completed after " << iter_duration.count() << "s. Total runtime is " << time_elapsed.count() << "mins.";

            //Saves position data of this run to individual text files
            //std::ofstream mytxtfile;
            //mytxtfile.open(".\\" + folder_name + "\\Run" + std::to_string(i + 1) + ".txt");

            /*if (mytxtfile.is_open()) {
                for (int i = 0; i < N; i++) {
                    mytxtfile << x[i] << "\n";
                }
                mytxtfile.close();
            }
            else {
                std::cout << "\nError opening file.";
            }*/

            if (mycsvfile.is_open()) {

                //mycsvfile << "Run" + std::to_string(i + 1);
                for (int j = 0; j < N; j++) {
                    mycsvfile << x[j] << ",";
                }
                mycsvfile << ",\n";

            }
            else {
                std::cout << "\nError opening file.";
            }
        }
        mycsvfile.close();

    }
}


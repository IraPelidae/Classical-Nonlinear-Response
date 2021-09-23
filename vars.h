#pragma once
#include <string>

//System variables

const int N = 3988; //number of rods
const auto T = 1 / sqrt(2); //temperature
const float L = 30; //length of the system
const double rod_length = 0.001; //rod length

//Simulation parameters
const int no_samples = 2000; //number of simulation runs (i.e. sample size)
const float max_time = 3; //max time to which system evolves
const float sampling_time = 3; //sampling time period

const std::string posn_init_method ="init_densityjump"; // can choose between "init_uniform" ,"init_densityjump"

const float l_weight = 100; //init_density_jump parameters
const float r_weight = 50;

//Directory
const std::string folder_name = "23_09_New_Evolve"; //Name of directory in which you store data file



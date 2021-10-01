#pragma once
#include <string>

//System variables

const int N = 128; //number of rods
const auto T = 0.5; //temperature
const double L = 30; //length of the system
const double rod_length = 0.125; //rod length
const double cell_length = 0.1; 

//Simulation parameters
const int no_samples = 1e7; //number of simulation runs (i.e. sample size)
const float max_time = 2; //max time to which system evolves
const float sampling_time = 0.5; //sampling time period

const std::string posn_init_method ="init_uniform"; // can choose between "init_uniform" ,"init_densityjump"

const float l_weight = 100; //init_density_jump parameters
const float r_weight = 50;

const bool implementing_v_kick = true; 
const double kick_centre = L / 2;
const double V = 0.1;
const double kick_width = 2;

//Exporting + Directory
const bool export_v = true;
const std::string folder_name = "01_10_high_interaction"; //Name of directory in which you store data file



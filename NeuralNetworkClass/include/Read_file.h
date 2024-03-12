#pragma once
#include <vector>
#include <fstream>
#include <ostream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <chrono>

#include "Neuralnetwork.h"
#include <string>


Mat load_file_to_matrix_image(std::string _name);
Mat load_file_to_matrix_label(std::string _name);
Mat load_file_to_matrix(std::string _name_file_in, std::string _name_file_out);
NN load_data_network(std::string _path);
void save_data_network(NN _nn, std::string _path);


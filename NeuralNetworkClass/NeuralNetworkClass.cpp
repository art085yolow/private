#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>

#include "include/Neuralnetwork.h"
#include "include/DataStream.h"
#include "include/RNN.h"



int main()
{
    std::cout << "Hello World!\n";

    // very small network
    // test2();    
   
    // data files mnist
    // 60k handwriten digits
    DataStream train_Images("testData/train-images.idx3-ubyte", "testData/train-labels.idx1-ubyte");
    // 10k handwriten digits
    DataStream test_Images("testData/t10k-images.idx3-ubyte", "testData/t10k-labels.idx1-ubyte");

    // network structure
    int input_size = test_Images.getWidth() * test_Images.getHeight(),    //inputs
        hidden_layer_size_0 = 28,                                         // h1
        hidden_layer_size_1 = 14,                                         // h2
        hidden_layer_size_2 = 10,                                         // h3
        output_size = 10;                                               // outputs
    
    // 784-16-16-10
    std::string s = std::to_string(input_size) + "-" + std::to_string(hidden_layer_size_0) + "- " + std::to_string(hidden_layer_size_1) + "- " + std::to_string(hidden_layer_size_2) + "- " + std::to_string(output_size);
    // creating network
    RNN tested_Network(s);
    
    // training network until reach 91% accuracity
    while (tested_Network.procent_of_correct_asware() < 95.0f)
    {

    tested_Network.trainNetwork(train_Images, 10, 45000);
    
    tested_Network.testNetwork(test_Images, 7500);
    }



    return 0;
};



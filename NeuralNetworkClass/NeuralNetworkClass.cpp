#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>

#include "include/Neuralnetwork.h"
#include "include/DataStream.h"
#include "include/RNN.h"


//void test1();
void test2();

int main()
{
    std::cout << "Hello World!\n";

    // very small network
    test2();    
   
    // data files mnist
    // 60k handwriten digits
    DataStream train_Images("testData/train-images.idx3-ubyte", "testData/train-labels.idx1-ubyte");
    // 10k handwriten digits
    DataStream test_Images("testData/t10k-images.idx3-ubyte", "testData/t10k-labels.idx1-ubyte");

    // network structure
    int input_size = test_Images.getWidth() * test_Images.getHeight(),    //inputs
        hidden_layer_size_0 = 22,                                         // h1
        hidden_layer_size_1 = 20,                                         // h2
        output_size = 10;                                               // outputs
    
    // 784-16-16-10
    std::string s = std::to_string(input_size) + "-" + std::to_string(hidden_layer_size_0) + "- " + std::to_string(hidden_layer_size_1) + "- " + std::to_string(output_size);
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


/*      ETAP DRUGI      -----------         WDROZYC BACKPROPAGATION!!!!         */        
/*
void test1()
{

   // przyklad do etapu drugiego -- example net
    /*  2x2x2
    *   input-1 = 0.05      input-2 = 0.1
    *   w1 = 0.15   w2 = 0.2    w3 = 0.25   w4 = 0.3
    *   h1                      h2                      b1 = 0.35
    *   w5 = 0.4    w5 = 0.45   w7 = 0.5    w8 = 0.55
    *   output-1                output-2                b2 = 0.60
    *
    * y_o1 = 0.01               y_o2 = 0.99
    *
    * ratio = 0.5
    */

    /// przygotowanie -- preparing
    /*
    std::vector<double*> in;
    in.push_back(new double(0.05));
    in.push_back(new double(0.1));

    for (size_t i = 0, tt = in.size(); i < tt; i++)
    {
        std::cout << "input-" << i << " = " << *in[i] << std::endl;
    }
    std::cout << std::endl;

    std::vector<double*> out;
    out.push_back(new double(0.0));
    out.push_back(new double(0.0));

    for (size_t i = 0, tt = out.size(); i < tt; i++)
    {
        std::cout << "output-" << i << " = " << *out[i] << std::endl;
    }

    NeuralNetwork nn;
    nn.createLayersOfNeurons(in, out, 1, 2);

    std::vector<double*> yOut;
    yOut.push_back(new double(0.01));
    yOut.push_back(new double(0.99));

    nn.setYPredicted(yOut);
    nn.setRatio(0.5);

    std::vector<double*> weights = nn.getNetLayers()[1]->getNeuronsFromLayer()[0]->getWeightVec();
    *weights[0] = 0.15;
    *weights[1] = 0.2;


    weights = nn.getNetLayers()[1]->getNeuronsFromLayer()[1]->getWeightVec();
    *weights[0] = 0.25;
    *weights[1] = 0.30;

    weights = nn.getNetLayers()[2]->getNeuronsFromLayer()[0]->getWeightVec();
    *weights[0] = 0.4;
    *weights[1] = 0.45;

    weights = nn.getNetLayers()[2]->getNeuronsFromLayer()[1]->getWeightVec();
    *weights[0] = 0.5;
    *weights[1] = 0.55;

    weights.clear();

    nn.getNetLayers()[1]->getNeuronsFromLayer()[0]->setBias(0.35);
    nn.getNetLayers()[1]->getNeuronsFromLayer()[1]->setBias(0.35);
    nn.getNetLayers()[2]->getNeuronsFromLayer()[0]->setBias(0.60);
    nn.getNetLayers()[2]->getNeuronsFromLayer()[1]->setBias(0.60);

    nn.process();

    std::cout << "---------------------" << std::endl;
    std::cout << std::endl;

    for (size_t i = 0, tt = out.size(); i < tt; i++)
    {
        std::cout << "output-" << i << " = " << *out[i] << std::endl;
    }

    /// <summary>
    ///  siec wstepna gotowa -- net done
    ///
    /// przewidzne wyniki - predictecd results
    ///
    /// h1 = 0.59326992
    /// h2 = 0.596884378
    ///
    /// o1 = 0.75136507
    /// o2 = 0.772928465
    ///
    /// wyniki zgodne -- results correct 
    /// -- feedforward -- CORRECT --  PRAWIDLOWY!!!!
    ///
    /// </summary>
    /// <returns></returns>

    std::cout << "---------------------" << std::endl;
    std::cout << std::endl;


    std::cout << "Blad sieci -- Net Error : " << nn.getError() << std::endl;

    /// <summary>
    ///
    /// obliczanie bledu PRAWIDLOWO!!!!
    /// calculation net error - correct!
    ///
    /// Zaczynamy proces backpropagation!!!
    /// initiation backpropagation!
    /// 
    /// </summary>
    /// <returns></returns>
    double netError = nn.getError();
    double predictedError = 0.000001;
    while (netError > predictedError)
    {
        nn.calculateTotalError();
        nn.backProb();
        nn.process();
        std::cout << "Blad sieci -- Net Error : " << nn.getError() << std::endl;
        netError = nn.getError();
    }

    for (size_t i = 0, tt = out.size(); i < tt; i++)
    {
        std::cout << "output-" << i << " = " << *out[i] << std::endl;
    }

    /// <summary>
    ///
    ///         przykładowa siec --- zaliczone!!!! zdala!!!!
    ///         example net --- correct!!! done!!!
    /// </summary>
    /// <returns></returns>
}
*/

void test2()
{
    // podobny do test1 - [ok]
    // test2/random bias&weights -- [ok]
    NeuralNetwork theNet("2-2-2");

    /*          using reference operator[] 
    std::vector<std::reference_wrapper<double>> inputs;
    theNet.getInputs(inputs);

    if (!inputs.empty() && (inputs.size() > 1))
    {

        inputs[0].get() = 0.05;
        inputs[1].get() = 0.1;
    }
    */

    theNet["I-0"] = 0.05;
    theNet["I-1"] = 0.1;

    theNet.process();

    // set Y prediction
    std::vector<double> y;

    y.push_back(0.01);
    y.push_back(0.99);

    // print to console
    theNet.print_All();

    // train net
    unsigned int stepper = 0, check=50;

    while (theNet.get_network_error(y) > 0.001)
    {
        std::cout << "\nSTEP: " << stepper << " ERROR NET: " << theNet.get_network_error(y) << "\n";
        // error vectors for neurons and synapses
        std::vector<double> errNeu, errSyn;

        // calculating error for current weights and biases
        theNet.calculate_network_error(errNeu, errSyn, y);
        // applying correction
        theNet.backpropagation(errNeu, errSyn);
        // calculating with new weights and biases
        theNet.process();

        // 50step check print
        if (stepper == check)
        {
            check += 50;
            theNet.print_All();
        }

        stepper++;
    }

    std::cout << "\n - - - - - RESOULTS - - - - -\n";
    theNet.print_All();

    theNet.print_Inputs();
    theNet.print_Outputs();

    std::cout << "Network saved: " << theNet.save_network() << "\n\n";

    // test complite - [ok]

    // new object with network loaded from file
    NeuralNetwork new_one("2-2-2");
    

    for (size_t i=0; i<new_one.get_input_size();i++)
    {
        new_one["I-"+std::to_string(i)]=new_one.random_double();
    }

    std::cout << "\n - - - - - RESOULTS - - - - -\n";
    new_one.print_All();
}

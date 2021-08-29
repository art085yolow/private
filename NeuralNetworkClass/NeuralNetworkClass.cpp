#include <iostream>
#include <vector>

#include "Neuralnetwork.h"


int main()
{
    std::cout << "Hello World!\n";
   
   // przyklad do etapu drugiego
    /*
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


    /// przygotowanie

    std::vector<double*> in;
    in.push_back(new double(0.05));
    in.push_back(new double(0.1));

    for (size_t i = 0, tt = in.size(); i < tt; i++)
    {
        std::cout << "i" << i << " = " << *in[i] << std::endl;
    }
    std::cout << std::endl;

    std::vector<double*> out;
    out.push_back(new double(0.0));
    out.push_back(new double(0.0));

    for (size_t i = 0, tt = out.size(); i < tt; i++)
    {
        std::cout << "i" << i << " = " << *out[i] << std::endl;
    }

    NeuralNetwork nn;
    nn.createLayersOfNeurons(in, out, 1, 2);
    
    std::vector<double> yOut;
    yOut.push_back(double(0.01));
    yOut.push_back(double(0.99));

    nn.setYPredicted(yOut);
    nn.setRatio(0.5);
    
    std::vector<double*> wagi = nn.NeuralNetworkLayers[1]->neuronsInLayer[0]->getWeightVec();
    *wagi[0] = 0.15;
    *wagi[1] = 0.2;
    
    wagi = nn.NeuralNetworkLayers[1]->neuronsInLayer[1]->getWeightVec();
    *wagi[0] = 0.25;
    *wagi[1] = 0.30;
    
    wagi = nn.NeuralNetworkLayers[2]->neuronsInLayer[0]->getWeightVec();
    *wagi[0] = 0.4;
    *wagi[1] = 0.45;
    
    wagi = nn.NeuralNetworkLayers[2]->neuronsInLayer[1]->getWeightVec();
    *wagi[0] = 0.5;
    *wagi[1] = 0.55;
    
    wagi.clear();

    nn.NeuralNetworkLayers[1]->neuronsInLayer[0]->setBias(0.35);
    nn.NeuralNetworkLayers[1]->neuronsInLayer[1]->setBias(0.35);
    nn.NeuralNetworkLayers[2]->neuronsInLayer[0]->setBias(0.60);
    nn.NeuralNetworkLayers[2]->neuronsInLayer[1]->setBias(0.60);

    nn.process();

    std::cout << "---------------------" << std::endl;
    std::cout << std::endl;

    for (size_t i = 0, tt = out.size(); i < tt; i++)
    {
        std::cout << "i" << i << " = " << *out[i] << std::endl;
    }

    /// <summary>
    ///  siec wstepna gotowa
    /// 
    /// przewidzne wyniki
    /// 
    /// h1 = 0.59326992
    /// h2 = 0.596884378
    /// 
    /// o1 = 0.75136507
    /// o2 = 0.772928465
    /// 
    /// wyniki zgodne -- feedforward PRAWIDLOWY!!!!
    /// 
    /// </summary>
    /// <returns></returns>

    std::cout << "---------------------" << std::endl;
    std::cout << std::endl;

    
    std::cout << "Blad sieci : " << nn.getError() << std::endl;

    /// <summary>
    ///
    /// obliczanie bledu PRAWIDLOWO!!!!
    /// 
    /// 
    /// Zaczynamy proces backpropagation!!!
    /// 
    /// </summary>
    /// <returns></returns>

    nn.calculateTotalError();



    std::cin.get();


    return 0;
}

/*
*               ETAP PIEWRWSZY
        ///     PRZESZEDL PIERWSZY TEST KIERUNKOWY      ///

    std::vector<double*> wejscie, wyjscie;

    for (size_t i = 0; i < 5; i++)
    {
        wejscie.push_back(new double(i + 2.0));
    }

    for (size_t i = 0; i < 3; i++)
    {
        wyjscie.push_back(new double);
    }

    NeuralNetwork siec;

    siec.createLayersOfNeurons(wejscie, wyjscie, 1, 2);

    siec.process();

    for (double* d : wejscie)
    {
        std::cout << "Wejscie " << *d << std::endl;
    }
    std::cout<<std::endl;

    for (double* w : wyjscie)
    {
        std::cout << "Wyjscia " << *w << std::endl;
    }

*/


/*      ETAP DRUGI      -----------         WDROZYC BACKPROPAGATION!!!!         */        
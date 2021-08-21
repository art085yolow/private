#include <iostream>
#include <vector>

#include "Neuralnetwork.h"


int main()
{
    std::cout << "Hello World!\n";
   
   

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
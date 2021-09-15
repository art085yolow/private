#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>

#include "include/Neuralnetwork.h"
#include "include/render/WindowNet.h"
#include "src/utils.cpp"
#include "include/DataStream.h"

GLOBAL_VARIABLE bool running = true;

void test1();
void test2();
void test3();

int main()
{
    std::cout << "Hello World!\n";
   
    WindowNet* progWindow = new WindowNet();

    
    test3();    /// refactoring code if needed to run with window /// option 1 : create a test3 as a <!> class <!> and what to have
   // DataStream trainImages("testData/train-images.idx3-ubyte", "testData/train-labels.idx1-ubyte");
    DataStream test("testData/t10k-images.idx3-ubyte", "testData/t10k-labels.idx1-ubyte");

    std::vector<Image*> pictures = test.getListOfImages();
    std::vector<unsigned int> picture = test.getListOfImages()[0]->getColor();


    progWindow->pushImage(pictures);

    while(running)
    {

        if (!progWindow->ProcessMsg())
        {
            std::cout << "Closing Window\n";
            running = false;
        }

        // Render
        
        progWindow->render();

        Sleep(10); // for now to not overstress cpu
    }



    return 0;
}

void test1()
{
/*               ETAP PIEWRWSZY -- STAGE ONE
        ///             FEEDFORWARD                ///
 */
    std::vector<double*> input, output;

    for (size_t i = 0; i < 5; i++)
    {
        input.push_back(new double(i + 2.0));
    }

    for (size_t i = 0; i < 3; i++)
    {
        output.push_back(new double);
    }

    NeuralNetwork net;

    net.createLayersOfNeurons(input, output, 1, 2);

    net.process();

    for (double* d : input)
    {
        std::cout << "Wejscie - input " << *d << std::endl;
    }
    std::cout<<std::endl;

    for (double* w : output)
    {
        std::cout << "Wyjscia - output " << *w << std::endl;
    }

}

/*      ETAP DRUGI      -----------         WDROZYC BACKPROPAGATION!!!!         */        
void test2()
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

    std::vector<double> yOut;
    yOut.push_back(double(0.01));
    yOut.push_back(double(0.99));

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

void test3()
{
    // prepering data for test //
    // target MNIST //

    /// train/test images data file
    /// 4 * sizeof(integer) - magic number(2051), number of images(60 000 or 10 000), number of rows(28), number of columns(28) // size_file = rows * columns * number of images
    /// offset 4*sizeof(integer) - unsigned byte (pixel value 0-255 : 0 - white(empty), 255 - black(full)  //  value / 255 = double result 0.0 - 1.0
    /// 
    /// train/test label data file
    /// 2 * sizeof(integer) - magic number(2049), number of labels(60 000 or 10 000) // size_file = number of labels
    /// offset 2 * sizeof(integer) - unsigned byte (value label = name)

    /*
    *   1. read data file - byte stream
    *   2. put to storage
    *   3. prepaire net inputs (28x28 pixels = 784 inputs, 2 hidden layer per 16 neurons, 10 outputs - accordingly index 0 is number 0 (zero))
    *   4. create appropriate places or classes to handle test no. 3 - simplify the reading of the results
    *   5. create window for graphs/images/ui -- still todo - what date needed
    */

    /// new image - data.sendImage(); -> load to inputs network -> processing -> calculating target results - output results -> backprop -> repit process
    /// test 10 image -> calculate sum error for each neuron -> backprop -> repit

    /// class data for stream
    /// - integer magicNumber
    /// - integer numbersOfImage
    /// - integer rows
    /// - integer columns
    /// - unsigned char* images
    /// 
    /*
    int color;
    unsigned char rgb;
    rgb = 34;
    color = rgb;
    std::cout << color << std::endl; //34
    rgb = 255;
    color += rgb << 8;
    std::cout << color << std::endl; // 65314
    */

}
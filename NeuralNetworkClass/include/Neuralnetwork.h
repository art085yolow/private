#pragma once
#include <vector>
#include "SAEnum.h"
#include "Layer.h"


	
	class NeuralNetwork
	{
	public:
		NeuralNetwork();


		/*		Simple creator		----	automatyczne dopasowanie ilosci wejsc i wyjsc(<vector>inputs, <vector>outputs), liczba warstw ukrytych, liczba neuronow w warstwach ukrytych, (enum) typ sumowania(domyslna SUMA), (enum) typ aktywacji(domyslna SIGM)		//
									----	automatic adjustment of the number of inputs and outputs (<vector> inputs, <vector> outputs), number of hidden layers, number of neurons in hidden layers, (enum) type of summation (default SUM), (enum) activation type (default SIGM)
		*/
		void createLayersOfNeurons(std::vector<double*> inputs, std::vector<double*> outputs, unsigned int nrLayers = 1, unsigned int nrNeuronsInLayer = 1, SummationEnum typSum=SummationEnum::Suma, ActivationEnum typActiv=ActivationEnum::Sigm);
		
		// TODO
		void createLayersOfNeurons(unsigned int nrInputs = 1, unsigned int nrOutputs = 1, unsigned int nrLayers = 1, unsigned int nrNeuronsInLayer = 1, SummationEnum typSum=SummationEnum::Suma, ActivationEnum typActiv=ActivationEnum::Sigm);

		// TODO  -- refactor
		void setInputsOutputs(std::vector<double*> inputs, std::vector<double*> outputs);
		
		// feedForward
		void process();


		// rename/refactor/TODO
		void calculateTotalError();
		
		// NET Error
		void setError();
		
		// required target //--refactoring
		void setYPredicted(std::vector<double> testOut);
		
		// -- TODO	-- refactor
		void backProb(); 
		
		// leearning ratio -- recommended 0.1 - 0.01
		void setRatio(double ratio);

		// get NET Error
		double getError();

		std::vector<Layer*> NeuralNetworkLayers; // first layer -- input / last layer -- output
	private:

		double ratioLearn = 0.05;
		double totalError = 0.0;
		
		// error for each neuron in layer
		std::vector<double*> errors;
		
		// expected targets
		std::vector<double> y;


	};

#pragma once
#include <vector>
#include <iostream>
#include "SAEnum.h"
#include <string>
#include "rebuild/BitDataMemo.h"


	
	class NeuralNetwork
	{
	public:
		NeuralNetwork(std::vector<double>& inputs, std::vector<double>& outputs, unsigned int nrLayers = 1, unsigned int nrNeuronsInLayer = 1);
		~NeuralNetwork();


		/*		Simple creator		----	automatyczne dopasowanie ilosci wejsc i wyjsc(<vector>inputs, <vector>outputs), liczba warstw ukrytych, liczba neuronow w warstwach ukrytych, (enum) typ sumowania(domyslna SUMA), (enum) typ aktywacji(domyslna SIGM)		//
									----	automatic adjustment of the number of inputs and outputs (<vector> inputs, <vector> outputs), number of hidden layers, number of neurons in hidden layers, (enum) type of summation (default SUM), (enum) activation type (default SIGM)
		*/
		void createLayersOfNeurons(std::vector<double> &inputs, std::vector<double> &outputs, std::vector<unsigned int> hiddenLayersWithNumNerouns);
		
		// TODO
		void createLayersOfNeurons(unsigned int nrInputs = 1, unsigned int nrOutputs = 1, unsigned int nrLayers = 1, unsigned int nrNeuronsInLayer = 1, SummationEnum typSum=SummationEnum::Suma, ActivationEnum typActiv=ActivationEnum::Sigm);
		void createNetwork();

		// TODO  -- refactor
		void setInputsOutputs(std::vector<double>& inputs, std::vector<double> &outputs);
		
		// feedForward  -- TODO P1
		void process();

		// randomize function - 0 - default for bias and weight // - 1 - for bias // - 2 - for synaps
		void randomize(unsigned int option = 0);

		// rename/refactor/TODO
		void calculateTotalError();
		
		// NET Error --- delete or rebuild
		void setError();
		
		// required target //--refactoring
		void setYPredicted(std::vector<double>& testOut);
		
		// -- TODO	-- refactor
		void backProb(); 
		
		// learning ratio -- recommended 0.1 - 0.01
		void setRatio(double ratio);

		// get NET Error
		double getNetError();


		
	private:
		// vec inputs
		std::vector<double> *inputsLayerNetwork = nullptr;  // -> & ?
		// vec outputs
		std::vector<double> *outputsLayerNetwork = nullptr;

		// vec
		std::vector<double> vecNeurons;
		std::vector<double> vecBias;
		std::vector<double> vecSynapse;
		
		std::vector<unsigned int> numberNeuronsInEachLayers;

		// layer		neuron
		std::vector<std::vector<double*>> networkNeuronsLayer;
		std::vector<std::vector<double*>> networkNeuronsBiasesLayer;
		
		// layer		neuron		synapse
		std::vector<std::vector<std::vector<double*>>> networkSynapsesLayer;

		BitDataMemo weightsSynapses;
		BitDataMemo neuronsBiases;

		double ratioLearn = 0.05;
		double totalError = 0.0;
		
		// error for each neuron in layer
		std::vector<double> errors;
		
		// expected targets
		std::vector<double> *y = nullptr;
		

	};

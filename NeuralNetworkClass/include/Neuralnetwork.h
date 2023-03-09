#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include "SAEnum.h"
#include <string>
#include "rebuild/BitDataMemo.h"
#include <map>



struct Synapse
{
	unsigned int id_number;

	unsigned int idNeuronIn = 0;
	unsigned int idNeuronOut = 0;

	double weight = 0.0;

	unsigned int getIdNeuronIn();
	double getValue();
};


struct Neuron
{
	unsigned int id_number;

	std::vector<unsigned int> idSynapsesListIn;
	//std::vector<unsigned int> idListOut; -- move that to doffrent section, backprob

	double axon = 0.0;
	double bias = 0.0;

	double getValue();
};

	class NeuralNetwork
	{
	public:
		NeuralNetwork(std::vector<double>& inputs, std::vector<double>& outputs, unsigned int nrLayers = 1, unsigned int nrNeuronsInLayer = 1);
		~NeuralNetwork();

		NeuralNetwork(std::string s); // s contain: "numbers of inputs" for [input, hidden, output] neurons, min 2 [in, out] numbers - exp: "1, 3, 4" | "1, 4,"| "9, 10, 2, 36, 100"

				
		// TODO
		void createNetwork();

		// TODO  -- refactor
		void getInputs(std::vector<std::reference_wrapper<double>>& in);
		void getOutputs(std::vector<std::reference_wrapper<double>>& out);
		
		// feedForward  -- TODO P1
		void process();

		// randomize function - 0 - default for bias and weight // - 1 - for bias // - 2 - for synaps
		double randomize();

		// rename/refactor/TODO
		void calculateTotalError();
		
		// NET Error range - set
		void setErrorRange(double val);
		
		// required target //--refactoring
		std::vector<double>& getYrefList();
		
		// -- TODO	-- refactor
		void backProb(); 
		
		// learning ratio -- recommended 0.1 - 0.01
		void setRatio(double ratio);

		// get NET Error
		double getNetError();

		// print neurons and synapses status
		void print();

		
	private:
		// vec network
		std::vector<Neuron> neuron_list_Id;
		std::vector<Synapse> synapse_list_Id;
		
		std::vector<unsigned int> numberNeuronsInEachLayers;


//		BitDataMemo weightsSynapses;
//		BitDataMemo neuronsBiases;

		double ratioLearn = 0.05;
		double networkError = 0.1;

		// network output error
		double totalErrorOutput = 0.0;
		
		// error for each neuron in layer
		std::vector<double> errorsBias;
		std::vector<double> errorsSynapse;
		
		// expected targets
		std::vector<double> y;
		

	};

#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include "SAEnum.h"
#include <string>
#include <map>
#include <regex>



struct Synapse
{
	unsigned int id_number;

	unsigned int idNeuronIn = 0;
	unsigned int idNeuronOut = 0;

	double weight = 0.0;

};


struct Neuron
{
	unsigned int id_number;

	double axon = 0.0;
	double bias = 0.0;
	
	std::vector<unsigned int> idSynapsesListIn;

};

	class NeuralNetwork
	{
	public:
		NeuralNetwork(std::string s); // s contain: "numbers of inputs" for [input-hidden-output] neurons, min 2 [in, out] numbers - exp: "1-3-4" | "1-4,"| "9-10-2-36-100"
		~NeuralNetwork();

		// creating new network or reading from file
		void create_network();
				
		size_t get_input_size();
		size_t get_output_size();
		
		// feedForward
		void process();

		// randomize function
		double randomize();

		
		// NET Error range - set
		void setErrorRange(double val);
		
		// backProb - apply changes from error list
		void backProb(std::vector<double> neuronErr, std::vector<double> synapseErr); 
		// calculating error for each neuron/synaps - one step
		void calculateNetErr(std::vector<double>& neuronErr, std::vector<double>& synapseErr, std::vector<double>& y);

		
		// learning ratio -- recommended 0.1 - 0.01
		void setRatio(double ratio);

		// get NETwork Error
		double getNetError(std::vector<double>& y);

		// print neurons and synapses status
		void printAll();
		void printInputs();
		void printOutputs();

		// name file corestond to network build // exampl: Network("2-2-2") => file name "2-2-2.nnc"
		// save/load network from file
		// network to save
		bool save_network();
		
		double& operator[](std::string in_out_neuron);
	private:
		// name is structure of network
		std::string name;
		// get numberNeuronsInEachLayers in string
		std::string get_network_structure();
		
		
		// network to load in from file name
		bool load_network(std::string name);


		// vec network
		std::vector<Neuron> neuron_list_Id;
		std::vector<Synapse> synapse_list_Id;
		
		// size number for each layer
		std::vector<unsigned int> numberNeuronsInEachLayers;
		unsigned int first_id_from_last_layer;

		double ratioLearn = 0.05;
		double networkError = 0.1;

		// network output error
		double totalErrorOutput = 0.0;
		
		// getting id for first output neuron
		unsigned int first_id_from_output_layer();
		
		// calculating Network error
		void calculateTotalError(std::vector<double>& y);

		// testing if string contain only numbers and separator "-"
		bool test_string(std::string s);

		
	};

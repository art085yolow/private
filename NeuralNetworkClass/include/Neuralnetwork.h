#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>
#include <map>
#include <regex>



struct Synapse
{
	unsigned int m_id_number;

	unsigned int m_id_neuron_In = 0;
	unsigned int m_id_neuron_Out = 0;

	double m_weight = 0.0;

};


struct Neuron
{
	unsigned int m_id_number;

	double m_axon = 0.0;
	double m_bias = 0.0;
	
	std::vector<unsigned int> m_id_synapses_list_In;

};

	class NeuralNetwork
	{
	public:
		// name file corresponding to network build // exampl: Network("2-2-2") => file name "2-2-2.nnc"
		// s contain: "numbers of inputs" for [input-hidden-output] neurons, min 2 [in, out] numbers - exp: "1-3-4" | "1-4,"| "9-10-2-36-100"
		NeuralNetwork(std::string _s); 
		~NeuralNetwork();

		// creating new network or reading from file
		void create_network();
		
		// size for input/output layer
		size_t get_input_size();
		size_t get_output_size();
		
		// feedForward
		void process();

		// randomize function
		double random_double();
		
		// backProb - apply changes from error list
		void backpropagation(std::vector<double> _neuronErr, std::vector<double> _synapseErr); 
		
		// calculating error for each neuron/synaps - one step
		void calculate_network_error(std::vector<double>& _neuronErr, std::vector<double>& _synapseErr, std::vector<double>& _y);
		
		// learning ratio -- recommended 0.1 - 0.01
		void set_learning_ratio(double _ratio);

		// get NETwork Error
		double get_network_error(std::vector<double>& _y);

		// print neurons and synapses status
		void print_All();
		void print_Inputs();
		void print_Outputs();

		// save/load network from file
		// network to save
		bool save_network();
		// network to load in from file name
		bool load_network(std::string _name);
		
		double& operator[](std::string _in_out_neuron);
	private:
		// name is structure of network
		std::string m_name;
		// get numberNeuronsInEachLayers in string
		std::string get_network_structure();

		// vec network
		std::vector<Neuron> m_neuron_list_Id;
		std::vector<Synapse> m_synapse_list_Id;
		
		// size number for each layer
		std::vector<unsigned int> m_number_neurons_in_each_layers;
		unsigned int m_first_id_from_last_layer;

		double m_learn_ratio = 0.05;
		
		// getting id for first output neuron
		unsigned int first_id_from_output_layer();

		// testing if string contain only numbers and separator "-"
		bool test_string(std::string _s);

		
	};

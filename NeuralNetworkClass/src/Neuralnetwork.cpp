﻿#include "../include/Neuralnetwork.h"


NeuralNetwork::~NeuralNetwork()
{
	// delete this->inputsLayerNetwork;
	// delete this->outputsLayerNetwork;
	// delete this->y;

	// correcily delete pointer list
	// prawidlowo usunac listy var*
}

void NeuralNetwork::create_network()
{

	std::cout << " - - - - - NETWORK CREATING - - - - -\n" << std::endl;
	
	
	// przypisac IDneurons in/out do synaps

	std::vector<unsigned int> theConnection;
	for (unsigned int l = 0, s = 0; l < numberNeuronsInEachLayers.size(); l++)
	{
		if (l > 0)
		{
			// ID started on each layer
			theConnection.push_back(numberNeuronsInEachLayers[l - 1] + theConnection[l - 1]);


			for (unsigned int n = 0; n < numberNeuronsInEachLayers[l]; n++)
			{
				unsigned int n_l_id = n + theConnection[l];

				// generating neurons
				Neuron neuronG;
				neuronG.id_number = n_l_id;

				for (unsigned int n_l_prev = 0; n_l_prev < numberNeuronsInEachLayers[l - 1]; n_l_prev++, s++)
				{
					Synapse syn;

					unsigned int n_l_prev_id = n_l_prev + theConnection[l - 1];

					syn.id_number = s;

					//assigne IDs in/out and random weight
					syn.idNeuronIn = n_l_prev_id;
					syn.idNeuronOut = n_l_id;
					syn.weight = randomize();

					synapse_list_Id.push_back(syn);

					// assigne synapseID to neuron list
					neuronG.idSynapsesListIn.push_back(syn.id_number);
				}

				neuronG.bias = randomize();
				neuron_list_Id.push_back(neuronG);
			}
		}
		else
		{
			// first ID started in input/first layer
			theConnection.push_back(l);

			// generating neurons for fist layer
			for (unsigned int n = 0; n < numberNeuronsInEachLayers[l]; n++)
			{

				Neuron in;
				// giving ID
				in.id_number = n;
				neuron_list_Id.push_back(in);
			}
		}
	};
}

NeuralNetwork::NeuralNetwork(std::string s)
{
	srand((unsigned)time(NULL));
	this->name = s;

	// making list from string
	std::istringstream input{ s };
	for (std::string line; std::getline(input, line, '-');)
	{
		numberNeuronsInEachLayers.push_back(std::stoi(line));
	};


	// if file not exist create new network
	if (!load_network(s))
	{
		create_network();
	}

	//print();

}

void NeuralNetwork::process()
{
	// -----------------------------------------------------------TODO -------------------------------------------------------------------------------
	
	// check -> del
		std::cout << "\n - - - - - PROCESSING - - - - -\n" << std::endl;

	for (auto& n:neuron_list_Id)
	{
		if (!n.idSynapsesListIn.empty())
		{
			double sum = n.bias;

			for (auto s_id : n.idSynapsesListIn)
			{

				sum += synapse_list_Id[s_id].weight * neuron_list_Id[synapse_list_Id[s_id].idNeuronIn].axon;

			}

			n.axon = 1 / (1 + exp(-sum));
		}

	}
	
	//print();
	
	
}

double NeuralNetwork::randomize()
{
	//srand((unsigned)time(NULL));

	return ( 1 - ((double)rand() / (RAND_MAX / 2))); // random number beetwen -1 and 1.
}


void NeuralNetwork::backProb(std::vector<double> neuronErr, std::vector<double> synapseErr)
{
	std::cout << "\n BACKPROB: \n - - - - - PROCESSING - - - - -\n";
	if (this->neuron_list_Id.size() == neuronErr.size() && this->synapse_list_Id.size() == synapseErr.size())
	{
		for (size_t it = 0; it < synapse_list_Id.size(); it++)
		{
			synapse_list_Id[it].weight -= this->ratioLearn * synapseErr[it];

			if (it < neuron_list_Id.size() )
			{
				neuron_list_Id[it].bias -= this->ratioLearn * neuronErr[it];
			}

		}

	}
	else
	{
		std::cout << "\n BACKPROB: vectors are diffrent \n PROCESS TERMINATED \n";
	}



}

void NeuralNetwork::calculateNetErr(std::vector<double>& neuronErr, std::vector<double>& synapseErr, std::vector<double>& y)
{

	if(this->neuron_list_Id.size() != neuronErr.size()) 
	{
	
		neuronErr.resize(this->neuron_list_Id.size());

	}

	if (this->synapse_list_Id.size() != synapseErr.size())
	{

		synapseErr.resize(this->synapse_list_Id.size());

	}
	// first calculate Error output from Y prediction
	for (unsigned int it=0, outputIt = first_id_from_output_layer() + it; it < y.size(); it++, outputIt++)
	{
		neuronErr[outputIt] += (-(y[it] - neuron_list_Id[outputIt].axon));
	}

	// calculating Error for neural net
	for (unsigned int it = this->neuron_list_Id.size()-1; it > this->numberNeuronsInEachLayers[0]; it--)
	{
					// error for current neuron multiply by revers calculatet neuron axon
		double partialErr = neuronErr[it] * (neuron_list_Id[it].axon * (1 - neuron_list_Id[it].axon));

		// first step - synaps error connected with itereted neuron
		for (unsigned int synapsIt = 0; synapsIt < neuron_list_Id[it].idSynapsesListIn.size(); synapsIt++)
		{
			unsigned int currentSynapseID = neuron_list_Id[it].idSynapsesListIn[synapsIt];
			// current synaps error
			synapseErr[currentSynapseID] += partialErr * neuron_list_Id[ synapse_list_Id[ currentSynapseID].idNeuronIn].axon;
			
			// error neuron connected with synapse
			neuronErr[synapse_list_Id[currentSynapseID].idNeuronIn] += partialErr * synapse_list_Id[currentSynapseID].weight;
		}


	}

}

unsigned int NeuralNetwork::first_id_from_output_layer()
{
	unsigned int ret = 0;
	for (size_t i = 0; i < numberNeuronsInEachLayers.size() - 1; i++)
	{
		ret+= numberNeuronsInEachLayers[i];
	}
	return ret;
}

void NeuralNetwork::setRatio(double ratio)
{
	this->ratioLearn = ratio;
}

double NeuralNetwork::getNetError(std::vector<double>& y)
{
	calculateTotalError(y);
	return this->totalErrorOutput;
}

void NeuralNetwork::printAll()
{
	for (auto& n : neuron_list_Id)
	{
		std::cout << "id N: " << n.id_number << " axon N: " << n.axon << " bias N: " << n.bias << " SynapseListSize N: \n - - - - - \n";
		for (auto& si : n.idSynapsesListIn)
		{
			std::cout << "id S: " << synapse_list_Id[si].id_number << " weight S: " << synapse_list_Id[si].weight << " idIn S: " << synapse_list_Id[si].idNeuronIn << " idOut S: " << synapse_list_Id[si].idNeuronOut << std::endl;
		}
		std::cout << " - - - - - \n" << std::endl;
	}
}

void NeuralNetwork::printInputs()
{
	std::cout << "\n - - - - - - - - - - - - - - - \n           INPUTS\n" << std::endl;
	
	for (size_t i = 0; i < this->numberNeuronsInEachLayers[0]; i++)
	{
		std::cout << "\n Neuron " << i << " : " << this->neuron_list_Id[i].axon;
	}

	std::cout << "\n - - - - - - - - - - - - - - - \n" << std::endl;
}

void NeuralNetwork::printOutputs()
{
	std::cout << "\n - - - - - - - - - - - - - - - \n          OUTPUTS\n" << std::endl;

	for (size_t i = this->first_id_from_output_layer(); i < this->neuron_list_Id.size(); i++)
	{
		std::cout << "\n Neuron " << i << " : " << this->neuron_list_Id[i].axon;
	}

	std::cout << "\n - - - - - - - - - - - - - - - \n" << std::endl;
}

bool NeuralNetwork::save_network()
{
	std::string filename = "data/" + this->get_network_structure() + ".nnc";


	std::ofstream write_file(filename, std::ios::binary);

	if (!write_file.is_open()) return false;

	// first save neuron vector size
	size_t vector_size = this->neuron_list_Id.size();
	write_file.write((char*)&vector_size, sizeof(vector_size));
	// save all neurons from vector
	for (Neuron& iter:this->neuron_list_Id)
	{
		write_file.write((char*)&iter.id_number, sizeof(iter.id_number));
		write_file.write((char*)&iter.axon, sizeof(iter.axon));
		write_file.write((char*)&iter.bias, sizeof(iter.bias));

		unsigned int size_vec = iter.idSynapsesListIn.size();
		write_file.write((char*)&size_vec, sizeof(size_vec));

		for (auto& i:iter.idSynapsesListIn)
		{
		write_file.write((char*)&i, sizeof(i));

		}

	}

	// save synapse vector size
	vector_size = this->synapse_list_Id.size();
	write_file.write((char*)&vector_size, sizeof(vector_size));
	// save all synapse from vector
	for (Synapse& iter : this->synapse_list_Id)
	{
		write_file.write((char*)&iter.id_number, sizeof(iter.id_number));
		write_file.write((char*)&iter.weight, sizeof(iter.weight));
		write_file.write((char*)&iter.idNeuronIn, sizeof(iter.idNeuronIn));
		write_file.write((char*)&iter.idNeuronOut, sizeof(iter.idNeuronOut));
	}

	write_file.close();

	return true;
}

bool NeuralNetwork::load_network(std::string name)
{
	this->neuron_list_Id.clear();
	this->synapse_list_Id.clear();

	std::string filename = "data/" + name + ".nnc";

	std::ifstream open_file(filename, std::ios::in | std::ios::binary);

	if (!open_file.is_open()) return false;

	size_t vector_size;
	open_file.read((char*)&vector_size, sizeof(vector_size));

	for (size_t iter = 0; iter < vector_size; iter++)
	{
		Neuron new_neuron;
		unsigned int new_int;

		open_file.read((char*)&new_neuron.id_number, sizeof(new_neuron.id_number));

		open_file.read((char*)&new_neuron.axon, sizeof(new_neuron.axon));

		open_file.read((char*)&new_neuron.bias, sizeof(new_neuron.bias));

		open_file.read((char*)&new_int, sizeof(new_int));
		for (size_t i = 0, list=new_int; i < list; i++)
		{
			open_file.read((char*)&new_int, sizeof(new_int));
			new_neuron.idSynapsesListIn.push_back(new_int);
		}

		this->neuron_list_Id.push_back(new_neuron);
	}

	open_file.read((char*)&vector_size, sizeof(vector_size));

	for (size_t i = 0; i < vector_size; i++)
	{
		Synapse new_synapse;

		open_file.read((char*)&new_synapse.id_number, sizeof(new_synapse.id_number));
		open_file.read((char*)&new_synapse.weight, sizeof(new_synapse.weight));
		open_file.read((char*)&new_synapse.idNeuronIn, sizeof(new_synapse.idNeuronIn));
		open_file.read((char*)&new_synapse.idNeuronOut, sizeof(new_synapse.idNeuronOut));
		
		this->synapse_list_Id.push_back(new_synapse);
	}

	open_file.close();

	return true;
}

std::string NeuralNetwork::get_network_structure()
{
	std::string ret;

	for (size_t i=0, last=this->numberNeuronsInEachLayers.size()-1; i < this->numberNeuronsInEachLayers.size();i++)
	{
		ret += std::to_string(this->numberNeuronsInEachLayers[i]);

		if (i < last) ret += "-";
	}

	return ret;
}


void NeuralNetwork::calculateTotalError(std::vector<double>& y)
{
//	std::cout << "\n - - - - - - - - - - - - - - - \n" << std::endl;
//	std::cout << "     NET ERROR CALCULATION     \n" << std::endl;
	
	// network error output
	 this->totalErrorOutput = 0.0;
	 
	 std::vector<std::reference_wrapper<double>> out;
	 this->getOutputs(out);
	 if (out.size() == y.size())
	 {

		 for (size_t i = 0; i < out.size(); i++)
		 {
			 double errorOut;
			 errorOut = 0.5 * ((y[i] - (out[i].get())) * (y[i] - (out[i].get())));

			 this->totalErrorOutput += errorOut;
		 }
	 }
	
	 
//	std::cout << "\n - - - - - - - - - - - - - - - \n" << std::endl;
};

void NeuralNetwork::setErrorRange(double val)
{ 
	
	this->networkError = val;
	
}


void NeuralNetwork::getInputs(std::vector<std::reference_wrapper<double>>& in)
{
	
	for (size_t i = 0; i < numberNeuronsInEachLayers[0]; i++)
	{
		in.push_back(neuron_list_Id[i].axon);
	}
}


void NeuralNetwork::getOutputs(std::vector<std::reference_wrapper<double>>& out)
{
	unsigned int id_neuron_from = first_id_from_output_layer();
	
	for (size_t i = id_neuron_from; i < neuron_list_Id.size(); i++)
	{
		out.push_back(neuron_list_Id[i].axon);
	}
}


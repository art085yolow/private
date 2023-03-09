#include "../include/Neuralnetwork.h"



NeuralNetwork::NeuralNetwork(std::vector<double>& inputs, std::vector<double>& outputs, unsigned int nrLayers, unsigned int nrNeuronsInLayer)
{
	/*
	this->inputsLayerNetwork = &inputs;
	this->outputsLayerNetwork = &outputs;


	// fisrt layer
	this->numberNeuronsInEachLayers.push_back(inputs.size());

	// hidden layer
	for (size_t idx = 0; idx < nrLayers; idx++)
	{
		this->numberNeuronsInEachLayers.push_back(nrNeuronsInLayer);
	}

	//last layer
	this->numberNeuronsInEachLayers.push_back(outputs.size());


	//randomize();

	/// from here set the network or call for function for that
	createNetwork();
	*/
}

NeuralNetwork::~NeuralNetwork()
{
	// delete this->inputsLayerNetwork;
	// delete this->outputsLayerNetwork;
	// delete this->y;

	// correcily delete pointer list
	// prawidlowo usunac listy var*
}

NeuralNetwork::NeuralNetwork(std::string s)
{
	srand((unsigned)time(NULL));


	// making list from string
	std::istringstream input{ s };
	
	// check -> del
	std::cout << " - - - - - NETWORK CREATING - - - - -\n" << std::endl;


	for (std::string line; std::getline(input, line, ',');)
	{
		numberNeuronsInEachLayers.push_back(std::stoi(line));
	};
		
		// przypisac IDneurons in/out do synaps
	
	std::vector<unsigned int> theConnection;
	for (size_t l = 0, s = 0; l < numberNeuronsInEachLayers.size(); l++)
	{
		if (l > 0)
		{
			// ID started on each layer
			theConnection.push_back(numberNeuronsInEachLayers[l - 1] + theConnection[l - 1]);


			for (size_t n = 0; n < numberNeuronsInEachLayers[l]; n++)
			{
				unsigned int n_l_id = n + theConnection[l];

				// generating neurons
				Neuron neuronG;
				neuronG.id_number = n_l_id;

				for (size_t n_l_prev = 0; n_l_prev < numberNeuronsInEachLayers[l - 1]; n_l_prev++, s++)
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
			for (size_t n = 0; n < numberNeuronsInEachLayers[l]; n++)
			{

				Neuron in;
				// giving ID
				in.id_number = n;
				neuron_list_Id.push_back(in);
			}
		}
	};

	//print();

}

void NeuralNetwork::process()
{
	// -----------------------------------------------------------TODO -------------------------------------------------------------------------------
	
	// check -> del
		std::cout << " - - - - - PROCESSING - - - - -\n" << std::endl;

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


void NeuralNetwork::backProb()
{			///			---			REFACTOR THE HOLY PROCESS OF CALCULATION			---				///
			// main problem is the size of 'error' list from last layer to next //
			// (784x16x16x10) it is calculating just 10 times but not for every weights!! //



	std::vector<double> revErrBias;

	for (size_t i = 0; i < this->errorsBias.size(); i++)
	{
		revErrBias.push_back(errorsBias[i]);
	}

	revErrBias.clear();

	for (size_t i = 0; this->errorsSynapse.size(); i++)
	{
		revErrBias.push_back(this->errorsSynapse[i]);
	}

	revErrBias.clear();

}

void NeuralNetwork::setRatio(double ratio)
{
	this->ratioLearn = ratio;
}

double NeuralNetwork::getNetError()
{
	calculateTotalError();
	return this->totalErrorOutput;
}

void NeuralNetwork::print()
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


void NeuralNetwork::calculateTotalError()
{
	
	// network error output
	 this->totalErrorOutput = 0.0;
	 
	 std::vector<std::reference_wrapper<double>> out;
	 this->getOutputs(out);
	 if (out.size() == y.size())
	 {

		 for (size_t i = 0; i < out.size(); i++)
		 {
			 double errorOut;
			 errorOut = 0.5 * (y[i] - (out[i].get()) * (y[i] - (out[i].get())));

			 this->totalErrorOutput += errorOut;
		 }
	 }
	
	 
};

void NeuralNetwork::setErrorRange(double val)
{ 
	
	this->networkError = val;
	
}

std::vector<double>& NeuralNetwork::getYrefList()
{
	return this->y;
}


void NeuralNetwork::getInputs(std::vector<std::reference_wrapper<double>>& in)
{
	
	for (size_t i = 0; i < numberNeuronsInEachLayers[0]; i++)
	{
		in.push_back(neuron_list_Id[i].axon);
	}
	// remeber to get value by [- '.get()' -]
}


void NeuralNetwork::getOutputs(std::vector<std::reference_wrapper<double>>& out)
{
	unsigned int id_neuron_from = 0;
	for (size_t i = 0; i < numberNeuronsInEachLayers.size()-2; i++)
	{
		id_neuron_from += numberNeuronsInEachLayers[i];
	}
	
	for (size_t i = id_neuron_from; i < neuron_list_Id.size()-1; i++)
	{
		out.push_back(neuron_list_Id[i].axon);
	}
}


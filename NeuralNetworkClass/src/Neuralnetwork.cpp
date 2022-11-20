#include "../include/Neuralnetwork.h"


void NeuralNetwork::createLayersOfNeurons(std::vector<double> &inputs, std::vector<double> &outputs, unsigned int nrLayers, unsigned int nrNeuronsInLayer)
{
	this->inputsLayerNetwork = &inputs;
	this->outputsLayerNetwork = &outputs;

	// build vec of neurons
	for (size_t idx = 0, totalSumOfNeurons = nrLayers * nrNeuronsInLayer + static_cast<unsigned int>(inputs.size() + outputs.size()); idx < totalSumOfNeurons; idx++)
	{
		vecNeurons.push_back(double(0.0));
		// build vec of Biases
		vecBias.push_back(double(0.0));
	}

	// fisrt layer
	this->numberNeuronsInEachLayers.push_back(inputs.size());

	// hidden layer
	for (size_t idx = 0; idx < nrLayers; idx++)
	{
		this->numberNeuronsInEachLayers.push_back(nrNeuronsInLayer);
	}

	//last layer
	this->numberNeuronsInEachLayers.push_back(outputs.size());

	for (size_t idn = 1; idn < this->numberNeuronsInEachLayers.size(); idn++)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>((this->numberNeuronsInEachLayers[idn - 1]) * (this->numberNeuronsInEachLayers[idn])); i++)
		{
			vecSynapse.push_back(double(0.0));
		}
	}

	randomize();
	
}


NeuralNetwork::NeuralNetwork(std::vector<double>& inputs, std::vector<double>& outputs, unsigned int nrLayers, unsigned int nrNeuronsInLayer)
{
	this->inputsLayerNetwork = &inputs;
	this->outputsLayerNetwork = &outputs;

	// build vec of neurons
	for (size_t idx = 0, totalSumOfNeurons = nrLayers * nrNeuronsInLayer + static_cast<unsigned int>(inputs.size() + outputs.size()); idx < totalSumOfNeurons; idx++)
	{
		vecNeurons.push_back(double(0.0));
		// build vec of Biases
		vecBias.push_back(double(0.0));
	}

	// fisrt layer
	this->numberNeuronsInEachLayers.push_back(inputs.size());

	// hidden layer
	for (size_t idx = 0; idx < nrLayers; idx++)
	{
		this->numberNeuronsInEachLayers.push_back(nrNeuronsInLayer);
	}

	//last layer
	this->numberNeuronsInEachLayers.push_back(outputs.size());

	for (size_t idn = 1; idn < this->numberNeuronsInEachLayers.size(); idn++)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>((this->numberNeuronsInEachLayers[idn - 1]) * (this->numberNeuronsInEachLayers[idn])); i++)
		{
			vecSynapse.push_back(double(0.0));
		}
	}

	randomize();

	/// from here set the network or call for function for that
	createNetwork();

}

NeuralNetwork::~NeuralNetwork()
{
	// delete this->inputsLayerNetwork;
	// delete this->outputsLayerNetwork;
	// delete this->y;

	// correcily delete pointer list
	// prawidlowo usunac listy var*
}

void NeuralNetwork::process()
{
	
	
	// -----------------------------------------------------------TODO -------------------------------------------------------------------------------
	// --2022-06-04
	
	for (size_t x = 1; x < this->numberNeuronsInEachLayers.size(); x++)
	{
		for (size_t y = 0; y < this->numberNeuronsInEachLayers[x]; y++)
		{
			double suma = *(this->networkNeuronsBiasesLayer[x][y]);

			for (size_t z = 0; z < this->numberNeuronsInEachLayers[x - 1]; z++)
			{
				suma += (*(this->networkNeuronsLayer[x-1][z])) * (*(this->networkSynapsesLayer[x][y][z]));
			}

			*this->networkNeuronsLayer[x][y] = 1 / (1 + exp(-suma));
	}

	}
	
	
}

void NeuralNetwork::randomize(unsigned int option)
{
	if (option < 2)
	{
		// put correct loops

		// to always have random number
		srand((unsigned)time(NULL));

		// 1 - ((double)rand() / (RAND_MAX / 2)); // random number beetwen -1 and 1.

		// if is not 2, then loop for bias
		if (option != 2)
		{
			for (size_t i = this->numberNeuronsInEachLayers[0]; i < this->vecBias.size(); i++)
			{
				this->vecBias[i] = 1.0 - ((double)rand() / (RAND_MAX / 2.0));
			}
		}

		// if is not 1, then loop for synapse
		if (option != 1)
		{
			for (size_t i = 0; i < this->vecSynapse.size(); i++)
			{
				this->vecSynapse[i] = 1.0 - ((double)rand() / (RAND_MAX / 2.0));
			}
		}

	}
}

void NeuralNetwork::setYPredicted(std::vector<double> &testOut) 
{
		this->y = &testOut;
}

void NeuralNetwork::backProb()
{			///			---			REFACTOR THE HOLY PROCESS OF CALCULATION			---				///
			// main problem is the size of 'error' list from last layer to next //
			// (784x16x16x10) it is calculating just 10 times but not for every weights!! //


	// 2022-06-12 - create vec with inverted iterators -- normal vec with pushed up in front vec
	
	// layer		neuron
	std::vector<std::vector<double*>> reversNetworkNeuronsLayer;
	// std::vector<std::vector<double*>> reversNetworkNeuronsBiasesLayer;

	// layer		neuron		synapse
	std::vector<std::vector<std::vector<double*>>> reversNetworkSynapsesLayer;

	// poprawic!!!!!!!!!!!!!!!!!!!!!!!!!

	reversNetworkNeuronsLayer.resize(this->networkNeuronsLayer.size());
	// reversNetworkNeuronsBiasesLayer.resize(this->networkNeuronsBiasesLayer.size());

	reversNetworkSynapsesLayer.resize(this->networkSynapsesLayer.size());

	// uzupelnic liste

	for (size_t i = 0; i < this->networkNeuronsLayer.size(); i++)
	{
		for (size_t y = 0; y < this->networkNeuronsLayer[i].size(); y++)
		{
			reversNetworkNeuronsLayer[i].insert(reversNetworkNeuronsLayer[i].begin(), this->networkNeuronsLayer[i][y]);
			// reversNetworkNeuronsBiasesLayer[i].insert(reversNetworkNeuronsBiasesLayer[i].begin(), this->networkNeuronsBiasesLayer[i][y]);
		}
	}

	for (size_t i = 0; i < this->networkSynapsesLayer.size(); i++)
	{
		reversNetworkSynapsesLayer[i].resize(this->networkSynapsesLayer[i].size());

		for (size_t y = 0; y < this->networkSynapsesLayer[i].size(); y++)
		{
			for (size_t z = 0; z < this->networkSynapsesLayer[i][y].size(); z++)
			{
				reversNetworkSynapsesLayer[i][y].insert(reversNetworkSynapsesLayer[i][y].begin(), this->networkSynapsesLayer[i][y][z]);
			}
		}
	}


	// teraz backprob

	// potrzebuje trzy vec : 1- errNeuron 2- errBias 3- errSynapse // 2 i 3 musi przetrwac i zsumowac bledy z batchy do sredniej

	std::vector<std::vector<double>> errorNeurons;

	errorNeurons.resize(reversNetworkNeuronsLayer.size());

	// sprawdzenie czy listy sa pelne
	if (this->errorsBias.empty()) this->errorsBias.resize(this->vecBias.size());
	if (this->errorsSynapse.empty()) this->errorsSynapse.resize(this->vecSynapse.size());
	
	size_t idxBias = 0, idxSynapse = 0;

	// Faza 1 - indywidualny blad neuronow/bias/synapse z output (output jest teraz na poczatku vec)
	
	for (size_t i = 0; i < reversNetworkNeuronsLayer[0].size(); i++)
	{
		errorNeurons[0].push_back((-((*y)[i] - *reversNetworkNeuronsLayer[0][i]))*(*reversNetworkNeuronsLayer[0][i] * (1 - *reversNetworkNeuronsLayer[0][i])));
		this->errorsBias[idxBias] += this->ratioLearn * errorNeurons[0][i];
		idxBias++;
	}

	for (size_t l = 1; l < reversNetworkNeuronsLayer.size(); l++)
	{
		if (errorNeurons[l].empty())errorNeurons[l].resize(reversNetworkNeuronsLayer[l].size());
	}
		
		// Faza 2 - warstwy ukryte
	for (size_t l = 1; reversNetworkNeuronsLayer.size(); l++)
	{
		
		for (size_t i = 0; i < reversNetworkNeuronsLayer[l].size(); i++)
		{


			for (size_t y = 0; y < reversNetworkSynapsesLayer[l][i].size(); y++)
			{
				
				errorNeurons[l][i] += *reversNetworkNeuronsLayer[l - 1][y] * *reversNetworkSynapsesLayer[l][i][y];
				this->errorsSynapse[idxSynapse] = this->ratioLearn * errorNeurons[l - 1][y] * *reversNetworkSynapsesLayer[l][i][y];
				idxSynapse++;
			}

			this->errorsBias[idxBias] += this->ratioLearn * errorNeurons[l][i];
			idxBias++;

		}
	};

	std::vector<double> revErrBias;

	for (size_t i = 0; i < this->errorsBias.size(); i++)
	{
		revErrBias.push_back(errorsBias[i]);
	}

	for (size_t i = 0; i < this->vecBias.size(); i++)
	{
		this->vecBias[i] -= revErrBias[i];
	}

	revErrBias.clear();

	for (size_t i = 0; this->errorsSynapse.size(); i++)
	{
		revErrBias.push_back(this->errorsSynapse[i]);
	}

	for (size_t i = 0; i < this->vecSynapse.size(); i++)
	{
		this->vecSynapse[i] -= revErrBias[i];
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
	return this->totalError;
}


void NeuralNetwork::calculateTotalError()
{
	// network error
	 this->totalError = 0.0;
	
	for (size_t i = 0, tt = (*this->outputsLayerNetwork).size(); i < tt; i++)
	{
		double errorOut ;
		errorOut = 0.5 * (((*y)[i] - (*this->outputsLayerNetwork)[i]) * ((*y)[i] - (*this->outputsLayerNetwork)[i]));
		
		this->totalError += errorOut;
	}
	 
};

void NeuralNetwork::setError(double val)
{ 
	
	this->totalError = val;
	
}

void NeuralNetwork::createNetwork()
{
	// new

	unsigned int idxVec = 0;



	// old
	/*
	// put the vecs neuron/bias/synaps in correct layers
	// thanks to vec this->numberNeuronsInEachLayers we know how devide neurons and biases

	unsigned int idxForNaB = 0;

	unsigned int lastLayer = this->numberNeuronsInEachLayers.size() - 1;

	for (size_t idx = 0; idx < this->numberNeuronsInEachLayers.size(); idx++)
	{
		//create vec for &neurons for each layer

		// dont forgot to increase idxForNaB!!

		//if {0} input layer
		// else if{lastLayer} output Layer
		// else all hiddden
		if (idx == 0)
		{
			// loop for first layer
			std::vector<double*> firstLayerOfInputsNeurons;
			std::vector<double*> firstLayerOfInputsNeuronsBias;

			for (size_t i = 0; i < this->numberNeuronsInEachLayers[idx]; i++)
			{
				double* addres = &((*inputsLayerNetwork)[i]);
				firstLayerOfInputsNeurons.push_back(addres);
				addres = &(vecBias[idxForNaB]);
				firstLayerOfInputsNeuronsBias.push_back(addres);
				idxForNaB++;
			}

			this->networkNeuronsLayer.push_back(firstLayerOfInputsNeurons);
			this->networkNeuronsBiasesLayer.push_back(firstLayerOfInputsNeuronsBias);

		}
		else if (idx == lastLayer)
		{
			// loop for last layer
			std::vector<double*> lastLayerOfOutputsNeurons;
			std::vector<double*> lastLayerOfOutputsNeuronsBias;

			for (size_t i = 0; i < this->numberNeuronsInEachLayers[idx]; i++)
			{
				double* addres = &((*outputsLayerNetwork)[i]);
				lastLayerOfOutputsNeurons.push_back(addres);
				addres = &(vecBias[idxForNaB]);
				lastLayerOfOutputsNeuronsBias.push_back(addres);
				idxForNaB++;
			}

			this->networkNeuronsLayer.push_back(lastLayerOfOutputsNeurons);
			this->networkNeuronsBiasesLayer.push_back(lastLayerOfOutputsNeuronsBias);
		}
		else
		{
			// loop for hidden layers
			std::vector<double*> hiddenLayersOfNeurons;
			std::vector<double*> hiddenLayerOfNeuronsBias;

			for (size_t i = 0; i < this->numberNeuronsInEachLayers[idx]; i++)
			{
				double* addres = &(vecNeurons[idxForNaB]);
				hiddenLayersOfNeurons.push_back(addres);
				addres = &(vecBias[idxForNaB]);
				hiddenLayerOfNeuronsBias.push_back(addres);
				idxForNaB++;
			}

			this->networkNeuronsLayer.push_back(hiddenLayersOfNeurons);
			this->networkNeuronsBiasesLayer.push_back(hiddenLayerOfNeuronsBias);
		}

	}

	// next we devide vecsynapses in correct places
	if (this->networkSynapsesLayer.size() == 0) { this->networkSynapsesLayer.resize(this->networkNeuronsLayer.size()); }
	// correct numbers of layer
	///  0 - empty // here we don't need to change anything
	///  1 - empty // from here prepering synapse
	///  2 - empty
	/// now add corespondet number of neurons
	/// 

	unsigned int idxSynapse = 0;

	for (size_t i = 1; i < this->numberNeuronsInEachLayers.size(); i++)
	{
		// resize to correct numbers of neurons in layer
		this->networkSynapsesLayer[i].resize(this->numberNeuronsInEachLayers[i]);

		// adding adres from vecSynapse
		// adding correct numbers of synaps for neuron
		for (size_t x = 0; x < this->networkSynapsesLayer[i].size(); x++)
		{
			for (size_t y = 0; y < this->numberNeuronsInEachLayers[i - 1]; y++)
			{
				double* adress = &(this->vecSynapse[idxSynapse]);
				this->networkSynapsesLayer[i][x].push_back(adress);
				idxSynapse++;
			}
		}

	}
	*/

}

void NeuralNetwork::setInputsOutputs(std::vector<double>& inputs, std::vector<double>& outputs)
{
	


	/*if ((inputs.size() == inputLayer.size()) | (outputs.size()== outputLayer.size()))
	{

		for (size_t i = 0, tt = inputs.size(); i < tt; i++)
		{
			inputLayer[i].setInputOutput(inputs[i]);
		}

		for (size_t i = 0, tt = outputs.size(); i < tt; i++)
		{
			outputLayer[i].setInputOutput(outputs[i]);
		}

	}

	*/
}


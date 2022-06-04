#include "../include/Neuralnetwork.h"


void NeuralNetwork::createLayersOfNeurons(std::vector<double> &inputs, std::vector<double> &outputs, std::vector<unsigned int> hiddenLayersWithNumNerouns)
{
	/// <summary>
	/// tworzenie dla bitmemo!! przeniesc i uproscic
	/// 
	/// jako funkcja dla gotowych list do wczytania i zapisania
	/// </summary>
	

	unsigned int numValueToSave;

	// this->numberNeuronsInEachLayers must have all information of each layer - from input to ouptut
	// push size of inputs to first layer this->numberNeuronsInEachLayers
	this->numberNeuronsInEachLayers.push_back(inputs.size());

	// space in memo for neurons and biases
	numValueToSave = outputs.size();

	for (size_t idx = 0; idx < hiddenLayersWithNumNerouns.size(); idx++)
		{
			numValueToSave += hiddenLayersWithNumNerouns[idx];
			// push size of each layer
			this->numberNeuronsInEachLayers.push_back(hiddenLayersWithNumNerouns[idx]);
		}

	// send to create memo-space
	// this->neuronsAxon.createSpaceMemory(numValueToSave);
	// this->neuronsBias.createSpaceMemory(numValueToSave);

	// space for weights
	// layer beetwen 0 and 1
	numValueToSave = inputs.size() * hiddenLayersWithNumNerouns[0];
	
	// layer from x-1 to x
	for (size_t idx = 1; idx < hiddenLayersWithNumNerouns.size(); idx++)
		{

			numValueToSave += hiddenLayersWithNumNerouns[idx - 1] * hiddenLayersWithNumNerouns[idx];
		
		}
	
	// layer from last hidden x with output
	numValueToSave += hiddenLayersWithNumNerouns[hiddenLayersWithNumNerouns.size() - 1] * outputs.size();
	
	// send to create memo-space
	// this->weightsNeurons.createSpaceMemory(numValueToSave);

	// last is output layer size
	this->numberNeuronsInEachLayers.push_back(outputs.size());
	
	// saving list of outputs
	this->outputsLayerNetwork = &outputs;
	this->inputsLayerNetwork = &inputs;

	// set randomize value for all memo-spacers -- TODO	
	// create function for that!!
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
	delete this->inputsLayerNetwork;
	delete this->outputsLayerNetwork;
	delete this->y;

	// prawidlowo usunac listy var*
}

void NeuralNetwork::process()
{
	
	
	// -----------------------------------------------------------TODO -------------------------------------------------------------------------------
	// --2022-06-04
	
	
	
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


	// moving from last layer to first
		std::vector<double> calcError;
	for (size_t i = 0, tl = 1; i < tl; i++)
	{


		for (size_t j = 0, tn = 1; j < tn; j++)
		{
			// here is the problem in calculation -- rework needed for the loops


			for (size_t w = 0, tt = 1; w < tt; w++)
			{
				if (calcError.size() != 1)
				{
					calcError.push_back(double(0.0));
				}


			}

			// bias
		}
		// change layer to L-1
		//(*NeuralNetworkLayers)[tl - i].getNeuronsFromLayer() = (*NeuralNetworkLayers)[tl - i - 1].getNeuronsFromLayer();

		for (size_t j = 0; j < 1; j++)
		{
			// error for L-1 for next calculation
		}

	}
	

	/*
	// sprawdzenie na poczatek czy bitmemo neuron i weight sa odwrocone -- cel koniec na poczatku

	if (!this->neuronsAxon.reversed()) { this->neuronsAxon.reverse(); };
	if (!this->neuronsBias.reversed()) { this->neuronsBias.reverse(); };
	if (!this->weightsNeurons.reversed()) { this->weightsNeurons.reverse(); };

	/*
	
	zlozonosc problemu:
	kolejnosc i precyzja obliczania

	O(0) ... O(N)									-- delErrorT dla O(0) to [-(Y(O(0) - Axon(O(0))] -- roznica miedzy przewidywana (Y) a wynikiem wyjscia (O)
	w(O(0)-H(0))(0) ...w(O(N)-H(N))(N)				-- [new]w = w - r * axon(H(0)) * [ axon(O(0)) * ( 1 - axon(0))] * [ - (Y{O(0)} - axon{O(0)}]
	H(0) ... H(N)									-- delErrorT dla H(0) to suma dla delError[O(0)] + ... + delError[O(N)] czyli delError[O(0)] = w(O(0)-H(0)) * { [axon(O(0) * (1 - axon(O(0))] * [ - (Y{O(0)} - axon{O(0)}] }
	w(H(0)-I(0))(0) ... w(H(N)-I(N))(N)				-- [new]w(H(0)-I(0)) = w(H(0)-I(0)) - r * axon(I(0)) * [axon(H(0)) * ( 1 - axon(H(0))] * [ delError[H(0)]
	I(0) ... I(N)
	
	*/

	/// czy jest sens bawic sie z bit data czy moze uzyc poprostu vectora?

	

}

void NeuralNetwork::setRatio(double ratio)
{
	this->ratioLearn = ratio;
}

double NeuralNetwork::getNetError()
{
	setError();
	return this->totalError;
}


void NeuralNetwork::calculateTotalError()
{
	

	 for (size_t i = 0, tt = 1; i < tt; i++)
	 {
		 if (this->errors.size() != tt)
		 {
			 this->errors.push_back(double(0.0));
		 }

	 }
};

void NeuralNetwork::setError()
{ 
	
	this->totalError = 0.0;
	
	for (size_t i = 0, tt = 1; i < tt; i++)
	{
		double errorOut ;
		// errorOut = 0.5 * (((*y)[i] - NeuralNetworkLayers[this->NeuralNetworkLayers.size() - 1].getNeuronsFromLayer()[i].getAxon()) * ((*y)[i] - NeuralNetworkLayers[this->NeuralNetworkLayers.size() - 1].getNeuronsFromLayer()[i].getAxon()));
		
		//	this->totalError += errorOut;
	}
	
}

void NeuralNetwork::createNetwork()
{
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
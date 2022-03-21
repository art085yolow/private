#include "../include/Neuralnetwork.h"


void NeuralNetwork::createLayersOfNeurons(std::vector<double*> inputs, std::vector<double*> outputs, unsigned int nrLayers, unsigned int nrNeuronsInLayer, SummationEnum typSum, ActivationEnum typActiv)
{
	// first layer
	NeuralNetworkLayers.push_back(new Layer);
	

	for (size_t i = 0, tt = inputs.size(); i < tt; i++)
	{
		NeuralNetworkLayers[0]->addNeuronToLayer(inputs[i]);
	}

	// hidden layers
	for (size_t i = 0; i < nrLayers; i++)
	{
		NeuralNetworkLayers.push_back(new Layer);

		Layer* lastLayer = this->NeuralNetworkLayers[i];
		Layer* currentLayer = this->NeuralNetworkLayers[i + 1];
		
		for (size_t j = 0; j < nrNeuronsInLayer; j++)
		{
			currentLayer->addNeuronToLayer(lastLayer->getNeuronsFromLayer(), typSum, typActiv);
		}
		
		
	}

	// output layer
	NeuralNetworkLayers.push_back(new Layer);
	Layer* outputLayer = this->NeuralNetworkLayers[NeuralNetworkLayers.size() - 1];
	std::vector<Neuron*> vecOfNeuronsFromLastHiddenLayer = this->NeuralNetworkLayers[this->NeuralNetworkLayers.size() - 2]->getNeuronsFromLayer();
	
	for (size_t i = 0, tt = outputs.size(); i < tt; i++)
	{
		outputLayer->addNeuronToLayer(vecOfNeuronsFromLastHiddenLayer, outputs[i], typSum, typActiv);
	}
}

void NeuralNetwork::createLayersOfNeurons(std::vector<double*> inputs, std::vector<double*> outputs, std::vector<unsigned int> hiddenLayersWithNumNerouns, SummationEnum typSum, ActivationEnum typActiv)
{

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
	this->neuronsAxon.createSpaceMemory(numValueToSave);
	this->neuronsBias.createSpaceMemory(numValueToSave);

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
	this->weightsNeurons.createSpaceMemory(numValueToSave);

	// last is output layer size
	this->numberNeuronsInEachLayers.push_back(outputs.size());
	
	// saving list of outputs
	this->outputsLayerNetwork = outputs;
	this->inputsLayerNetwork = inputs;

	// set randomize value for all memo-spacers -- TODO	
	// create function for that!!
}

NeuralNetwork::NeuralNetwork()
{
}

NeuralNetwork::~NeuralNetwork()
{
	this->NeuralNetworkLayers.clear();
	this->y.clear();
	this->errors.clear();
}

void NeuralNetwork::process()
{
	/*
	// old part
	for (size_t i = 0, tt = NeuralNetworkLayers.size(); i < tt; i++)
	{
		NeuralNetworkLayers[i]->process();
	}
	*/
	
	// -----------------------------------------------------------TODO -------------------------------------------------------------------------------
	size_t indexWeights =0;
	size_t indexAxonBias = 0;  // axon and bias are in pair

	for (size_t indexLayer = 1, indexer1PosInLayerNeuronsBias = 0; indexLayer < this->numberNeuronsInEachLayers.size(); indexLayer++)
	{
		if (indexLayer == 1)
		{

			for (size_t idxCountForNeuronInLayer = 0; idxCountForNeuronInLayer < this->numberNeuronsInEachLayers[indexLayer]; idxCountForNeuronInLayer++)
			{
				double axonValue = this->neuronsBias.getValue(indexAxonBias); // first - sum for neuron; second - sigm for axon

				for (size_t idxCountEachNeuronLastLayer = 0; idxCountEachNeuronLastLayer < this->numberNeuronsInEachLayers[indexLayer - 1]; idxCountEachNeuronLastLayer++)
				{
					axonValue += *this->inputsLayerNetwork[idxCountEachNeuronLastLayer] * this->weightsNeurons.getValue(indexWeights);

					indexWeights++;
				}


				//sigmoid activation
				axonValue = (1 / (1 + exp(-axonValue)));

				// send result
				this->neuronsAxon.setValue(indexAxonBias, axonValue);

				// increase index
				indexAxonBias++;
			}

		}
		else
		{
			// todo -- first we need know from where in memoIdx is beganing last layer position

			for (size_t indxActAxon = 0; indxActAxon < this->numberNeuronsInEachLayers[indexLayer]; indxActAxon++)
			{
				double axonValue = this->neuronsBias.getValue(indexAxonBias); // first - sum for neuron; second - sigm for axon

				//counting neurons from NeuLayer
				for (size_t lastIndxAxon = 0; lastIndxAxon < this->numberNeuronsInEachLayers[indexLayer - 1]; lastIndxAxon++)
				{
					axonValue += this->neuronsAxon.getValue(indexer1PosInLayerNeuronsBias - this->numberNeuronsInEachLayers[indexLayer - 1] + lastIndxAxon) * this->weightsNeurons.getValue(indexWeights);
				
					indexWeights++;
				}

				//sigmoid activation
				axonValue = (1 / (1 + exp(-axonValue)));

				// send result
				this->neuronsAxon.setValue(indexAxonBias, axonValue);


					indexAxonBias++;
			}
		}
		
		indexer1PosInLayerNeuronsBias = indexAxonBias; /// doprecyzowac
	}
}

void NeuralNetwork::setYPredicted(std::vector<double> testOut) 
{
		this->y = testOut;
}

void NeuralNetwork::backProb()
{			///			---			REFACTOR THE HOLY PROCESS OF CALCULATION			---				///
			// main problem is the size of 'error' list from last layer to next //
			// (784x16x16x10) it is calculating just 10 times but not for every weights!! //

	// moving from last layer to first
		std::vector<double*> calcError;
	for (size_t i = 0, tl = this->NeuralNetworkLayers.size() - 1; i < tl; i++)
	{
			//std::vector<Neuron*> currentLayer = this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer();



		for (size_t j = 0, tn = this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer().size(); j < tn; j++)
		{
			// here is the problem in calculation -- rework needed for the loops

			//std::vector<double*> weightOld = this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer()[j]->getWeightVec();
			//std::vector<Neuron*> inputNeuronsForCurentLayer = this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer()[j]->getInputNeuronsVec();

			for (size_t w = 0, tt = this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer()[j]->getWeightVec().size(); w < tt; w++)
			{
				if (calcError.size() != this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer()[j]->getWeightVec().size())
				{
					double toSet = 0.0;
					calcError.push_back(&toSet);
				}

				*calcError[w] += *this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer()[j]->getWeightVec()[w] * *this->errors[j];
				*this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer()[j]->getWeightVec()[w] = *this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer()[j]->getWeightVec()[w] - this->ratioLearn * this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer()[j]->getInputNeuronsVec()[w]->getAxon() * *this->errors[j];

			}

			// bias
			this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer()[j]->setBias(this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer()[j]->getBias() - this->ratioLearn * *this->errors[j]);
		}
		// change layer to L-1
		this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer() = this->NeuralNetworkLayers[tl - i - 1]->getNeuronsFromLayer();

		for (size_t j = 0; j < this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer().size(); j++)
		{
			// error for L-1 for next calculation
			*calcError[j] = this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer()[j]->getDerivativeAxon() * *calcError[j];
		}

		this->errors.swap(calcError);
		calcError.clear();
		calcError.shrink_to_fit();
	}
	this->errors.clear();
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

std::vector<Layer*>& NeuralNetwork::getNetLayers()
{
	return this->NeuralNetworkLayers;
}

void NeuralNetwork::calculateTotalError()
{
	// std::vector<Neuron*> currentNeuronInLayer = NeuralNetworkLayers[NeuralNetworkLayers.size()-1]->getNeuronsFromLayer();
	
	for (size_t i = 0, tt = NeuralNetworkLayers[NeuralNetworkLayers.size() - 1]->getNeuronsFromLayer().size(); i < tt; i++)
	{
		if (this->errors.size() != tt)
		{
			this->errors.push_back(new double(0.0));
		}
			*this->errors[i] += -(this->y[i] - NeuralNetworkLayers[NeuralNetworkLayers.size() - 1]->getNeuronsFromLayer()[i]->getAxon()) * NeuralNetworkLayers[NeuralNetworkLayers.size() - 1]->getNeuronsFromLayer()[i]->getDerivativeAxon();

	}

}

void NeuralNetwork::setError()
{
	this->totalError = 0.0;
	//std::vector<Neuron*> layerOutput = this->NeuralNetworkLayers[this->NeuralNetworkLayers.size() - 1]->getNeuronsFromLayer();
	
	for (size_t i = 0, tt = this->NeuralNetworkLayers[this->NeuralNetworkLayers.size() - 1]->getNeuronsFromLayer().size(); i < tt; i++)
	{
		double errorOut ;
		errorOut = 0.5 * ((this->y[i] - this->NeuralNetworkLayers[this->NeuralNetworkLayers.size() - 1]->getNeuronsFromLayer()[i]->getAxon()) * (this->y[i] - this->NeuralNetworkLayers[this->NeuralNetworkLayers.size() - 1]->getNeuronsFromLayer()[i]->getAxon()));
		this->totalError += errorOut;
	}
}

void NeuralNetwork::setInputsOutputs(std::vector<double*> inputs, std::vector<double*> outputs)
{
	std::vector<Neuron*> inputLayer = this->NeuralNetworkLayers[0]->getNeuronsFromLayer();
	std::vector<Neuron*> outputLayer = this->NeuralNetworkLayers[this->NeuralNetworkLayers.size()-1]->getNeuronsFromLayer();


	if ((inputs.size() == inputLayer.size()) | (outputs.size()== outputLayer.size()))
	{

		for (size_t i = 0, tt = inputs.size(); i < tt; i++)
		{
			inputLayer[i]->setInputOutput(inputs[i]);
		}

		for (size_t i = 0, tt = outputs.size(); i < tt; i++)
		{
			outputLayer[i]->setInputOutput(outputs[i]);
		}

	}

	
}
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
	for (size_t i = 0, tt = NeuralNetworkLayers.size(); i < tt; i++)
	{
		NeuralNetworkLayers[i]->process();
	}
}

void NeuralNetwork::setYPredicted(std::vector<double> testOut) 
{
		this->y = testOut;
}

void NeuralNetwork::backProb()
{
	std::vector<double*> calcError;

	std::vector<Neuron*> currentNeuronsInLayer;

	// moving from last layer to first
	for (size_t i = 0, tl = this->NeuralNetworkLayers.size() - 1, j = 0, tn = this->NeuralNetworkLayers[tl - i]->getNeuronsFromLayer().size(); i < tl; i++)
	{
		std::vector<Neuron*> currentLayer = this->NeuralNetworkLayers[tl-i]->getNeuronsFromLayer();

		for (j = 0; j < tn; j++)
		{
			std::vector<double*> weightOld = currentLayer[j]->getWeightVec();
			std::vector<Neuron*> inputNeuronsForCurentLayer = currentLayer[j]->getInputNeuronsVec();

			for (size_t w = 0, tt = weightOld.size(); w < tt; w++)
			{
				if (calcError.size() != weightOld.size())
				{
					calcError.push_back(new double(0.0));
				}

				*calcError[w] += *weightOld[w] * *errors[j];
				*weightOld[w] = *weightOld[w] - this->ratioLearn * inputNeuronsForCurentLayer[w]->getAxon() * *errors[j];

			}

			// bias
			currentLayer[j]->setBias(currentLayer[j]->getBias() - ratioLearn * *errors[j]);
			inputNeuronsForCurentLayer.clear();
			weightOld.clear();
		}
		// change layer to L-1
		currentLayer = this->NeuralNetworkLayers[tl - i - 1]->getNeuronsFromLayer();

		for (j = 0; j < currentLayer.size(); j++)
		{
			// error for L-1 for next calculation
			*calcError[j] = currentLayer[j]->getDerivativeAxon() * *calcError[j];
		}

		this->errors = calcError;
		calcError.clear();
		currentLayer.clear();
	}
	currentNeuronsInLayer.clear();
}

void NeuralNetwork::setRatio(double ratio)
{
	this->ratioLearn = ratio;
}

double NeuralNetwork::getError()
{
	setError();
	return this->totalError;
}

std::vector<Layer*> NeuralNetwork::getNetLayers()
{
	return this->NeuralNetworkLayers;
}

void NeuralNetwork::calculateTotalError()
{
	std::vector<double*> calcError;
	
	std::vector<Neuron*> currentNeuronInLayer = NeuralNetworkLayers[NeuralNetworkLayers.size()-1]->getNeuronsFromLayer();
	
	for (size_t i = 0, tt = currentNeuronInLayer.size(); i < tt; i++)
	{
			double er =-(this->y[i] - currentNeuronInLayer[i]->getAxon()) * currentNeuronInLayer[i]->getDerivativeAxon();
			calcError.push_back(new double(er));

	}

	if (this->errors.empty())
	{
		// if 'errors' vector is empty, 'errors' take all from 'calcError'
		this->errors = calcError;  /// error ostatniej warstwy PRAWIDLOWO --- error last layer -- correct
	}
	else
	{
		// if is not empty, each value of the same index is added
		for (size_t i = 0, tt = this->errors.size(); i < tt; i++)
		{
			*this->errors[i] += *calcError[i];
		}
	}
	calcError.clear();
	currentNeuronInLayer.clear();
}

void NeuralNetwork::setError()
{
	this->totalError = 0.0;
	std::vector<Neuron*> layerOutput = this->NeuralNetworkLayers[this->NeuralNetworkLayers.size() - 1]->getNeuronsFromLayer();
	
	for (size_t i = 0, tt = layerOutput.size(); i < tt; i++)
	{
		double errorOut = 0.5 * ((this->y[i] - layerOutput[i]->getAxon()) * (this->y[i] - layerOutput[i]->getAxon()));
		this->totalError += errorOut;
	}
	layerOutput.clear();
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
	inputLayer.clear();
	outputLayer.clear();
}
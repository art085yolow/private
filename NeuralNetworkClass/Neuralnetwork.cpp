#include "Neuralnetwork.h"


void NeuralNetwork::createLayersOfNeurons(std::vector<double> inputs, std::vector<double> outputs, unsigned int nrLayers, unsigned int nrNeuronsInLayer, SummationEnum typSum, ActivationEnum typActiv)
{
	// first layer
	NeuralNetworkLayers.push_back(new Layer);
	
	for (size_t i = 0, tt = inputs.size(); i < tt; i++)
	{
		NeuralNetworkLayers[0]->neuronsInLayer.push_back(new Neuron(&inputs[i]));
	}

	// hidden layers
	for (size_t i = 0; i < nrLayers; i++)
	{
		NeuralNetworkLayers.push_back(new Layer);
		for (size_t j = 0; j < nrNeuronsInLayer; j++)
		{
			NeuralNetworkLayers[i + 1]->neuronsInLayer.push_back(new Neuron(NeuralNetworkLayers[i]->neuronsInLayer, typSum, typActiv));
		}
	}

	// output layer
	NeuralNetworkLayers.push_back(new Layer);
	size_t lastLayer = NeuralNetworkLayers.size() - 1;
	for (size_t i = 0, tt = outputs.size(); i < tt; i++)
	{
		NeuralNetworkLayers[lastLayer]->neuronsInLayer.push_back(new Neuron(NeuralNetworkLayers[lastLayer - 1]->neuronsInLayer, &outputs[i], typSum, typActiv));
	}
}

void NeuralNetwork::process()
{
	for (size_t i = 0, tt = NeuralNetworkLayers.size(); i < tt; i++)
	{
		NeuralNetworkLayers[i]->process();
	}
}

void NeuralNetwork::setInputsOutputs(std::vector<double> inputs, std::vector<double> outputs)
{

	if ((inputs.size() == NeuralNetworkLayers[0]->neuronsInLayer.size()) | (outputs.size()== NeuralNetworkLayers[NeuralNetworkLayers.size() - 1]->neuronsInLayer.size()))
	{

		for (size_t i = 0, tt = inputs.size(); i < tt; i++)
		{
			NeuralNetworkLayers[0]->neuronsInLayer[i]->setInputOutput(&inputs[i]);
		}

		for (size_t i = 0, tt = outputs.size(); i < tt; i++)
		{
			NeuralNetworkLayers[NeuralNetworkLayers.size()-1]->neuronsInLayer[i]->setInputOutput(&outputs[i]);
		}

	}

	
}
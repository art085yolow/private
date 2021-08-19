#include "Neuralnetwork.h"


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
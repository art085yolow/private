#include "Neuralnetwork.h"

void NeuralNetwork::process()
{
	for (size_t i = 0, tt = NeuralNetworkLayers.size(); i < tt; i++)
	{
		NeuralNetworkLayers[i]->process();
	}
}

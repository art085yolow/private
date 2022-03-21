#include "../include/Layer.h"

Layer::Layer()
{
}

Layer::~Layer()
{
	this->neuronsInLayer.clear();
}

void Layer::process()
{
	for (size_t i = 0, tt = neuronsInLayer.size(); i < tt; i++)
	{
		neuronsInLayer[i]->process();
	}
}

void Layer::addNeuronToLayer(double* inputForNeuron)
{
	this->neuronsInLayer.push_back(new Neuron(inputForNeuron));
}

void Layer::addNeuronToLayer(std::vector<Neuron*> synapseIn, SummationEnum typSum, ActivationEnum typActiv)
{
	this->neuronsInLayer.push_back(new Neuron(synapseIn, typSum, typActiv));
}

void Layer::addNeuronToLayer(std::vector<Neuron*> synapseIn, double* out, SummationEnum typSum, ActivationEnum typActiv)
{
	this->neuronsInLayer.push_back(new Neuron(synapseIn, out, typSum, typActiv));
}


std::vector<Neuron*>& Layer::getNeuronsFromLayer()
{
	return this->neuronsInLayer;
}

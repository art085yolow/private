#include "Neuron.h"

Neuron::Neuron()
{
}

Neuron::~Neuron()
{
}

void Neuron::setSynapse(std::vector<Neuron*> synapseIn)
{
	for (size_t i = 0, tt = synapseIn.size(); i < tt; i++) {
		this->_dendrites.push_back(new Synapse);
		this->inputsDendrites.push_back(synapseIn[i]);
	}

}

void Neuron::setBias(double val)
{
	this->bias = val;
}

void Neuron::setInputOutput(double* val)
{
	if (_dendrites.empty())
	{
		this->inOutValue = val;
	}
	else
	{
		val = this->inOutValue;
	}
}

double Neuron::getAxon()
{
	return this->axon;
}

double Neuron::getBias()
{
	return this->bias;
}

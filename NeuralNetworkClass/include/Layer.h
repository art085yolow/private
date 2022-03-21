#pragma once
#include <vector>
#include "Neuron.h"


	class Layer
	{
	public:
		Layer();
		~Layer();

		// activate calculation in each neuron
		void process();

		// add new Neuron
		void addNeuronToLayer(double* inputForNeuron);
		void addNeuronToLayer(std::vector<Neuron*> synapseIn, SummationEnum typSum = SummationEnum::Suma, ActivationEnum typActiv = ActivationEnum::Sigm);
		void addNeuronToLayer(std::vector<Neuron*> synapseIn, double* out, SummationEnum typSum = SummationEnum::Suma, ActivationEnum typActiv = ActivationEnum::Sigm);

		// get vector of neuronsfrom this layer
		std::vector<Neuron*>& getNeuronsFromLayer();
	
	private:
		std::vector<Neuron*> neuronsInLayer;
	};

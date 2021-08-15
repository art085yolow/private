#pragma once
#include <vector>

#include "Synapse.h"

#include "SAEnum.h"


	

	class Neuron
	{
	public:
		Neuron();
		~Neuron();

		//Neuron* getInstance(enum SummationEnum _typSum, enum ActivationEnum _typActiv, std::vector<Neuron*> synapseIn);
		//Neuron* getInstRef();

		void process();
		
		void setSynapse(std::vector<Neuron*> synapseIn);
		void setBias(double val);
		void setInputOutput(double* val);
		void setSumActiv(enum SummationEnum _typSum, enum ActivationEnum _typActiv);
		void activationFunction();
		void summationFunctions();

		double getAxon();
		double getBias();

	private:
		SummationEnum typSum;
		ActivationEnum typActiv;
		double soma = 0.0;
		double axon = 0.0;
		double bias = 1.0;
		
		std::vector<Synapse*> _dendrites;
		std::vector<Neuron*> inputsDendrites;
		

		double* inOutValue = nullptr;
	};

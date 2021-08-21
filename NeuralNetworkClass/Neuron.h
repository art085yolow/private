#pragma once
#include <vector>

#include "Synapse.h"
#include "SAEnum.h"

	class Neuron
	{
	public:
		Neuron();
		// Neuron(konstruktor z wartosciami wejsciowymi); // czy sa potrzebne?

		// Create Neuron with input value
		Neuron(double* in);

		// Create Neuron with last layer and set Summation and Activation
		Neuron(std::vector<Neuron*> synapseIn, SummationEnum typSum = SummationEnum::Suma, ActivationEnum typActiv = ActivationEnum::Sigm);
		
		// Create Neuron in last layer with layer befor and set Summation and Activation
		Neuron(std::vector<Neuron*> synapseIn, double* out, SummationEnum typSum = SummationEnum::Suma, ActivationEnum typActiv = ActivationEnum::Sigm);
		
		~Neuron();


		// funkcje wykonawcze
		void process();
		void activationFunction();
		void summationFunctions();
		
		// funkcje set
		void setSynapse(std::vector<Neuron*> synapseIn);
		void setBias(double val);
		void setInputOutput(double* val);
		void setSumActiv(SummationEnum _typSum, ActivationEnum _typActiv);

		// funkcje get
		double getAxon();
		double getBias();

	private:
		SummationEnum typSum = SummationEnum::Suma;
		ActivationEnum typActiv = ActivationEnum::Sigm;
		
		// wartosci neuronow
		double soma = 0.0;
		double axon = 0.0;
		double bias = 1.0;
		
		// wejscia neuronow - neuron wejsciowy i synapsa
		std::vector<Synapse*> _dendrites; //aktualnie to tylko "double weight". rozwarzyc usuniecie synapse.h/cpp zastepujac poprostu double-list.<?><?>
		std::vector<Neuron*> inputsDendrites;
		
		// wartosci do/z neuronu 
		double* inOutValue = nullptr;
	};

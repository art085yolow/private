#pragma once
#include <vector>

#include "SAEnum.h"

	class Neuron
	{
	public:
		Neuron();

		// Create Neuron with input value
		Neuron(double* in);

		// Create Neuron(second layer ++) with connection with layer L-1 and set Summation and Activation
		Neuron(std::vector<Neuron*> synapseIn, SummationEnum typSum = SummationEnum::Suma, ActivationEnum typActiv = ActivationEnum::Sigm);
		
		// Create Neuron with connection with layer L-1 with desired output and set Summation and Activation
		Neuron(std::vector<Neuron*> synapseIn, double* out, SummationEnum typSum = SummationEnum::Suma, ActivationEnum typActiv = ActivationEnum::Sigm);
		
		~Neuron();


		// feedforward
		void process();

		

		// set

		// set connection and weight
		void setSynapse(std::vector<Neuron*> synapseIn);
		
		// set bias
		void setBias(double val);
		
		// set ptr inOutValue -- for input or output for/from calculation
		void setInputOutput(double* val);
		
		// set type of summation or activation
		void setSumActiv(SummationEnum _typSum, ActivationEnum _typActiv);


		//  get

		// get result value of neuron
		double getAxon();

		// derivative function for backpropagation
		double getDerivativeAxon();

		// get value of bias
		double getBias();

		// get vector of weight of neurons connected with this neuron
		std::vector<double*> getWeightVec();

	private:
		SummationEnum typSum = SummationEnum::Suma;
		ActivationEnum typActiv = ActivationEnum::Sigm;
		
		// neuron value

		// summation value
		double soma = 0.0;

		// activated value of neuron
		double axon = 0.0;

		double bias = 1.0;
		
		// connected inputs neurons and coresponted weights
		std::vector<double*> _weights; 
		std::vector<Neuron*> inputsDendrites;
		
		// ptr for input value and output to send out
		double* inOutValue = nullptr;

		// calculation function
		void activationFunction();
		void summationFunctions();
		
	};

#pragma once
#include <vector>
#include "SAEnum.h"
#include "Layer.h"


	
	class NeuralNetwork
	{
	public:
		NeuralNetwork() {};

		void createLayersOfNeurons(unsigned int nrInputs = 1, unsigned int nrOutputs = 1, unsigned int nrLayers = 1, unsigned int nrNeuronsInLayer = 1, enum SummationEnum typSum=SummationEnum::Suma, enum ActivationEnum typActiv=ActivationEnum::Sigm);
		void process();

		void setInputsOutputs(std::vector<double> inputs, std::vector<double> outputs);

	private:
		std::vector<Layer*> NeuralNetworkLayers; //obejmuje In Hidden Out

	};

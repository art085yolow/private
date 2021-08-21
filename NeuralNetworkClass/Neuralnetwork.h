#pragma once
#include <vector>
#include "SAEnum.h"
#include "Layer.h"


	
	class NeuralNetwork
	{
	public:
		NeuralNetwork();


		/*		podstawowy szybki kreator		----		automatyczne dopasowanie ilosci wejsc i wyjsc(<vector>inputs, <vector>outputs), liczba warstw ukrytych, liczba neuronow w warstwach ukrytych, (enum) typ sumowania(domyslna SUMA), (enum) typ aktywacji(domyslna SIGM)*/
		void createLayersOfNeurons(std::vector<double*> inputs, std::vector<double*> outputs, unsigned int nrLayers = 1, unsigned int nrNeuronsInLayer = 1, SummationEnum typSum=SummationEnum::Suma, ActivationEnum typActiv=ActivationEnum::Sigm);
		
		/* indywidualne tworzenie sieci -- zachowac ostroznosc*/
		void createLayersOfNeurons(unsigned int nrInputs = 1, unsigned int nrOutputs = 1, unsigned int nrLayers = 1, unsigned int nrNeuronsInLayer = 1, SummationEnum typSum=SummationEnum::Suma, ActivationEnum typActiv=ActivationEnum::Sigm);
		
		void process();

		void setInputsOutputs(std::vector<double*> inputs, std::vector<double*> outputs);

	private:
		std::vector<Layer*> NeuralNetworkLayers; //obejmuje In Hidden Out

	};

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
		
		/* indywidualne tworzenie sieci -- zachowac ostroznosc*/ // TODO
		void createLayersOfNeurons(unsigned int nrInputs = 1, unsigned int nrOutputs = 1, unsigned int nrLayers = 1, unsigned int nrNeuronsInLayer = 1, SummationEnum typSum=SummationEnum::Suma, ActivationEnum typActiv=ActivationEnum::Sigm);

		void setInputsOutputs(std::vector<double*> inputs, std::vector<double*> outputs);
		
		// feedForward
		void process();


		// lista bledu dla warstwy output do obliczen -- znalesc i zrozumiec!

		void setError(); // -- ustalic co wyslac lub jakie funkcje spisac <?> --- TODO

	private:
		std::vector<Layer*> NeuralNetworkLayers; //obejmuje In Hidden Out

		double ratioLearn = 0.05;
		double predictedY =0.0;

	};

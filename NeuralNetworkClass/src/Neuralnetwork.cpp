#include "../include/Neuralnetwork.h"


void NeuralNetwork::createLayersOfNeurons(std::vector<double*> inputs, std::vector<double*> outputs, unsigned int nrLayers, unsigned int nrNeuronsInLayer, SummationEnum typSum, ActivationEnum typActiv)
{
	// first layer
	NeuralNetworkLayers.push_back(new Layer);
	
	for (size_t i = 0, tt = inputs.size(); i < tt; i++)
	{
		NeuralNetworkLayers[0]->neuronsInLayer.push_back(new Neuron(inputs[i]));
	}

	// hidden layers
	for (size_t i = 0; i < nrLayers; i++)
	{
		NeuralNetworkLayers.push_back(new Layer);
		for (size_t j = 0; j < nrNeuronsInLayer; j++)
		{
			NeuralNetworkLayers[i + 1]->neuronsInLayer.push_back(new Neuron(NeuralNetworkLayers[i]->neuronsInLayer, typSum, typActiv));
		}
	}

	// output layer
	NeuralNetworkLayers.push_back(new Layer);
	size_t lastLayer = NeuralNetworkLayers.size() - 1;
	for (size_t i = 0, tt = outputs.size(); i < tt; i++)
	{
		NeuralNetworkLayers[lastLayer]->neuronsInLayer.push_back(new Neuron(NeuralNetworkLayers[lastLayer - 1]->neuronsInLayer, outputs[i], typSum, typActiv));
	}
}

NeuralNetwork::NeuralNetwork()
{
}

void NeuralNetwork::process()
{
	for (size_t i = 0, tt = NeuralNetworkLayers.size(); i < tt; i++)
	{
		NeuralNetworkLayers[i]->process();
	}
}

void NeuralNetwork::setYPredicted(std::vector<double> testOut) 
{
	for (size_t i = 0, tt = testOut.size(); i < tt; i++)
	{
		this->y.push_back(testOut[i]);
	}
}

void NeuralNetwork::setRatio(double ratio)
{
	this->ratioLearn = ratio;
}

double NeuralNetwork::getError()
{
	setError();
	return this->totalError;
}

void NeuralNetwork::calculateTotalError()
{
	
	std::vector<double*> calcError;
	for (size_t i = 0, ll = NeuralNetworkLayers.size() - 1, tt = NeuralNetworkLayers[ll]->neuronsInLayer.size(); i < tt; i++)
	{
			double er =-(this->y[i] - this->NeuralNetworkLayers[ll]->neuronsInLayer[i]->getAxon()) * this->NeuralNetworkLayers[ll]->neuronsInLayer[i]->getDerivativeAxon();
			calcError.push_back(new double(er));

	}


	this->errors = calcError;  /// error ostatniej warstwy PRAWIDLOWO --- error last layer -- correct

	calcError.clear();

	for (size_t i = 0, tl = this->NeuralNetworkLayers.size()-1, j = 0, tn = this->NeuralNetworkLayers[ tl-i]->neuronsInLayer.size(); i < tl; i++)
	{
		for (j=0; j < tn; j++)
		{
			std::vector<double*> weightOld = this->NeuralNetworkLayers[tl - i]->neuronsInLayer[j]->getWeightVec();
			
			for (size_t w = 0, tt = weightOld.size(); w < tt; w++)
			{
				if (calcError.size() != weightOld.size())
				{
					calcError.push_back(new double(0.0));
				}

				*calcError[w] += *weightOld[w] * *errors[j];
				*weightOld[w] = *weightOld[w] - this->ratioLearn * this->NeuralNetworkLayers[tl - i - 1]->neuronsInLayer[w]->getAxon() * *errors[j];
				
			}
			
			// bias
			this->NeuralNetworkLayers[tl - i]->neuronsInLayer[j]->setBias(this->NeuralNetworkLayers[tl - i]->neuronsInLayer[j]->getBias() - ratioLearn * *errors[j]);
		}

		for (j = 0; j < this->NeuralNetworkLayers[tl - i - 1]->neuronsInLayer.size(); j++)
		{
			// error for L-1
			*calcError[j] = this->NeuralNetworkLayers[tl - i - 1]->neuronsInLayer[j]->getDerivativeAxon() * *calcError[j];
		}
		
		this->errors = calcError;
		calcError.clear();
	}

}

void NeuralNetwork::setError()
{
	this->totalError = 0.0;
	size_t lOut = this->NeuralNetworkLayers.size() - 1;
	for (size_t i = 0, tt = this->NeuralNetworkLayers[lOut]->neuronsInLayer.size(); i < tt; i++)
	{
		double errorOut = 0.5 * ((this->y[i] - this->NeuralNetworkLayers[lOut]->neuronsInLayer[i]->getAxon()) * (this->y[i] - this->NeuralNetworkLayers[lOut]->neuronsInLayer[i]->getAxon()));
		this->totalError += errorOut;
	}
}

void NeuralNetwork::setInputsOutputs(std::vector<double*> inputs, std::vector<double*> outputs)
{

	if ((inputs.size() == NeuralNetworkLayers[0]->neuronsInLayer.size()) | (outputs.size()== NeuralNetworkLayers[NeuralNetworkLayers.size() - 1]->neuronsInLayer.size()))
	{

		for (size_t i = 0, tt = inputs.size(); i < tt; i++)
		{
			NeuralNetworkLayers[0]->neuronsInLayer[i]->setInputOutput(inputs[i]);
		}

		for (size_t i = 0, tt = outputs.size(); i < tt; i++)
		{
			NeuralNetworkLayers[NeuralNetworkLayers.size()-1]->neuronsInLayer[i]->setInputOutput(outputs[i]);
		}

	}

	
}
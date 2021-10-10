#include "../include/RNN.h"

RNN::RNN()
{

}

RNN::RNN(WindowNet* _progWindow, DataStream _data)
{
	this->window = _progWindow;
	this->data = _data;
}

RNN::~RNN()
{
}

void RNN::setData(DataStream _data)
{
	this->data = _data;
}

void RNN::setNetwork()
{
	if (!this->data.getListOfImages().empty())
	{
		for (size_t i = 0, tt = this->data.getListOfImages()[0]->width * this->data.getListOfImages()[0]->height; i < tt; i++)
		{
			this->inputs.push_back(new double());
		}

		for (size_t i = 0; i < 10; i++)
		{
			this->outputs.push_back(new double());
		}

		RNNnet.createLayersOfNeurons(this->inputs, this->outputs, 2, 16);

	}

}

void RNN::trainNetwork()
{
	// devide all train data for 10 sampling image train and calculate error

	for (size_t nrImages = this->data.getListOfImages().size(); this->sampling < nrImages; this->sampling++)
	{
		for (size_t i = 0; i < 10; i++)
		{
			// send image list to network,process and calculate error
			for (size_t y = 0, tt = this->data.getListOfImages()[this->sampling]->m_char.size(); y < tt; y++)
			{
				*this->inputs[y] = this->data.getListOfImages()[this->sampling]->getDoubleValue(y);
			}

			this->RNNnet.process();
			// need sort function in NeuralNetwork. 

			// set the correct 'y' answer
			unsigned int yLab = this->data.getListOfLabels()[this->sampling];
			std::vector<double> yResult;
			// TODO loop for output layer
			for (size_t g = 0; g < 10; g++)
			{
				if(g==yLab)
				{
					yResult.push_back(1.0);
				}
				else
				{
					yResult.push_back(0.0);
				}
			}
			this->RNNnet.setYPredicted(yResult);

			// calculate error output
			this->RNNnet.calculateTotalError();

		}
		this->RNNnet.backProb();
	}
}

void RNN::testNetwork()
{
}

void RNN::render()
{
	this->window->renderImage(0.0f, 0.0f, 1.0f, this->data.getListOfImages()[0]->width, this->data.getListOfImages()[0]->height, this->data.getListOfImages()[sampling]->getColor());
}

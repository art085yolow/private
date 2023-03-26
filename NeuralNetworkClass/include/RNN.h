#pragma once
#include "Neuralnetwork.h"
#include "DataStream.h"

class RNN 
{
public:
	RNN(std::string neural_network_size);
	~RNN();

	void setData(DataStream& _data);
	void setNetwork();


	void trainNetwork();
	void testNetwork();

	void render();

private:
	DataStream* data;
	NeuralNetwork *RNNnet = nullptr;

	unsigned int samplings =0;
	unsigned int numberTest;
	unsigned int correctAnswer;
};
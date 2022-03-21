#pragma once
#include "Neuralnetwork.h"
#include "DataStream.h"
#include "render/WindowNet.h"

class RNN :  NeuralNetwork {
public:
	RNN();
	RNN(WindowNet* progWindow, DataStream _data);
	~RNN();

	void setData(DataStream _data);
	void setNetwork();


	void trainNetwork();
	void testNetwork();

	void render();

private:
	WindowNet* window;
	DataStream data;
	NeuralNetwork RNNnet;
	std::vector<double*> inputs;
	std::vector<double*> outputs;
	std::vector<double*> yPredict;

	unsigned int sampling =0;
	unsigned int numberTest;
	unsigned int correctAnswer;
};
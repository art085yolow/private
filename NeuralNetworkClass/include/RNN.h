#pragma once
#include "Neuralnetwork.h"
#include "DataStream.h"

class RNN 
{
public:
	RNN(std::string neural_network_size);
	~RNN();

	
	void trainNetwork(DataStream& _data, unsigned int batch_size, unsigned int test_size);
	void testNetwork(DataStream& _data, unsigned int question_size);

	float procent_of_correct_asware();

private:
	NeuralNetwork *RNNnet = nullptr;

	unsigned int samplings =0;
	unsigned int numberTest;
	unsigned int correctAnswer;
	float procent_correct_asware=0.0f;
	
	
	// print in console
	void render( Image& image, unsigned int width, unsigned int height);

};
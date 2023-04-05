#pragma once
#include "Neuralnetwork.h"
#include "DataStream.h"

class RNN 
{
public:
	RNN(std::string _neural_network_size);
	~RNN();

	
	void trainNetwork(DataStream& _data, unsigned int _batch_size, unsigned int _test_size);
	void testNetwork(DataStream& _data, unsigned int _question_size);

	float procent_of_correct_asware();

private:
	NeuralNetwork* m_RNNnet = nullptr;

	double m_error_limit = 0.05;
	unsigned int m_samplings =0;
	unsigned int m_correctAnswer;
	float m_procent_correct_asware=0.0f;
	
	
	// print in console
	void render( Image& _image, unsigned int _width, unsigned int _height);

};
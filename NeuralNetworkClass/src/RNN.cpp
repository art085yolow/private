#include "../include/RNN.h"

RNN::RNN(std::string neural_network_size)
{
	// try find if file exist if not create network
}


RNN::~RNN()
{
}

void RNN::setData(DataStream& _data)
{
	this->data = &_data;
}

void RNN::setNetwork()
{

}

void RNN::trainNetwork()
{
	// devide all train data for 10 sampling image train and calculate error

}

void RNN::testNetwork()
{
}

void RNN::render()
{
	// print image on console
}

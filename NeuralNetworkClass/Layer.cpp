#include "Layer.h"

Layer::Layer()
{
}

Layer::~Layer()
{
}

void Layer::process()
{
	for (size_t i = 0, tt = neuronsInLayer.size(); i < tt; i++)
	{
		neuronsInLayer[i]->process();
	}
}

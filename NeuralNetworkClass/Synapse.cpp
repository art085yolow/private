#include "Synapse.h"
#include <stdlib.h>

Synapse::Synapse()
{
	this->weight = (double)rand() / RAND_MAX;
}

void Synapse::setWeight(double val)
{
	this->weight = val;
}

double Synapse::getWeight()
{
	return this->weight;
}

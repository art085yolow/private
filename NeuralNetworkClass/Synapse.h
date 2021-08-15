#pragma once

class Synapse
{
public:
	Synapse();
	~Synapse() {};

	void setWeight(double val);

	double getWeight();

private:
	double weight = 0.0;

};

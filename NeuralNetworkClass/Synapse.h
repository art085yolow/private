#pragma once

class Synapse
{
public:
	Synapse();
	~Synapse() {};

	// funkcje set
	void setWeight(double val);

	// funkcje get
	double getWeight();

private:
	double weight = 0.0;

};

#pragma once
#include <vector>
#include "Neuron.h"


	class Layer
	{
	public:
		Layer();
		~Layer() { };

		void process();

	
		std::vector<Neuron*> neuronsInLayer;
	};

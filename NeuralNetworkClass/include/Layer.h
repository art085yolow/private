#pragma once
#include <vector>
#include "Neuron.h"


	class Layer
	{
	public:
		Layer();
		~Layer();

		// activate calculation in each neuron
		void process();

	
		std::vector<Neuron*> neuronsInLayer;
	};

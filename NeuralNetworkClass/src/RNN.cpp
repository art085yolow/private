#include "../include/RNN.h"

RNN::RNN(std::string _neural_network_size)
{
	// try find if file exist if not create network
	this->m_RNNnet=new NeuralNetwork(_neural_network_size);

}


RNN::~RNN()
{
}


void RNN::trainNetwork(DataStream& _data, unsigned int _batch_size, unsigned int _test_size)
{
	// shuflle data
	_data.shuffle_list();

	// devide all train data for 10|_batch_size sampling image train and calculate error
	
	// test if _test_size is in range of data
	if(_test_size> _data.sizeList()) _test_size = _data.sizeList();
	
	for (size_t imagetIteration = 0; imagetIteration < _test_size; imagetIteration )
	{
		//system("cls");

		//std::cout << "\n\tStep: " << imagetIteration << "\n";
		
		// creating general vector for errors for neurons and synapses
		std::vector<double> errNeu, errSyn;

		// batch iteration
		for (size_t batch_idx = 0; batch_idx < _batch_size && imagetIteration < _test_size; batch_idx++, imagetIteration++)
		{
			// error vectors for one step
			std::vector<double> errNeu_step, errSyn_step;
			
			auto image_in = _data[imagetIteration];
			
			// write value to inputs
			for (size_t index = 0; index < image_in.size(); index++)
			{
				// creating string for index // exp: "I-23" - neuron-typ input: index 23
				std::string string_index = "I-" + std::to_string(index);
				// assigning a floating point(double) value from 0-255(unsigned char)
				(*m_RNNnet)[string_index] = image_in.m_image_source[index] / 255.0;
			}

			// calculate network
			m_RNNnet->process();

			
				// write prediction y
				std::vector<double> y(10);
				for (auto& it : y)
				{
					// setign low neural activity
					it = 0.001;
				}
				// seting high neural activity 
				y[image_in.m_label] = 0.99;

				// calcularing error network for that one step
				m_RNNnet->calculate_network_error(errNeu_step, errSyn_step, y);

				// checking if general error vectors are equal if not, adjust
				if (errNeu.size() != errNeu_step.size()) errNeu.resize(errNeu_step.size());
				if (errSyn.size() != errSyn_step.size()) errSyn.resize(errSyn_step.size());

				// add error from this step to general error vector
				for (size_t i = 0; i < errSyn.size(); i++)
				{
					errSyn[i] += errSyn_step[i];
					if (i < errNeu.size())
					{
						errNeu[i] += errNeu_step[i];
					}
				}
			
		}

		// get average sum of errors
		for (auto& n : errNeu) n /= _batch_size;
		for (auto& s : errSyn) s /= _batch_size;

		// adjust network
		m_RNNnet->backpropagation(errNeu, errSyn);
		
	}

	std::cout << "\n - - - - NETWORK SAVED - - - - \n" << std::endl;
	m_RNNnet->save_network();
}

void RNN::testNetwork(DataStream& _data, unsigned int _question_size)
{
	// shuffle data
	_data.shuffle_list();
	
	// clearing value for test
	this->m_correctAnswer = 0;
	this->m_samplings = 0;
	
	// testing if _question_size is in range of data
	if (_question_size > _data.sizeList()) _question_size = _data.sizeList();
	
	// testing network for each image
	for (size_t imagetIteration = 0; imagetIteration < _question_size; imagetIteration++)
	{
			// taking one image
			auto image_in = _data[imagetIteration];
			
			// write value to inputs
			for (size_t index = 0; index < image_in.size(); index++)
			{
				// string for index // exp: "I-23" - neuron-typ input: index 23 // assigning value
				(*m_RNNnet)[ "I-" + std::to_string(index) ] = image_in.m_image_source[index] / 255.0;
			}

			// network calculating 
			m_RNNnet->process();


			// highest activ neuron
			size_t answare = 0;
			double answare_value = 0.0;
			for (size_t output_index = 0, output_size = m_RNNnet->get_output_size(); output_index < output_size; output_index++)
			{
				double answare_output = (*m_RNNnet)["O-" + std::to_string(output_index)];
				if (answare_output > answare_value)
				{
					answare_value = answare_output;
					answare = output_index;
				}
			}

			// comparison of aswares
			if (answare == (unsigned char)image_in.m_label)
			{
				//increas correct asware
				this->m_correctAnswer++;
			}
			else
			{
				//wrong - print image
				unsigned int number = image_in.m_label;
				this->render(image_in, _data.getWidth(), _data.getHeight());
			std::cout << "\tNUMBER: " << number << "\tNETWORK ANSWARE: " << answare << std::endl;
			}

			// increasing sampling
			this->m_samplings++;
			
			//procent of correct aswares
			this->m_procent_correct_asware = ((float)this->m_correctAnswer) / (float)(this->m_samplings) * 100.0f;

	}
	std::cout << "\t%: " << this->m_procent_correct_asware << std::endl;
}

float RNN::procent_of_correct_asware()
{
	return this->m_procent_correct_asware;
}

void RNN::render(Image& _image, unsigned int _width, unsigned int _height)
{
	// print image on console
	//system("cls");

	std::cout << "\n - - - - - - - - - - - - - - - \n" << std::endl;


	std::string string_image;
	for (size_t y = 0; y < _height; y++)
	{
		for (size_t x = 0; x < _width; x++)
		{
			
			unsigned int pixel = _image.m_image_source[y * _width + x];
			if (pixel >= 0 && pixel < 85)
			{
				string_image += " ";
			}
			else if (pixel >= 85 && pixel < 170)
			{
				string_image += ".";
			}
			else if (pixel>=170)
			{
				string_image += "*";
			}
		}

		string_image += "\n";
	}

	std::cout << string_image;
	std::cout << " - - - - - - - - - - - - - - - \n" << std::endl;
	
}

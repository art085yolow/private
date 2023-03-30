#include "../include/RNN.h"

RNN::RNN(std::string neural_network_size)
{
	// try find if file exist if not create network

	this->RNNnet=new NeuralNetwork(neural_network_size);

}


RNN::~RNN()
{
}


void RNN::trainNetwork(DataStream& _data, unsigned int batch_size, unsigned int test_size)
{
	_data.shuffle_list();

	// devide all train data for 10|batch_size sampling image train and calculate error
	
	if(test_size> _data.sizeList()) test_size = _data.sizeList();
	
	for (size_t imagetIteration = 0; imagetIteration < test_size; imagetIteration )
	{
		//system("cls");

		//std::cout << "\n\tStep: " << imagetIteration << "\n";
		std::vector<double> errNeu, errSyn;
		// batch iteration
		for (size_t batch_idx = 0; batch_idx < batch_size && imagetIteration < test_size; batch_idx++, imagetIteration++)
		{
			std::vector<double> errNeu_step, errSyn_step;
			auto image_in = _data[imagetIteration];
			// write value to inputs
			for (size_t index = 0; index < image_in.size(); index++)
			{
				std::string string_index = "I-" + std::to_string(index);
				(*RNNnet)[string_index] = (unsigned char)image_in.imageSource[index] / 255.0;
			}

			// calculate 
			RNNnet->process();

			
				// write prediction y
				std::vector<double> y(10);
				for (auto& it : y)
				{
					it = 0.001;
				}
				y[(unsigned char)image_in.label] = 0.99;

				RNNnet->calculateNetErr(errNeu_step, errSyn_step, y);

				if (errNeu.size() != errNeu_step.size()) errNeu.resize(errNeu_step.size());
				if (errSyn.size() != errSyn_step.size()) errSyn.resize(errSyn_step.size());

				for (size_t i = 0; i < errSyn.size(); i++)
				{
					errSyn[i] += errSyn_step[i];
					if (i < errNeu.size())
					{
						errNeu[i] += errNeu_step[i];
					}
				}
			
		}
		for (auto& n : errNeu) n /= batch_size;
		for (auto& s : errSyn) s /= batch_size;

		RNNnet->backProb(errNeu, errSyn);
		
	}

	std::cout << "\n - - - - NETWORK SAVED - - - - \n" << std::endl;
	RNNnet->save_network();
}

void RNN::testNetwork(DataStream& _data, unsigned int question_size)
{
	_data.shuffle_list();
	this->correctAnswer = 0;
	this->samplings = 0;
	if (question_size > _data.sizeList()) question_size = _data.sizeList();
	for (size_t imagetIteration = 0; imagetIteration < question_size; imagetIteration++)
	{
			auto image_in = _data[imagetIteration];
			// write value to inputs
			for (size_t index = 0; index < image_in.size(); index++)
			{
				
				(*RNNnet)[ "I-" + std::to_string(index) ] = (unsigned char)image_in.imageSource[index] / 255.0;
			}

			// calculate 
			RNNnet->process();

			size_t answare = 0;
			double answare_value = 0.0;
			for (size_t output_index = 0, output_size = RNNnet->get_output_size(); output_index < output_size; output_index++)
			{
				double answare_output = (*RNNnet)["O-" + std::to_string(output_index)];
				if (answare_output > answare_value)
				{
					answare_value = answare_output;
					answare = output_index;
				}
			}


			if (answare == (unsigned char)image_in.label)
			{
				this->correctAnswer++;
			}
			else
			{
				unsigned int number = (unsigned char)image_in.label;
				this->render(image_in, _data.getWidth(), _data.getHeight());
			std::cout << "\tNUMBER: " << number << "\tNETWORK ANSWARE: " << answare << std::endl;
			}
			
			this->samplings++;

			this->procent_correct_asware = ((float)this->correctAnswer) / (float)(this->samplings) * 100.0f;

	}
	std::cout << "\t%: " << this->procent_correct_asware << std::endl;
}

float RNN::procent_of_correct_asware()
{
	return this->procent_correct_asware;
}

void RNN::render(Image& image, unsigned int width, unsigned int height)
{
	// print image on console
	//system("cls");

	std::cout << "\n - - - - - - - - - - - - - - - \n" << std::endl;


	std::string string_image;
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			
			unsigned int pixel = (unsigned char)image.imageSource[y * width + x];
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

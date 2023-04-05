#include "../include/Neuralnetwork.h"


NeuralNetwork::~NeuralNetwork()
{
	
}

void NeuralNetwork::create_network()
{

	std::cout << " - - - - - NETWORK CREATING - - - - -\n";
	
	
	// przypisac IDneurons in/out do synaps

	std::vector<unsigned int> theConnection;
	for (unsigned int l = 0, s = 0; l < m_number_neurons_in_each_layers.size(); l++)
	{
		if (l > 0)
		{
			// ID started on each layer
			theConnection.push_back(m_number_neurons_in_each_layers[l - 1] + theConnection[l - 1]);


			for (unsigned int n = 0; n < m_number_neurons_in_each_layers[l]; n++)
			{
				unsigned int n_l_id = n + theConnection[l];

				// generating neurons
				Neuron neuronG;
				neuronG.m_id_number = n_l_id;

				for (unsigned int n_l_prev = 0; n_l_prev < m_number_neurons_in_each_layers[l - 1]; n_l_prev++, s++)
				{
					Synapse syn;

					unsigned int n_l_prev_id = n_l_prev + theConnection[l - 1];

					syn.m_id_number = s;

					//assigne IDs in/out and random m_weight
					syn.m_id_neuron_In = n_l_prev_id;
					syn.m_id_neuron_Out = n_l_id;
					syn.m_weight = random_double();

					m_synapse_list_Id.push_back(syn);

					// assigne synapseID to neuron list
					neuronG.m_id_synapses_list_In.push_back(syn.m_id_number);
				}

				// random bias. adding to pool
				neuronG.m_bias = random_double();
				m_neuron_list_Id.push_back(neuronG);
			}
		}
		else
		{
			// first ID started in input/first layer
			theConnection.push_back(l);

			// generating neurons for fist layer
			for (unsigned int n = 0; n < m_number_neurons_in_each_layers[l]; n++)
			{

				Neuron in;
				// giving ID
				in.m_id_number = n;
				m_neuron_list_Id.push_back(in);
			}
		}
	};
}

size_t NeuralNetwork::get_input_size()
{
	return this->m_number_neurons_in_each_layers[0];
}

size_t NeuralNetwork::get_output_size()
{
	return this->m_number_neurons_in_each_layers[this->m_number_neurons_in_each_layers.size()-1];
}

size_t NeuralNetwork::get_size_neurons()
{
	return this->m_neuron_list_Id.size();
}

size_t NeuralNetwork::get_size_synapses()
{
	return this->m_synapse_list_Id.size();
}

NeuralNetwork::NeuralNetwork(std::string _s)
{
	// removing whitespaces
	_s.erase(
		std::remove_if(
			_s.begin(), _s.end(), ::isspace
						),
		_s.end());

	if (!test_string(_s))
	{
		std::cout << "NeuralNetwork intializing: STRING is invalid\n";
		return;
	}

	srand((unsigned)time(NULL));
	this->m_name = _s;

	// making list from string
	std::istringstream input{ _s };
	for (std::string line; std::getline(input, line, '-');)
	{
		m_number_neurons_in_each_layers.push_back(std::stoi(line));
	};
	this->m_first_id_from_last_layer = this->first_id_from_output_layer();


	// if file not exist create new network
	if (!load_network(_s))
	{
		create_network();
	}

	//print();

}

void NeuralNetwork::process()
{
		
		//std::cout << "\n - - - - - PROCESSING - - - - -\n";

	for (auto& n:m_neuron_list_Id)
	{
		if (!n.m_id_synapses_list_In.empty())
		{
			double sum = n.m_bias;

			for (auto s_id : n.m_id_synapses_list_In)
			{

				sum += m_synapse_list_Id[s_id].m_weight * m_neuron_list_Id[m_synapse_list_Id[s_id].m_id_neuron_In].m_axon;

			}

			n.m_axon = 1 / (1 + exp(-sum));
		}

	}
	
	//print();
	
	//std::cout << "\n - - - - - - - - - - - - - - - \n";

}

double NeuralNetwork::random_double()
{
	//srand((unsigned)time(NULL));

	return ( 1 - ((double)rand() / (RAND_MAX / 2))); // random number beetwen -1 and 1.
}


void NeuralNetwork::backpropagation(std::vector<double> _neuronErr, std::vector<double> _synapseErr)
{
	//std::cout << "\n\t   BACKPROB: \n - - - - - PROCESSING - - - - -\n";

	// updating: biases and weights - learn ratio multiply by error
	if (this->m_neuron_list_Id.size() == _neuronErr.size() && this->m_synapse_list_Id.size() == _synapseErr.size())
	{
		for (size_t it = 0; it < m_synapse_list_Id.size(); it++)
		{
			m_synapse_list_Id[it].m_weight -= this->m_learn_ratio * _synapseErr[it];

			if (it < m_neuron_list_Id.size() )
			{
				m_neuron_list_Id[it].m_bias -= this->m_learn_ratio * _neuronErr[it];
			}

		}

	}
	else
	{
		std::cout << "\n BACKPROB: vectors are diffrent \n PROCESS TERMINATED \n";
	}

	//std::cout << "\n - - - - - - - - - - - - - - - \n";


}

void NeuralNetwork::calculate_network_error(std::vector<double>& _neuronErr, std::vector<double>& _synapseErr, std::vector<double>& _y)
{
	// vectors size check
	if(this->m_neuron_list_Id.size() != _neuronErr.size()) 
	{
		_neuronErr.resize(this->m_neuron_list_Id.size());
	}

	if (this->m_synapse_list_Id.size() != _synapseErr.size())
	{
		_synapseErr.resize(this->m_synapse_list_Id.size());
	}

	// first calculate Error for each neuron output from Y prediction
	for (unsigned int it=0, outputIt = this->m_first_id_from_last_layer; it < _y.size(); it++, outputIt++)
	{
		_neuronErr[outputIt] += (-(_y[it] - m_neuron_list_Id[outputIt].m_axon));
	}

	// calculating Error for neural network up to begining
	for (unsigned int it = this->m_neuron_list_Id.size()-1; it > this-> m_number_neurons_in_each_layers[0]; it--)
	{
					// error for current neuron multiply by error neuron from last layer
		double partialErr = _neuronErr[it] * (m_neuron_list_Id[it].m_axon * (1 - m_neuron_list_Id[it].m_axon));

		// first step - synaps error connected with itereted neuron
		for (unsigned int synapsIt = 0; synapsIt < m_neuron_list_Id[it].m_id_synapses_list_In.size(); synapsIt++)
		{
			unsigned int currentSynapseID = m_neuron_list_Id[it].m_id_synapses_list_In[synapsIt];
			// current synaps error
			_synapseErr[currentSynapseID] += partialErr * m_neuron_list_Id[ m_synapse_list_Id[ currentSynapseID].m_id_neuron_In].m_axon;
			
			// error neuron connected with synapse
			_neuronErr[m_synapse_list_Id[currentSynapseID].m_id_neuron_In] += partialErr * m_synapse_list_Id[currentSynapseID].m_weight;
		}


	}

}

unsigned int NeuralNetwork::first_id_from_output_layer()
{
	unsigned int ret = 0;
	for (size_t i = 0; i <  m_number_neurons_in_each_layers.size() - 1; i++)
	{
		ret+=  m_number_neurons_in_each_layers[i];
	}
	return ret;
}

void NeuralNetwork::set_learning_ratio(double _ratio)
{
	this->m_learn_ratio = _ratio;
}

double NeuralNetwork::get_network_error(std::vector<double>& _y)
{
	double ret=0;
	unsigned int output_index = this->m_first_id_from_last_layer;
	if (this->get_output_size() == _y.size())
	{

		for (size_t i = 0; i + output_index < this->m_neuron_list_Id.size(); i++)
		{
			double errorOut;
			errorOut = 0.5 * ((_y[i] - (this->m_neuron_list_Id[i + output_index].m_axon)) * (_y[i] - (this->m_neuron_list_Id[i + output_index].m_axon)));

			ret += errorOut;
		}
	}

	return ret;
}

void NeuralNetwork::print_All()
{
	for (auto& n : m_neuron_list_Id)
	{
		std::cout << "id N: " << n.m_id_number << " m_axon N: " << n.m_axon << " m_bias N: " << n.m_bias << " SynapseListSize N: \n - - - - - \n";
		for (auto& si : n.m_id_synapses_list_In)
		{
			std::cout << "id S: " << m_synapse_list_Id[si].m_id_number << " m_weight S: " << m_synapse_list_Id[si].m_weight << " idIn S: " << m_synapse_list_Id[si].m_id_neuron_In << " idOut S: " << m_synapse_list_Id[si].m_id_neuron_Out << std::endl;
		}
		std::cout << " - - - - - \n" << std::endl;
	}
}

void NeuralNetwork::print_Inputs()
{
	std::cout << "\n - - - - - - - - - - - - - - - \n           INPUTS\n";
	
	for (size_t i = 0; i < this-> m_number_neurons_in_each_layers[0]; i++)
	{
		std::cout << "\n Neuron " << i << " : " << this->m_neuron_list_Id[i].m_axon;
	}

	std::cout << "\n - - - - - - - - - - - - - - - \n";
}

void NeuralNetwork::print_Outputs()
{
	std::cout << "\n - - - - - - - - - - - - - - - \n          OUTPUTS\n";

	for (size_t i = this->m_first_id_from_last_layer; i < this->m_neuron_list_Id.size(); i++)
	{
		std::cout << "\n Neuron " << i << " : " << this->m_neuron_list_Id[i].m_axon;
	}

	std::cout << "\n - - - - - - - - - - - - - - - \n";
}

bool NeuralNetwork::save_network()
{
	std::string filename = "data/" + this->get_network_structure() + ".nnc";


	std::ofstream write_file(filename, std::ios::binary);

	if (!write_file.is_open())
	{
		write_file.close();
		return false;
	}
	// first save neuron vector size
	size_t vector_size = this->m_neuron_list_Id.size();
	write_file.write((char*)&vector_size, sizeof(vector_size));
	// save all neurons from vector
	for (Neuron& iter:this->m_neuron_list_Id)
	{
		write_file.write((char*)&iter.m_id_number, sizeof(iter.m_id_number));
		write_file.write((char*)&iter.m_axon, sizeof(iter.m_axon));
		write_file.write((char*)&iter.m_bias, sizeof(iter.m_bias));

		unsigned int size_vec = iter.m_id_synapses_list_In.size();
		write_file.write((char*)&size_vec, sizeof(size_vec));

		for (auto& i:iter.m_id_synapses_list_In)
		{
		write_file.write((char*)&i, sizeof(i));

		}

	}

	// save synapse vector size
	vector_size = this->m_synapse_list_Id.size();
	write_file.write((char*)&vector_size, sizeof(vector_size));
	// save all synapse from vector
	for (Synapse& iter : this->m_synapse_list_Id)
	{
		write_file.write((char*)&iter.m_id_number, sizeof(iter.m_id_number));
		write_file.write((char*)&iter.m_weight, sizeof(iter.m_weight));
		write_file.write((char*)&iter.m_id_neuron_In, sizeof(iter.m_id_neuron_In));
		write_file.write((char*)&iter.m_id_neuron_Out, sizeof(iter.m_id_neuron_Out));
	}

	write_file.close();

	return true;
}

bool NeuralNetwork::load_network(std::string _name)
{
	this->m_neuron_list_Id.clear();
	this->m_synapse_list_Id.clear();

	std::string filename = "data/" + _name + ".nnc";

	std::ifstream open_file(filename, std::ios::in | std::ios::binary);

	if (!open_file.is_open())
	{
		open_file.close();
		return false;
	}
	size_t vector_size;
	open_file.read((char*)&vector_size, sizeof(vector_size));

	for (size_t iter = 0; iter < vector_size; iter++)
	{
		Neuron new_neuron;
		unsigned int new_int;

		open_file.read((char*)&new_neuron.m_id_number, sizeof(new_neuron.m_id_number));

		open_file.read((char*)&new_neuron.m_axon, sizeof(new_neuron.m_axon));

		open_file.read((char*)&new_neuron.m_bias, sizeof(new_neuron.m_bias));

		open_file.read((char*)&new_int, sizeof(new_int));
		for (size_t i = 0, list=new_int; i < list; i++)
		{
			open_file.read((char*)&new_int, sizeof(new_int));
			new_neuron.m_id_synapses_list_In.push_back(new_int);
		}

		this->m_neuron_list_Id.push_back(new_neuron);
	}

	open_file.read((char*)&vector_size, sizeof(vector_size));

	for (size_t i = 0; i < vector_size; i++)
	{
		Synapse new_synapse;

		open_file.read((char*)&new_synapse.m_id_number, sizeof(new_synapse.m_id_number));
		open_file.read((char*)&new_synapse.m_weight, sizeof(new_synapse.m_weight));
		open_file.read((char*)&new_synapse.m_id_neuron_In, sizeof(new_synapse.m_id_neuron_In));
		open_file.read((char*)&new_synapse.m_id_neuron_Out, sizeof(new_synapse.m_id_neuron_Out));
		
		this->m_synapse_list_Id.push_back(new_synapse);
	}

	open_file.close();

	return true;
}

std::string NeuralNetwork::get_network_structure()
{
	std::string ret;

	for (size_t i=0, last=this-> m_number_neurons_in_each_layers.size()-1; i < this-> m_number_neurons_in_each_layers.size();i++)
	{
		ret += std::to_string(this-> m_number_neurons_in_each_layers[i]);

		if (i < last) ret += "-";
	}

	return ret;
}

double& NeuralNetwork::operator[](std::string _in_out_neuron)
{
	std::transform(_in_out_neuron.begin(), _in_out_neuron.end(), _in_out_neuron.begin(), ::toupper);

	if (!std::regex_match(_in_out_neuron, std::regex("^[IO]-[0-9]*$")))
	{
		std::cout << "NeuralNetwork::operator[] - wrong string syntax\n";
		
	};

	std::string io_string;
	unsigned int identCall;
	// making list from string
	std::istringstream string_input{ _in_out_neuron };
	for (std::string line; std::getline(string_input, line, '-');)
	{
		if (line == "I")
		{
			io_string = line;
		}
		else if (line == "O")
		{
			io_string = line;
		}else
		identCall = std::stoi(line);
	};


	if (io_string == "I")
	{
		
		if (identCall < this->get_input_size())
		{
			return this->m_neuron_list_Id[identCall].m_axon;
		}
		else 
		{
			std::cout << "NeuralNetwork::operator[] - Input index out of range\n";
		}
	}

	if (io_string == "O")
	{
		unsigned int out_size = this->get_output_size();
		if (identCall < out_size)
		{
			size_t idx = identCall + this->m_first_id_from_last_layer;
			return this->m_neuron_list_Id[idx].m_axon;
		}
		else
		{
			std::cout << "NeuralNetwork::operator[] - Output index out of range\n";
		}
	}
}

bool NeuralNetwork::test_string(std::string _s)
{ 
	bool ret =std::regex_match(_s, std::regex("^[0-9-]*[0-9]$"));

		return ret;
}
;



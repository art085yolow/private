#include "../include/Neuralnetwork.h"

// NeuralNetwork::NeuralNetwork(size_t _arch_count, size_t* _arch)
// {
// 	this->set_neuralnetwork(_arch_count, _arch);
// }
// 
// NeuralNetwork::NeuralNetwork(NeuralNetwork& _Network_copy)
// {
// 	this->m_data = _Network_copy.get_data();
// 	this->create_container(false);
// }
// 
// NeuralNetwork::NeuralNetwork(std::string _file_name)
// {
// 	this->load(_file_name);
// }
// 
// NeuralNetwork::NeuralNetwork()
// {
// }

//void NeuralNetwork::operator=(NeuralNetwork& _copy_from)
//NeuralNetwork& NeuralNetwork::operator=(NeuralNetwork& _copy_from)
//{
//	this->m_data = _copy_from.m_data;
//	this->create_container(false);
//	return *this;
//}

NN NeuralNetwork::get_nn()
{
	this->create_nn();
	this->set_act();
	return this->m_nn;
}

double NeuralNetwork::get_fitness()
{
	return this->m_fitness_score;
}

void NeuralNetwork::add_fitness_score(double _score)
{
	this->m_fitness_score += _score;
}

void NeuralNetwork::clear_fitness_score()
{
	this->m_fitness_score = 0;
}

std::vector<float>& NeuralNetwork::get_genom()
{
	return this->m_data.m_container_wsbs;
}

void NeuralNetwork::set_genom(std::vector<float>& _genom)
{
	this->m_data.m_container_wsbs = _genom;
}

NetworkData& NeuralNetwork::get_data()
{
	return this->m_data;
}

void NeuralNetwork::set_neuralnetwork(size_t _arch_count, size_t* _arch)
{
	this->m_data.m_arch.resize(_arch_count);
	for (size_t i = 0; i < _arch_count; i++)
	{
		this->m_data.m_arch[i] = _arch[i];
	}

	this->create_container();

}

void NeuralNetwork::set_neuralnetwork(NeuralNetwork& _Network_copy)
{
	this->m_data = _Network_copy.get_data();
	this->create_container(false);
}

void NeuralNetwork::save(std::string _filename)
{
	std::string path = "data\\" + _filename + ".netnn";

	//

	std::ofstream write_file(path, std::ios::binary);

	if (!write_file.is_open())
	{
		write_file.close();
		printf("Saving failed...\n");
		return;
	}

	// saving nn arch 
	write_file.write((char*)&this->m_nn.arch_count, sizeof(this->m_nn.arch_count));
	for (size_t i = 0; i < this->m_data.m_arch.size(); i++)
	{
		write_file.write((char*)&this->m_data.m_arch[i], sizeof(size_t));
	}


	// saving weights and biases
	for (float& wbs : this->m_data.m_container_wsbs)
	{
		write_file.write((char*)&wbs, sizeof(float));
	}

	for (Act& act : this->m_data.m_activation_type)
	{
		write_file.write((char*)&act, (sizeof(act)));
	}

	write_file.close();

	printf("Saved...\n");

}

void NeuralNetwork::load(std::string _filename)
{
	std::string path = "data\\" + _filename + ".netnn";

	std::ifstream open_file(path, std::ios::in | std::ios::binary);

	if (!open_file.is_open())
	{
		open_file.close();
		printf("Failed to load file...\n");
		exit(-1);
	}

	// get architecture
	open_file.read((char*)&this->m_nn.arch_count, sizeof(this->m_nn.arch_count));

	this->m_data.m_arch.resize(this->m_nn.arch_count);

	for (size_t i = 0; i < this->m_data.m_arch.size(); i++)
	{
		open_file.read((char*)&this->m_data.m_arch[i], sizeof(this->m_data.m_arch[i]));
	}

	// create template
	this->create_container();

	// load data
	for (float& wbs : this->m_data.m_container_wsbs)
	{
		open_file.read((char*)&wbs, sizeof(wbs));
	}


	for (Act& act : this->m_data.m_activation_type)
	{
		open_file.read((char*)&act, sizeof(act));
	}

	this->create_nn();
	this->set_act();

	open_file.close();

	printf("Loaded...\n");
}

void NeuralNetwork::set_activation(size_t _layer, Act _act_typ)
{
	if (!(_layer > 0) || !(_layer < this->m_data.m_arch.size()))
	{
		printf("Number is out of range layers.\n");
		exit(-2);
	}

	this->m_data.m_activation_type[_layer - 1] = _act_typ;
}

void NeuralNetwork::set_activation(std::vector<Act> _act_typ)
{
	this->m_data.m_activation_type = _act_typ;
}

void NeuralNetwork::create_container(bool _new_net)
{
	size_t ws_size=0;
	size_t bs_size=0;
	size_t as_size=0;

	as_size += this->m_data.m_arch[0];

	for (size_t i = 1; i < this->m_data.m_arch.size(); i++)
	{
		ws_size += this->m_data.m_arch[i - 1] * this->m_data.m_arch[i];
		bs_size += this->m_data.m_arch[i];
		as_size += this->m_data.m_arch[i];
	}

	if (_new_net)
	{
		this->m_data.m_container_wsbs.resize(ws_size + bs_size, 0.0f);
		this->m_data.m_activation_type.resize(this->m_data.m_arch.size() - 1);
	}

	this->m_container_as.resize(as_size, 0.0f);
}

void NeuralNetwork::create_nn()
{
	this->m_nn.arch_count = this->m_data.m_arch.size();
	this->m_nn.arch = &(this->m_data.m_arch[0]);

	this->m_nn.ws = new Mat[this->m_nn.arch_count-1];
	this->m_nn.bs = new Row[this->m_nn.arch_count-1];
	this->m_nn.as = new Row[this->m_nn.arch_count];

	size_t index = 0;
	size_t index_a = 0;

	this->m_nn.as[0].cols = this->m_nn.arch[0];
	this->m_nn.as[0].elements = &(this->m_container_as[0]);

	index_a = this->m_nn.arch[0];


	for (size_t l = 1; l < this->m_nn.arch_count; l++)
	{
		this->m_nn.ws[l-1].rows = this->m_nn.arch[l - 1];
		this->m_nn.ws[l-1].cols = this->m_nn.arch[l];
		this->m_nn.ws[l-1].elements = &(this->m_data.m_container_wsbs[index]);

		this->m_nn.as[l].cols = this->m_nn.arch[l];
		this->m_nn.as[l].elements = &(this->m_container_as[index_a]);
		index_a += this->m_nn.arch[l];

		index += this->m_nn.ws[l-1].rows * this->m_nn.ws[l-1].cols;
	}


	for (size_t l = 1; l < this->m_nn.arch_count; l++)
	{
		this->m_nn.bs[l - 1].cols = this->m_nn.arch[l];
		this->m_nn.bs[l - 1].elements = &(this->m_data.m_container_wsbs[index]);
		index += this->m_nn.bs[l - 1].cols;
	}

}

void NeuralNetwork::set_act()
{
	for (size_t i = 0; i < this->m_data.m_activation_type.size(); i++)
	{
		this->m_nn.as[i + 1].act = this->m_data.m_activation_type[i];
	}
}

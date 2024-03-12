#pragma once

#include <vector>
#include "nn.h"
#include "Read_file.h"

struct NetworkData {
	std::vector<float> m_container_wsbs;
	std::vector<size_t> m_arch;
	std::vector<Act> m_activation_type;
};

struct NeuralNetwork
{
public:
	// NeuralNetwork(size_t _arch_count, size_t* _arch);
	// NeuralNetwork(NeuralNetwork& _Network_copy);
	// NeuralNetwork(std::string _file_name);
	// NeuralNetwork();

	//void operator=(NeuralNetwork& _copy_from);
	//NeuralNetwork& operator=(NeuralNetwork& _copy_from);

	NN get_nn();
	double get_fitness();
	void add_fitness_score(double _score);
	void clear_fitness_score();

	std::vector<float>& get_genom();
	void set_genom(std::vector<float>& _genom);
	NetworkData& get_data();

	// for new nn or change. It reseting stats
	void set_neuralnetwork(size_t _arch_count, size_t* _arch);
	void set_neuralnetwork(NeuralNetwork& _Network_copy);

	void save(std::string _filename);
	void load(std::string _filename);

	void set_activation(size_t _layer, Act _act_typ);
	void set_activation( std::vector<Act> _act_typ);

private:

	void create_container(bool _new_net= true);
	void create_nn();
	void set_act();


	NetworkData m_data; 
	std::vector<float> m_container_as;
	NN m_nn;
	double m_fitness_score=0;
};
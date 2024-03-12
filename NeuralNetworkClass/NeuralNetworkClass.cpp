#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <Windows.h>
#include <algorithm>

#include "include/Neuralnetwork.h"
#include "include/Read_file.h"

void gate_test(std::string _filename_save);
void gate_test(std::string _filename_load, std::string _filename_save);
void hand_digit_train(size_t _max_epoch, std::string _filename_save);
void hand_digit_load_file(size_t _max_epoch, std::string _filename_load, std::string _filename_save);

void arrays_nn(size_t _max_epoch, std::string _filename, size_t _pool_size, size_t _pool_size_mult, float _mutation_rate);

int main()
{
	std::cout << "Hello World!\n";
	
	
	///  STEP 1
	/*
		The schemat for each layer look:

		INPUT					->		* WEIGHT	+ BIAS		->	OUTPUT

		so:

		OUTPUT (LAST LAYER)		->		* WEIGHT	+ BIAS		->	INPUT (NEXT LAYER)

		That means I need 3 types of matrices group.
		1. INPUTS/OUTPUTS -> identified as 'a' in many ann documentation.
		2. WEIGHTS -> identified as 'w' in ann documentation.
		3. BIASES -> identified as 'b' in ann documentation.


		L0			L1			L2
		INPUT		HIDDEN		OUTPUT
		a0			a1			a2
		w0			w1			w2
		b0			b1			b1

		2x2x1

			a0					a1					a2
			2					2					1
			col	
	row	|	I1, I2	|		|	H1, H2	|		|	O1	|

			w0					w1
			2*2					2*1
	row	|	w1, w2	|		|	w1	|
	row	|	w3, w4	|		|	w2	|

			b0					b1
			2					1					|				idea is: a0 * w0 + b0 = a1 ect...
	row	|	b1, b2	|		|	b1	|

		evade vectors -> use array

		something to keep pseudo matrices, so goal is to read 1-dim array as 2-dim so for that
		need struct of:

		rows size												|	in terms of x,y coords is y | or how many examples
		cols size												|	in terms of x,y coords is x | or how many inputs
		stride side -> if using part of the data				|	most cases =cols, but if orgin (attach to value pointer) data for expl: OR gate 0,0,0 (and 3 other ) mat_in row=4,col=2,stride=3 - mat_out row=4,col=1,stride=3
		double* value -> array of data to read

	*/

	// gate_test("test_gate");
	// gate_test("test_gate", "test_gate2");

	// hand_digit_train( 10 * 50,"test3");
	
	//hand_digit_load_file(2 * 50, "test3", "test3");
	//hand_digit_load_file(100 * 50, "test1tanhtanhsig", "test1tanhtanhsig");
	//hand_digit_load_file(1 * 50, "test1tahnrelusig", "test2tahnrelusig");
	//hand_digit_load_file(1 * 50, "test2tahnrelusig", "test1tahnrelusig");
	//hand_digit_load_file(1 * 50, "test2act", "test1act");

	arrays_nn(100 * 50, "gens", 100, 8, 0.01f);

	return 0;
};

void gate_test(std::string _filename_save)
{
	float or_gate[] = {
		0,0,0,
		0,1,1,
		1,0,1,
		1,1,1
	};

	float and_gate[] = {
		0,0,0,
		0,1,0,
		1,0,0,
		1,1,1
	};

	float nand_gate[] = {
		0,0,1,
		0,1,1,
		1,0,1,
		1,1,0
	};

	float nor_gate[] = {
		0,0,1,
		0,1,0,
		1,0,0,
		1,1,0
	};

	float xor_gate[] = {
		0,0,0,
		0,1,1,
		1,0,1,
		1,1,0
	};

	// assigne what gate you want to test
	float* training_data = xor_gate;

	Mat m_train = {
		.rows = 4,
		.cols = 3,
		.elements = training_data
	};


	MAT_PRINT(m_train);
	printf("\n");

	size_t arch[] = { 2,2,1 };

	NeuralNetwork net_nn;
	net_nn.set_neuralnetwork(ARRAY_LEN(arch), arch);

	NN nn = net_nn.get_nn();
	//NN nn = nn_alloc(NULL, arch, ARRAY_LEN(arch));
	nn_rand(nn, -1.0f, 1.0f);

	// nn.as[1].act = ACT_RELU;
	// nn.as[2].act = ACT_SIN;

	net_nn.set_activation(1, ACT_RELU);
	net_nn.set_activation(2, ACT_TANH);

	double cost = nn_cost(nn, m_train);
	double rate = 5e-2;

	printf("\n%f\n", cost);
	Batch batch = { 0 };
	size_t batches_per_frame = 200;
	size_t batch_size = 50;
	size_t epoch = 0;
	size_t max_epoch = 1000 * 1000;
	
	while (cost > 0.001 && epoch < max_epoch)
	{
		for (size_t i = 0; i < batches_per_frame ; ++i) {
			batch_process(NULL, &batch, batch_size, nn, m_train, rate);
			if (batch.finished) {
				epoch += 1;
				batch.cost = 0;
				cost = nn_cost(nn, m_train);
			}
		}
	}


	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			MAT_AT(NN_INPUT(nn), 0, 0) = (float)i;
			MAT_AT(NN_INPUT(nn), 0, 1) = (float)j;
			nn_forward(nn);
			printf("%zu ^ %zu = %f\n", i, j, MAT_AT(NN_OUTPUT(nn), 0, 0));
		}
	}

	//save_data_network(nn, "data\\" + _filename_save + ".nn");
	net_nn.save(_filename_save);

	printf("\n");
	NN_PRINT(nn);
	printf("\n%f\n", cost);

	for (float& f : net_nn.get_genom())
	{
		std::cout << f << " ";
	}
	printf("\n");
	printf("\n");
	printf("\n");

	//nn_deallocation(nn);
}

void gate_test(std::string _filename_load, std::string _filename_save)
{
	float or_gate[] = {
		0,0,0,
		0,1,1,
		1,0,1,
		1,1,1
	};

	float and_gate[] = {
		0,0,0,
		0,1,0,
		1,0,0,
		1,1,1
	};

	float nand_gate[] = {
		0,0,1,
		0,1,1,
		1,0,1,
		1,1,0
	};

	float nor_gate[] = {
		0,0,1,
		0,1,0,
		1,0,0,
		1,1,0
	};

	float xor_gate[] = {
		0,0,0,
		0,1,1,
		1,0,1,
		1,1,0
	};

	// assigne what gate you want to test
	float* training_data = xor_gate;

	Mat m_train = {
		.rows = 4,
		.cols = 3,
		.elements = training_data
	};


	MAT_PRINT(m_train);
	printf("\n");

	size_t arch[] = { 2,2,1 };

	NeuralNetwork net_nn;
	net_nn.load(_filename_load);
	NN nn = net_nn.get_nn();
	//NN nn = load_data_network("data\\" + _filename_load + ".nn");

	//NN nn = nn_alloc(NULL, arch, ARRAY_LEN(arch));
	//nn_rand(nn, -1.0f, 1.0f);

	double cost = nn_cost(nn, m_train);
	double rate = 5e-2;

	printf("\n%f\n", cost);
	Batch batch = { 0 };
	size_t batches_per_frame = 200;
	size_t batch_size = 50;
	size_t epoch = 0;
	size_t max_epoch = 1000 * 1000;

	while (cost > 0.001)
	{
		for (size_t i = 0; i < batches_per_frame && epoch < max_epoch; ++i) {
			batch_process(NULL, &batch, batch_size, nn, m_train, rate);
			if (batch.finished) {
				epoch += 1;
				batch.cost = 0;
				cost = nn_cost(nn, m_train);
			}
		}
	}

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			MAT_AT(NN_INPUT(nn), 0, 0) = (float)i;
			MAT_AT(NN_INPUT(nn), 0, 1) = (float)j;
			nn_forward(nn);
			printf("%zu ^ %zu = %f\n", i, j, MAT_AT(NN_OUTPUT(nn), 0, 0));
		}
	}
	//save_data_network(nn, "data\\" + _filename_save + ".nn");

	net_nn.save(_filename_save);

	printf("\n");
	NN_PRINT(nn);
	printf("\n%f\n", cost);
	printf("\n");
	printf("\n");
	printf("\n");

	//nn_deallocation(nn);
}

void hand_digit_train(size_t _max_epoch, std::string _filename_save)
{
	// data files mnist
	// 60k handwriten digits
	Mat train_image = load_file_to_matrix("testData/train-images.idx3-ubyte", "testData/train-labels.idx1-ubyte");
	// 10k handwriten digits
	Mat test_image = load_file_to_matrix("testData/t10k-images.idx3-ubyte", "testData/t10k-labels.idx1-ubyte");


	// network structure
	// 784-16-16-10
	size_t arch[] = { 28 * 28, 17, 16, 10 };

	NeuralNetwork net_nn;
	net_nn.set_neuralnetwork(ARRAY_LEN(arch), arch);
	NN nn = net_nn.get_nn();
	nn_rand(nn, -1.0f, 1.0f);


	net_nn.set_activation(1, ACT_TANH);
	net_nn.set_activation(2, ACT_RELU);
	net_nn.set_activation(3, ACT_SIG);

	// nn.as[1].act = ACT_TANH;
	// nn.as[2].act = ACT_TANH;
	// nn.as[3].act = ACT_SIG;


	int answ_corr = 0;

	for (size_t i = 0; i < test_image.rows; i++)
	{
		Row row = mat_row(test_image, i);
		Row in = row_slice(row, 0, NN_INPUT(nn).cols);
		Row out = row_slice(row, NN_INPUT(nn).cols, NN_OUTPUT(nn).cols);
		row_copy(NN_INPUT(nn), in);

		nn_forward(nn);

		size_t hightest_val_answ = 0;
		double hightest_val_d_answ = 0;
		bool get_answ = 0;
		size_t y_answ = 0;

		for (size_t out_nn_answ = 0; out_nn_answ < NN_OUTPUT(nn).cols; out_nn_answ++)
		{
			if (MAT_AT(NN_OUTPUT(nn), 0, out_nn_answ) > hightest_val_d_answ)
			{
				hightest_val_d_answ = MAT_AT(NN_OUTPUT(nn), 0, out_nn_answ);
				hightest_val_answ = out_nn_answ;
			}

			if (ROW_AT(out, out_nn_answ) == 1.0f)
			{
				y_answ = out_nn_answ;
			}
		}

		get_answ = hightest_val_d_answ > 0.5f ? 1 : 0;
		get_answ = get_answ ? hightest_val_answ == y_answ : 0;


		answ_corr += get_answ;

	};
	printf(" NN_FILE:: correct answares %5d\n", answ_corr);


	double cost = nn_cost(nn, test_image);
	printf("START::cost %f \n", cost);

	Batch batch = { 0 };

	size_t epoch = 0;
	//size_t max_epoch = 1000 * 1000;
	size_t last_corr = 0;
	size_t batch_size = 50;
	size_t batches_per_frame = 200;
	float rate = 0.9f;

	bool b_key[7](false);

	while (answ_corr < test_image.rows - 100 && epoch < _max_epoch && !b_key[6])
	{
		for (int8_t k = 0; k < 7; k++)
			b_key[k] = (0x8000 & GetAsyncKeyState((unsigned char)("ASDFGHQ"[k]))) != 0;

		if (answ_corr < 7000 || b_key[0]) rate = 0.9f;
		else if (answ_corr < 8500 || b_key[1]) rate = 0.5f;
		else if (answ_corr < 8700 || b_key[2]) rate = 0.25f;
		else if (answ_corr < 9000 || b_key[3]) rate = 0.1f;
		else if (answ_corr < 9100) ;
		else if ((answ_corr < 9550 && rate > 0.00000001f) || (rate>0.00000001f && b_key[4])) rate -= 0.00000001f;
		//else if (answ_corr < 9300) rate -= 0.00001f;
		//else if (answ_corr < 9340) rate -= 0.00000001f;

		answ_corr = 0;

		for (size_t i = 0; i < batches_per_frame && epoch < _max_epoch; ++i) {
			batch_process(NULL, &batch, batch_size, nn, train_image, rate);
			if (batch.finished) {
				epoch += 1;
				batch.cost = 0;
				mat_shuffle_rows(train_image);
			}
		}


		for (size_t i = 0; i < test_image.rows; i++)
		{
			Row row = mat_row(test_image, i);
			Row in = row_slice(row, 0, NN_INPUT(nn).cols);
			Row out = row_slice(row, NN_INPUT(nn).cols, NN_OUTPUT(nn).cols);
			row_copy(NN_INPUT(nn), in);

			nn_forward(nn);

			size_t hightest_val_answ = 0;
			double hightest_val_d_answ = 0;
			bool get_answ = 0;
			size_t y_answ = 0;

			for (size_t out_nn_answ = 0; out_nn_answ < NN_OUTPUT(nn).cols; out_nn_answ++)
			{
				if (MAT_AT(NN_OUTPUT(nn), 0, out_nn_answ) > hightest_val_d_answ)
				{
					hightest_val_d_answ = MAT_AT(NN_OUTPUT(nn), 0, out_nn_answ);
					hightest_val_answ = out_nn_answ;
				}

				if (ROW_AT(out, out_nn_answ) == 1.0f)
				{
					y_answ = out_nn_answ;
				}
			}

			get_answ = hightest_val_d_answ > 0.5f ? 1 : 0;
			get_answ = get_answ ? hightest_val_answ == y_answ : 0;


			answ_corr += get_answ;

		}

		cost = nn_cost(nn, test_image);
		printf(" TRAIN:: correct %4i, epoch %3i, cost %f, highest %4i, rate %f\n", (int)answ_corr, (int)epoch, cost, (int)last_corr, rate);

		if (answ_corr > last_corr)
		{
			last_corr = answ_corr;
			//save_networkdata(net_nn, "data\\" + _filename_save + ".nn");
			net_nn.save(_filename_save);
		}

	};


	cost = nn_cost(nn, test_image);
	printf(" END:: cost %f\n", cost);

	//nn_deallocation(nn);
}

void hand_digit_load_file(size_t _max_epoch, std::string _filename_load, std::string _filename_save)
{
	// data files mnist
	// 60k handwriten digits
	Mat train_image = load_file_to_matrix("testData/train-images.idx3-ubyte", "testData/train-labels.idx1-ubyte");
	// 10k handwriten digits
	Mat test_image = load_file_to_matrix("testData/t10k-images.idx3-ubyte", "testData/t10k-labels.idx1-ubyte");


	// network structure
	// 784-16-16-10
	size_t arch[] = { 28 * 28, 16, 16, 10 };

	NeuralNetwork nn_net;
	nn_net.load(_filename_load);
	NN nn = nn_net.get_nn();
	
	//NN nn = load_data_network("data\\" + _filename_load + ".nn");

	int answ_corr = 0;

//#define NN_BACKPROP_TRADITIONAL

	for (size_t i = 0; i < test_image.rows; i++)
	{
		Row row = mat_row(test_image, i);
		Row in = row_slice(row, 0, NN_INPUT(nn).cols);
		Row out = row_slice(row, NN_INPUT(nn).cols, NN_OUTPUT(nn).cols);
		row_copy(NN_INPUT(nn), in);

		nn_forward(nn);

		size_t hightest_val_answ = 0;
		double hightest_val_d_answ = 0;
		bool get_answ = 0;
		size_t y_answ = 0;

		for (size_t out_nn_answ = 0; out_nn_answ < NN_OUTPUT(nn).cols; out_nn_answ++)
		{
			if (MAT_AT(NN_OUTPUT(nn), 0, out_nn_answ) > hightest_val_d_answ)
			{
				hightest_val_d_answ = MAT_AT(NN_OUTPUT(nn), 0, out_nn_answ);
				hightest_val_answ = out_nn_answ;
			}

			if (ROW_AT(out, out_nn_answ) == 1.0f)
			{
				y_answ = out_nn_answ;
			}
		}

		get_answ = hightest_val_d_answ > 0.5f ? 1 : 0;
		get_answ = get_answ ? hightest_val_answ == y_answ : 0;


		answ_corr += get_answ;

	};
	printf(" NN_FILE:: correct answares %5d\n", answ_corr);
	
	int start_f = answ_corr;


	double cost = nn_cost(nn, test_image);
	printf("START::cost %f \n", cost);

	Batch batch = { 0 };

	size_t epoch = 0;
	//size_t max_epoch = 1000 * 1000;
	size_t last_corr = 0;
	size_t batch_size = 50;
	size_t batches_per_frame = 200;
	float rate = 0.9f;

	bool b_key[7](false);

	while (answ_corr < test_image.rows - 100 && epoch < _max_epoch && !b_key[6])
	{
		for (int8_t k = 0; k < 7; k++)
			b_key[k] = (0x8000 & GetAsyncKeyState((unsigned char)("ASDFGHQ"[k]))) != 0;

		if (answ_corr < 7000 || b_key[0]) rate = 0.9f;
		else if (answ_corr < 8500 || b_key[1]) rate = 0.5f;
		else if (answ_corr < 8700 || b_key[2]) rate = 0.25f;
		else if (answ_corr < 9000 || b_key[3]) rate = 0.1f;
		else if (answ_corr < 9100);
		else if (answ_corr < 9200 || b_key[4]) rate = 0.05f;
		else if ((answ_corr < 9550 && rate > 0.00000001f) || (rate > 0.00000001f && b_key[5])) rate -= 0.00000001f;

		answ_corr = 0;

		for (size_t i = 0; i < batches_per_frame && epoch < _max_epoch; ++i) {
			batch_process(NULL, &batch, batch_size, nn, train_image, rate);
			if (batch.finished) {
				epoch += 1;
				batch.cost = 0;
				mat_shuffle_rows(train_image);
			}
		}


		for (size_t i = 0; i < test_image.rows; i++)
		{
			Row row = mat_row(test_image, i);
			Row in = row_slice(row, 0, NN_INPUT(nn).cols);
			Row out = row_slice(row, NN_INPUT(nn).cols, NN_OUTPUT(nn).cols);
			row_copy(NN_INPUT(nn), in);

			nn_forward(nn);

			size_t hightest_val_answ = 0;
			double hightest_val_d_answ = 0;
			bool get_answ = 0;
			size_t y_answ = 0;

			for (size_t out_nn_answ = 0; out_nn_answ < NN_OUTPUT(nn).cols; out_nn_answ++)
			{
				if (MAT_AT(NN_OUTPUT(nn), 0, out_nn_answ) > hightest_val_d_answ)
				{
					hightest_val_d_answ = MAT_AT(NN_OUTPUT(nn), 0, out_nn_answ);
					hightest_val_answ = out_nn_answ;
				}

				if (ROW_AT(out, out_nn_answ) == 1.0f)
				{
					y_answ = out_nn_answ;
				}
			}

			get_answ = hightest_val_d_answ > 0.5f ? 1 : 0;
			get_answ = get_answ ? hightest_val_answ == y_answ : 0;


			answ_corr += get_answ;

		}

		cost = nn_cost(nn, test_image);
		printf(" TRAIN:: correct %4i, epoch %3i, cost %f, highest %4i, rate %f\n", (int)answ_corr, (int)epoch, cost, (int)last_corr, rate);

		if (answ_corr > last_corr && answ_corr > start_f)
		{
			last_corr = answ_corr;
			//save_data_network(nn, "data\\" + _filename_save + ".nn");
			nn_net.save(_filename_save);
		}

	};


	cost = nn_cost(nn, test_image);
	printf(" END:: cost %f\n", cost);


	//nn_deallocation(nn);
}


bool compare_by_score(const std::pair<double, size_t>& a, const std::pair<double, size_t>& b) {

	return a.first > b.first;
};

NeuralNetwork get_offsprings(NeuralNetwork _a, NeuralNetwork _b, float mutation_rate)
{	
	NeuralNetwork ret = _a;
	// crossover
	size_t number_of_crossover_points = 2;
	std::vector<size_t> crossovers_points(number_of_crossover_points);

	srand(time(0));

	for (auto& i : crossovers_points)
	{
		i = rand() % _a.get_genom().size();
	}

	for (size_t i = 0; i < number_of_crossover_points - 1; i++)
	{
		for (size_t j = crossovers_points[i]; j < crossovers_points[i + 1]; j++)
		{
			ret.get_genom()[j] = _b.get_genom()[j];

		}
	}

	// mutation

	for (size_t i = 0; i < _a.get_genom().size(); i++)
	{

		unsigned int* binary_representation = reinterpret_cast<unsigned int*>(&_a.get_genom()[i]);
		unsigned int* binary_representation_2 = reinterpret_cast<unsigned int*>(&_b.get_genom()[i]);
		size_t num_bits = sizeof(float) * 8;

		for (size_t j = 0; j < num_bits; j++)
		{
			//srand(time(0));
			if ((rand() / (RAND_MAX + 1.0)) <  mutation_rate)
			{
				*binary_representation ^= (1u << j);
			}

			//srand(time(0));
			if ((rand() / (RAND_MAX + 1.0)) < mutation_rate )
			{
				*binary_representation_2 ^= (1u << j);
			}
		}

	}
	return _a;
}

void chunk_process(NeuralNetwork& _chunky, Matrix _data)
{
	// individual evaluation score

	_chunky.clear_fitness_score();
		NN nn = _chunky.get_nn();
		int answ_corr = 0;
		for (size_t i = 0; i < _data.get_mat().rows; i++)
		{
			Row row = mat_row(_data.get_mat(), i);
			Row in = row_slice(row, 0, NN_INPUT(nn).cols);
			Row out = row_slice(row, NN_INPUT(nn).cols, NN_OUTPUT(nn).cols);
			row_copy(NN_INPUT(nn), in);

			nn_forward(nn);

			size_t hightest_val_answ = 0;
			double hightest_val_d_answ = 0;
			bool get_answ = 0;
			size_t y_answ = 0;

			for (size_t out_nn_answ = 0; out_nn_answ < NN_OUTPUT(nn).cols; out_nn_answ++)
			{
				if (MAT_AT(NN_OUTPUT(nn), 0, out_nn_answ) > hightest_val_d_answ)
				{
					hightest_val_d_answ = MAT_AT(NN_OUTPUT(nn), 0, out_nn_answ);
					hightest_val_answ = out_nn_answ;
				}

				if (ROW_AT(out, out_nn_answ) == 1.0f)
				{
					y_answ = out_nn_answ;
				}
			}

			get_answ = hightest_val_d_answ > 0.5f ? 1 : 0;
			get_answ = get_answ ? hightest_val_answ == y_answ : 0;


			answ_corr += get_answ;

		};
		
		_chunky.add_fitness_score((size_t)answ_corr);
	
}

void arrays_nn(size_t _max_epoch, std::string _filename, size_t _pool_size, size_t _pool_size_mult, float _mutation_rate)
{
	float mutation_rate = 0.01f;
	// pool newborns
	std::vector<NeuralNetwork> nn_list(_pool_size * _pool_size_mult);
	
	// prep for threads
	const size_t number_threads = std::thread::hardware_concurrency();
	size_t chunck_size = nn_list.size() / number_threads;
	std::vector<std::thread> threads;


	srand(time(0));

	// architecture struct
	size_t arch[] = {
		28 * 28, 16, 16, 10
	};

	//std::vector<Act> act_list{ ACT_TANH, ACT_RELU,ACT_SIG };


	// activation pool
	for (NeuralNetwork& nn : nn_list)
	{
		nn.set_neuralnetwork(ARRAY_LEN(arch), arch);
		//nn.set_activation(act_list);
		nn_rand(nn.get_nn(), -1, 1);
	}



	// data files mnist
	// 60k handwriten digits train
	//Mat train_image = load_file_to_matrix("testData/train-images.idx3-ubyte", "testData/train-labels.idx1-ubyte");
	// 10k handwriten digits test
	Matrix test_image( load_file_to_matrix("testData/t10k-images.idx3-ubyte", "testData/t10k-labels.idx1-ubyte"));


	// 2-3MB mem leak - very wrong
	for(size_t epoch = 0; epoch<_max_epoch; epoch++)
	{
		
		
		// split nn pool by number of MAX threads
		for (size_t thread_id = 0; thread_id < number_threads; thread_id++)
		{
			size_t start = thread_id * chunck_size;
			size_t end = (thread_id==number_threads-1) ? nn_list.size() : (thread_id+1) * chunck_size;

			// create a thread to process the chunk
			threads.emplace_back([=, &nn_list, &test_image] {
				for (size_t j = start; j < end; j++)
				{

					chunk_process(nn_list[j], test_image);
				}
				
				});
		}


		for (auto& t : threads)
		{
			t.join();
		}


		/// sorting pool by top highest score
		std::vector<std::pair<double, size_t>> score_list;
		for (size_t idx = 0; idx < nn_list.size(); idx++)
		{
			score_list.push_back(std::make_pair(nn_list[idx].get_fitness(), idx));
		};
		std::sort(score_list.begin(), score_list.end(), compare_by_score);



		// generate new pool for offspring
		std::vector<NeuralNetwork> nn_list_2(_pool_size * _pool_size_mult);

		// crossover and mutation
		size_t ln2 = 0;
		// for (size_t idx = 0; idx < _pool_size; idx++)
		for (size_t idx = 0; idx < _pool_size; idx+= _pool_size_mult)
		{

			for (size_t idy = 0; idy < _pool_size_mult; idy++)
			{
					nn_list_2[idx * _pool_size_mult + idy] = get_offsprings(nn_list[score_list[idx].second], nn_list[score_list[idx + idy].second], mutation_rate * (idy * _pool_size_mult / 2));

				//if (ln2 < nn_list_2.size())
				//{
				//	NeuralNetwork offspring = get_offsprings(nn_list[score_list[idx].second], nn_list[score_list[idx + idy].second], mutation_rate * (idy * _pool_size_mult / 2));
				//	nn_list_2[ln2] = offspring;
				//	ln2 ++;
				//};
				
				//if(ln2+1<nn_list_2.size())
				//nn_list_2[ln2+1] = offspring[1];

			}

		}
		
		for (size_t paire=0; paire < 10 ;paire++)
			printf(" NN_FILE:: epoch: %3d, id: %3d, answares: %1f\n", epoch, score_list[paire].second, nn_list[score_list[paire].second].get_fitness());
		printf("\n");

		nn_list = nn_list_2;
		//nn_list.shrink_to_fit();
		threads.clear();
		threads.shrink_to_fit();
	}


}


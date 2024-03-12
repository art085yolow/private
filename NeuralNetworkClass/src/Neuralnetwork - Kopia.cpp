#include "../include/Neuralnetwork.h"



double random_double()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(-1.0f, 1.0f);
	return dis(gen); // random number beetwen -1 and 1.
}

Matrix matrix_allocation(size_t _rows, size_t _cols)
{
	return Matrix{
			.row = _rows,
			.col = _cols,
			.stride = _cols,
			.data = new float[(_rows * _cols)]()
	};
}

void matrix_deallocation(Matrix _m)
{
	delete[] _m.data;
}

void matrix_randomize(Matrix _source)
{
	size_t m_size = _source.row * _source.col;
	for (size_t i = 0; i < m_size; i++)
	{
		_source.data[i] = random_double();
	}
}

Matrix matrix_row(Matrix _source, size_t _row_to_return)
{
	return Matrix{
			.row = 1,
			.col = _source.col,
			.stride = _source.stride,
			.data = &MATRIX_AT(_source, _row_to_return, 0)
	};
}

void matrix_fill(Matrix _source, double _value)
{
	// can't go just 'row*col' for matrix because it can be part of diff type/shape of matrix // *data could be shared
	for (size_t i = 0; i < _source.row; i++)
	{
		for (size_t j = 0; j < _source.col; j++)
		{
			MATRIX_AT(_source, i, j) = _value;
		}
	}
}

void matrix_copy(Matrix _dst, Matrix _src)
{
	if (_dst.row != _src.row || _dst.col != _src.col)
	{
		fprintf(stderr, "MATRIX_COPY::WRONG MATRIX FORMAT\N");
		exit(1);
	}

	for (size_t i = 0; i < _dst.row; i++)
	{
		for (size_t j = 0; j < _dst.col; j++)
		{
			MATRIX_AT(_dst, i, j) = MATRIX_AT(_src, i, j);
		}
	}
}

void matrix_print(Matrix _m, std::string _name, size_t _pad)
{
	printf("%*s%s = [\n", (int)_pad, "", _name.c_str());
	for (size_t i = 0; i < _m.row; i++)
	{
		printf("%*s", (int)_pad, "");
		for (size_t j = 0; j < _m.col; j++)
		{
			printf(" %9lf", MATRIX_AT(_m, i, j));
		}
		printf("\n");
	}
	
	printf("%*s]\n", (int)_pad, "");
}

void matrix_dot(Matrix _dst, Matrix _a, Matrix _b)
{
	if ((_a.col != _b.row || _b.col != _dst.col || _a.row != _dst.row)) {
		fprintf(stderr, "MATRIX_DOT::MATRIX WRONG FORMAT\n");
		exit(1);
	}
	size_t n = _a.col;

	for (size_t i = 0; i < _dst.row; i++)
	{
		for (size_t j = 0; j < _dst.col; j++)
		{
			MATRIX_AT(_dst, i, j) = 0;

			for (size_t k = 0; k < n; k++)
			{
				MATRIX_AT(_dst, i, j) += MATRIX_AT(_a, i, k) * MATRIX_AT(_b, k, j);
			}
		}
	}
}

void matrix_sum(Matrix _dst, Matrix _a)
{
	if (_dst.row != _a.row || _dst.col != _a.col )
	{
		fprintf(stderr, "MATRIX_SUM::WRONG MATRIX FORMAT\N");
		exit(3);
	}

	for (size_t i = 0; i < _dst.row; i++)
	{
		for (size_t j = 0; j < _dst.col; j++)
		{
			MATRIX_AT(_dst, i, j) += MATRIX_AT(_a, i, j) ;
		}
	}
}

void matrix_funct_activ(Matrix _m)
{
	for (size_t i = 0; i < _m.row; i++)
	{
		for (size_t j = 0; j < _m.col; j++)
		{
			MATRIX_AT(_m, i, j) = sigmoid(MATRIX_AT(_m, i, j));
		}
	}
}

double sigmoid(double _val)
{
	return 1.0f / (1.0f + exp(-_val));
}

// NN_matrix
NN_matrix nn_allocation(size_t _num_of_layers, size_t* _arch)
{
	if (_num_of_layers < 1) {
		fprintf(stderr, "NN_ALLOC::WRONG NN MATRIX ARCHITECTURE\n");
		exit(1);
	}

	NN_matrix nn;

	nn.layers = _num_of_layers - 1;

	nn.ws = new Matrix[nn.layers];
	nn.bs = new Matrix[nn.layers];
	nn.as = new Matrix[nn.layers + 1];

	if ((nn.ws == NULL || nn.bs == NULL || nn.as == NULL)) {
		fprintf(stderr, "NN_ALLOC::FAILED TO ALLOC MATRIX's\n");
		exit(1);
	}

	nn.as[0] = matrix_allocation(1, _arch[0]);

	for (size_t i = 1; i < _num_of_layers; i++)
	{
		nn.ws[i - 1] = matrix_allocation(nn.as[i - 1].col, _arch[i]);
		nn.bs[i - 1] = matrix_allocation(1, _arch[i]);
		nn.as[i] = matrix_allocation(1, _arch[i]);
	}


	return nn;
}

void nn_deallocation(NN_matrix _nn)
{
	// deallocation Matrix data
	for (size_t i = 0; i < _nn.layers; i++)
	{
		matrix_deallocation(_nn.as[i]);
		matrix_deallocation(_nn.bs[i]);
		matrix_deallocation(_nn.ws[i]);
	}
	// and last layer(output)
	matrix_deallocation(_nn.as[_nn.layers]);

	// deallocation Matrix array in NN
	delete[] _nn.as;
	delete[] _nn.bs;
	delete[] _nn.ws;

}

void nn_randomize(NN_matrix _nn)
{
	for (size_t i = 0; i < _nn.layers; i++)
	{
		matrix_randomize(_nn.bs[i]);
		matrix_randomize(_nn.ws[i]);
	}
}

void nn_print(NN_matrix _nn, std::string _name)
{
	printf("%s = {\n", _name.c_str());
	for (size_t i = 0; i < _nn.layers; i++)
	{
		matrix_print(_nn.ws[i], "ws" + std::to_string(i), 4);
		matrix_print(_nn.bs[i], "bs" + std::to_string(i), 4);
		if (i < _nn.layers - 1) printf("    = = = = = = = = = = = = =\n");
	}
	printf("}\n");
}

void nn_forward(NN_matrix _nn)
{
	for (size_t i = 0; i < _nn.layers; i++)
	{
		matrix_dot(_nn.as[i+1], _nn.as[i], _nn.ws[i]);
		matrix_sum(_nn.as[i+1], _nn.bs[i]);
		matrix_funct_activ(_nn.as[i+1]);
	}
}

void nn_backprop(NN_matrix _nn, NN_matrix _grad, double _rate, Matrix _train_data, Matrix _train_data_out, size_t _batch_size)
{
	if (_train_data.row != _train_data_out.row ||
		NN_OUTPUT(_nn).col != _train_data_out.col) {
		fprintf(stderr, "NN_BACKPROP::NN or MATRIX data WRONG FORMAT\n");
		exit(1);
	}

	size_t numbers_data = _train_data.row;
	if (_batch_size > numbers_data) _batch_size = numbers_data;
	// printf("where is fucking problem 1\n");

	for (size_t curr_sample = 0; curr_sample < numbers_data; )
	{
		nn_clear_grad(_grad);

		for (size_t bath = 0; bath < _batch_size && curr_sample < numbers_data; bath++, curr_sample++)
		{

			matrix_copy(NN_INPUT(_nn), matrix_row(_train_data, curr_sample));
			nn_forward(_nn);
			// now check if NN_OUTPUT(_nn) == matrix_row(_train_data_out, curr_sample);

			// reset as to 0
			for (size_t j = 0; j < _nn.layers; j++)
			{
				matrix_fill(_grad.as[j], 0);
			}
			matrix_fill(_grad.as[_nn.layers], 0);

			for (size_t j = 0; j < _train_data_out.col; j++)
			{
				MATRIX_AT(NN_OUTPUT(_grad), 0, j) = -(MATRIX_AT(_train_data_out, curr_sample, j) - MATRIX_AT(NN_OUTPUT(_nn), 0, j));
			}

			for (size_t l = _nn.layers; l > 0; l--)
			{
				// l - layer of the network
				for (size_t j = 0; j < _nn.as[l].col; j++)
				{
					double cost = 2 * MATRIX_AT(_grad.as[l], 0, j) * MATRIX_AT(_nn.as[l], 0, j) * (1 - MATRIX_AT(_nn.as[l], 0, j));
					MATRIX_AT(_grad.bs[l - 1], 0, j) += cost;
					for (size_t k = 0; k < _nn.as[l - 1].col; k++)
					{
						// j - weight matrix cols
						// k - weight matrix rows
						MATRIX_AT(_grad.ws[l - 1], k, j) += cost * MATRIX_AT(_nn.as[l - 1], 0, k);
						MATRIX_AT(_grad.as[l - 1], 0, k) += cost * MATRIX_AT(_nn.ws[l - 1], k, j);
					}
				}
			}

		}

		// printf("where is fucking problem 2\n");

		for (size_t i = 0; i < _grad.layers; i++)
		{
			for (size_t j = 0; j < _grad.ws[i].row; j++)
			{
				for (size_t k = 0; k < _grad.ws[i].col; k++)
				{
					MATRIX_AT(_grad.ws[i], j, k) /= _batch_size;
				}
			}

			for (size_t j = 0; j < _grad.bs[i].row; j++)
			{
				for (size_t k = 0; k < _grad.bs[i].col; k++)
				{
					MATRIX_AT(_grad.bs[i], j, k) /= _batch_size;
				}
			}
		}

		nn_apply_grad(_nn, _grad, _rate);

	}
}

void nn_apply_grad(NN_matrix _dst, NN_matrix _src, double _rate)
{

	for (size_t i = 0; i < _dst.layers; i++)
	{
		for (size_t j_n = 0; j_n < _dst.ws[i].row; j_n++)
		{
			for (size_t i_n = 0; i_n < _dst.ws[i].col; i_n++)
			{
				MATRIX_AT(_dst.ws[i], j_n, i_n) -= MATRIX_AT(_src.ws[i], j_n, i_n) * _rate;
			}
		}

		for (size_t j_n = 0; j_n < _dst.bs[i].row; j_n++)
		{
			for (size_t i_n = 0; i_n < _dst.bs[i].col; i_n++)
			{
				MATRIX_AT(_dst.bs[i], j_n, i_n) -= MATRIX_AT(_src.bs[i], j_n, i_n) * _rate;
			}
		}
	}
}

void nn_clear_grad(NN_matrix _grad)
{
	for (size_t i = 0; i < _grad.layers; i++)
	{
		matrix_fill(_grad.as[i], 0);
		matrix_fill(_grad.bs[i], 0);
		matrix_fill(_grad.ws[i], 0);
	}
	// and last layer(output)
	matrix_fill(_grad.as[_grad.layers], 0);
}

double nn_cost(NN_matrix _nn, Matrix _train_in, Matrix _train_out)
{
	if (_train_in.row != _train_out.row || NN_OUTPUT(_nn).col != _train_out.col )
	{
		fprintf(stderr, "NN_COST:: MATRIX NOT MATCH\n");
		exit(5);
	}

	size_t n = _train_in.row;

	double c = 0.0;

	for (size_t i = 0; i < n; i++)
	{
		matrix_copy(NN_INPUT(_nn), matrix_row(_train_in, i));
		nn_forward(_nn);
		for (size_t j = 0; j < _train_out.col; j++)
		{
			double delta = MATRIX_AT(NN_OUTPUT(_nn), 0, j) - MATRIX_AT(matrix_row(_train_out, i), 0, j);
			c += delta * delta;
		}
	}

	return c/n;
}



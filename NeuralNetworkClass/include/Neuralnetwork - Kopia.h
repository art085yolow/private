#pragma once
#include <iostream>
#include <string>
#include <regex>

#include <ostream>
#include <fstream>
#include <sstream>

#include <random>
#include <cstdlib>


// return value from Matrix at index 'i' of 'row' position and 'j' of 'col' position in that row
#define MATRIX_AT(m,i,j) (m).data[(i)*(m).stride +(j)]
// default matrix/nn print
#define MATRIX_PRINT(m)(matrix_print((m), #m, 0))
#define NN_PRINT(m)(nn_print((m), #m))
// small macro for nn alloc, give architecture array
#define NN_ALLOC(arch) (nn_allocation((sizeof(arch)/sizeof((arch)[0])),(arch)))
// nn randomize net
#define NN_RAND(nn) nn_randomize((nn))
// NN INPUT/OUTPUT
#define NN_INPUT(nn) ((nn).as[0])
#define NN_OUTPUT(nn) ((nn).as[(nn).layers])

struct Matrix
{
	// 1-dim array read as 2-dim
	size_t row = 0;
	size_t col = 0;
	size_t stride = 0;
	float* data = nullptr; 
};


struct NN_matrix
{
	//	REMEMBER FOR 3 LAYER NET (0-2) SUBSTRUCT 1. IT IT FOR MATH. THE LAST LAYER IS OUTPUT SO DON'T NEED 'WS' AND 'BS' TO CARRY FOR ANOTHER LAYER. BUT THAT WAY LAST LAYER IS CALLED AS[LAYERS].
	size_t layers = 0;
	Matrix* as = nullptr;
	Matrix* ws = nullptr;
	Matrix* bs = nullptr;
};

// randomize function
double random_double();

// Matrix functions

// allocated mem for matrix and return.
Matrix matrix_allocation(size_t _rows, size_t _cols);
void matrix_deallocation(Matrix _m);
// randomize all value in matrix
void matrix_randomize(Matrix _source);
// retrun single row matrix from source matrix
Matrix matrix_row(Matrix _source, size_t _row_to_return);
// fill all matrix value with given _value
void matrix_fill(Matrix _source, double _value);
// copy matrix from source to destination
void matrix_copy(Matrix _dst, Matrix _src);
// print matrix to console
void matrix_print(Matrix _m, std::string _name = "", size_t _pad = 0);
// dot product, matrix multiplication a with b to destination
void matrix_dot(Matrix _dst, Matrix _a, Matrix _b);
// summarize, matrix a added to destination
void matrix_sum(Matrix _dst, Matrix _a);
// matrix function activation - but find diff method to use diff type activation
void matrix_funct_activ(Matrix _m);

double sigmoid(double _val);

// NN_matrix functions and allocation
NN_matrix nn_allocation(size_t _num_of_layers, size_t* _arch);
void nn_deallocation(NN_matrix _nn);
void nn_randomize(NN_matrix _nn);
void nn_print(NN_matrix _nn, std::string _name = "nn");
void nn_forward(NN_matrix _nn);
void nn_backprop(NN_matrix _nn, NN_matrix _grad, double _rate, Matrix _train_data, Matrix _train_data_out, size_t _batch_size = 1);
void nn_apply_grad(NN_matrix _dst, NN_matrix _src, double _rate);
void nn_clear_grad(NN_matrix _grad);
double nn_cost(NN_matrix _nn, Matrix _train_in, Matrix _train_out);



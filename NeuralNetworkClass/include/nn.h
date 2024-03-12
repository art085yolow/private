// modif file nn.h. let's go c++

#ifndef NN_H_
#define NN_H_

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string>

// #define NN_BACKPROP_TRADITIONAL

#ifndef NN_ACT
#define NN_ACT ACT_SIG
#endif // NN_ACT

#ifndef NN_RELU_PARAM
#define NN_RELU_PARAM 0.01f
#endif // NN_RELU_PARAM

#ifndef NN_MALLOC
#include <stdlib.h>
#define NN_MALLOC malloc
#endif // NN_MALLOC

#ifndef NN_ASSERT
#include <assert.h>
#define NN_ASSERT assert
#endif // NN_ASSERT
#include <vector>

#define ARRAY_LEN(xs) sizeof((xs))/sizeof((xs)[0])

#define region_reset(r) (NN_ASSERT((r) != NULL), (r)->size = 0)
#define region_occupied_bytes(r) (NN_ASSERT((r) != NULL), (r)->size*sizeof(*(r)->words))
#define region_save(r) (NN_ASSERT((r) != NULL), (r)->size)
#define region_rewind(r, s) (NN_ASSERT((r) != NULL), (r)->size = s)

#define row_alloc(r, cols) mat_row(mat_alloc(r, 1, cols), 0)
#define row_rand(row, low, high) mat_rand(row_as_mat(row), low, high)
#define row_fill(row, x) mat_fill(row_as_mat(row), x);
#define row_print(row, name, padding) mat_print(row_as_mat(row), name, padding)
#define row_copy(dst, src) mat_copy(row_as_mat(dst), row_as_mat(src))
#define ROW_AT(row, col) (row).elements[col]

#define MAT_AT(m, i, j) (m).elements[(i)*(m).cols + (j)]
#define MAT_PRINT(m) mat_print(m, #m, 0)

#define NN_INPUT(nn) (NN_ASSERT((nn).arch_count > 0), (nn).as[0])
#define NN_OUTPUT(nn) (NN_ASSERT((nn).arch_count > 0), (nn).as[(nn).arch_count-1])
#define NN_PRINT(nn) nn_print(nn, #nn);


enum Act {
	ACT_SIG,
	ACT_RELU,
	ACT_TANH,
	ACT_SIN,
};


struct Region {
	size_t capacity;
	size_t size;
	uintptr_t *words;
};

struct Mat {
	size_t rows;
	size_t cols;
	float *elements;
};

struct Row {
	size_t cols;
	Act act = NN_ACT;
	float *elements;
};

struct NN{
	size_t *arch;
	size_t arch_count;
	Mat *ws; // The amount of activations is arch_count-1
	Row *bs; // The amount of activations is arch_count-1
	Row *as;
};

struct Batch {
	size_t begin;
	float cost;
	bool finished;
};

float rand_float(void);

double sigmoidf(double x);
double reluf(double x);
double tanhf(double x);

// Dispatch to the corresponding activation function
double actf(double x, Act act);

// Derivative of the activation function based on its value
double dactf(double y, Act act);

// capacity is in bytes, but it can allocate more just to keep things
// word aligned
Region region_alloc_alloc(size_t capacity_bytes);
void *region_alloc(Region *r, size_t size_bytes);

Mat row_as_mat(Row row);
Mat mat_alloc(Region *r, size_t rows, size_t cols);
void mat_fill(Mat m, float x);
void mat_rand(Mat m, float low, float high);
void mat_copy(Mat dst, Mat src);
void mat_dot(Mat dst, Mat a, Mat b);
void mat_sum(Mat dst, Mat a);
void mat_act(Mat m);
void mat_print(Mat m, const char *name, size_t padding);
void mat_shuffle_rows(Mat m);

Row row_slice(Row row, size_t i, size_t cols);
void row_act(Row r);
Row mat_row(Mat m, size_t row);

NN nn_alloc(Region *r, size_t *arch, size_t arch_count);
NN nn_backprop(Region *r, NN nn, Mat t);
void nn_zero(NN nn);
void nn_print(NN nn, const char *name);
void nn_rand(NN nn, float low, float high);
void nn_forward(NN nn);
float nn_cost(NN nn, Mat t);
void nn_learn(NN nn, NN g, double rate);

void batch_process(Region *r, Batch *b, size_t batch_size, NN nn, Mat t, double rate);

void nn_deallocation(NN _nn);
void matrix_deallocation(Mat _m);
void row_deallocation(Row _row);


struct Matrix {
public:
	Matrix();
	Matrix(Mat _mat);

	Mat get_mat();
private:
	std::vector<float> m_vec;
	size_t m_row=0;
	size_t m_col=0;
};

#endif // NN_H_

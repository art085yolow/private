#include "../include/Read_file.h"


Mat load_file_to_matrix_image(std::string _name)
{
	std::ifstream f;
	f.open(_name, std::ios::in | std::ios::binary);

	int fileSize = 0;

	if (!f.is_open()) {
		std::cout << "file could not be opened" << std::endl;
		exit(-7);
	}

	const int fileHeaderSizeI = 4 * sizeof(int);
	unsigned char fileHeaderI[fileHeaderSizeI];
	f.read(reinterpret_cast<char*>(fileHeaderI), fileHeaderSizeI);

	size_t m_magic_number = fileHeaderI[3] + (fileHeaderI[2] << 8) + (fileHeaderI[1] << 16) + (fileHeaderI[0] << 24);
	size_t m_number_Im_Lab = fileHeaderI[7] + (fileHeaderI[6] << 8) + (fileHeaderI[5] << 16) + (fileHeaderI[4] << 24);
	size_t m_width = fileHeaderI[11] + (fileHeaderI[10] << 8) + (fileHeaderI[9] << 16) + (fileHeaderI[8] << 24);
	size_t m_height = fileHeaderI[15] + (fileHeaderI[14] << 8) + (fileHeaderI[13] << 16) + (fileHeaderI[12] << 24);

	fileSize = m_number_Im_Lab * m_width * m_height;

	char* char_data = new char[fileSize];

	Mat ret{
		.rows = m_number_Im_Lab,
		.cols = m_width * m_height,
		.elements = new float[fileSize]
	};

	f.read(reinterpret_cast<char*>(char_data), fileSize);
	for (size_t i = 0; i < fileSize; i++)
	{
		ret.elements[i] = (unsigned char)char_data[i] / 255.0f;
	}

	delete[] char_data;
	f.close();

	return ret;
}

Mat load_file_to_matrix_label(std::string _name)
{
	std::ifstream f;
	f.open(_name, std::ios::in | std::ios::binary);

	int fileSize = 0;

	if (!f.is_open()) {
		std::cout << "file could not be opened" << std::endl;
		exit(-7);
	}

	const int fileHeaderSizeI = 2 * sizeof(int);
	unsigned char fileHeaderI[fileHeaderSizeI];
	f.read(reinterpret_cast<char*>(fileHeaderI), fileHeaderSizeI);

	size_t m_magic_number = fileHeaderI[3] + (fileHeaderI[2] << 8) + (fileHeaderI[1] << 16) + (fileHeaderI[0] << 24);
	size_t m_number_Im_Lab = fileHeaderI[7] + (fileHeaderI[6] << 8) + (fileHeaderI[5] << 16) + (fileHeaderI[4] << 24);

	fileSize = m_number_Im_Lab;

	char* char_data = new char[fileSize];

	Mat ret{
		.rows = m_number_Im_Lab,
		.cols = 10,
		.elements = new float[fileSize * 10]
	};

	f.read(reinterpret_cast<char*>(char_data), fileSize);
	for (size_t i = 0; i < fileSize; i++)
	{
		for (size_t lab = 0; lab < 10; lab++)
		{
			if((int)char_data[i]==lab)
			ret.elements[i * 10 + lab] = 1.0f;
			else
			ret.elements[i * 10 + lab] = 0.000001f;
		}
	}

	delete[] char_data;
	f.close();

	return ret;
}

Mat load_file_to_matrix(std::string _name_file_in, std::string _name_file_out)
{
	Mat in = load_file_to_matrix_image(_name_file_in);
	Mat out = load_file_to_matrix_label(_name_file_out);


	// copy[not move] IN and OUT to RET and delete elements from IN and OUT.
	// put IN and OUT rows in one line.

	if (in.rows != out.rows) {
		fprintf(stderr, "Number of samples not mach in FILES\n");
		exit(-5);
	}

	Mat ret{
		.rows = in.rows ,
		.cols = in.cols + out.cols,
		.elements = new float[ret.rows * ret.cols]
	};

	for (size_t r = 0; r < in.rows; r++)
	{
		for (size_t c = 0; c < in.cols; c++)
		{
			MAT_AT(ret, r, c) = MAT_AT(in, r, c);
		}

		for (size_t c = 0; c < out.cols; c++)
		{
			MAT_AT(ret, r, in.cols + c) = MAT_AT(out, r, c);
		}
	}

	delete[] in.elements;
	delete[] out.elements;
	return ret;
}

void save_data_network(NN _nn, std::string _path)
{
	size_t count_layer = _nn.arch_count;

	size_t* arch = _nn.arch;

	//

	std::ofstream write_file(_path, std::ios::binary);

	if (!write_file.is_open())
	{
		write_file.close();
		printf("Saving failed...\n");
		return;
	}

	// saving nn arch 
	write_file.write((char*)&count_layer, sizeof(count_layer));
	for (size_t i = 0; i < count_layer; i++)
	{
		write_file.write((char*)&arch[i], sizeof(size_t));

	}

	// saving weights and biases
	for (size_t l = 0; l < _nn.arch_count - 1; l++)
	{
		write_file.write((char*)_nn.ws[l].elements, (sizeof(float) * _nn.ws[l].cols * _nn.ws[l].rows));
		write_file.write((char*)_nn.bs[l].elements, (sizeof(float) * _nn.bs[l].cols));
	}

	for (size_t l = 1; l < _nn.arch_count; l++)
	{
		write_file.write((char*)&_nn.as[l].act, (sizeof(_nn.as[l].act)));
	};

	write_file.close();

	printf("Saved...\n");

}

NN load_data_network(std::string _path)
{

	std::ifstream open_file(_path, std::ios::in | std::ios::binary);

	if (!open_file.is_open())
	{
		open_file.close();
		printf("Failed to load file...\n");
		return NN();
	}

	size_t layers_count;

	open_file.read((char*)&layers_count, sizeof(layers_count));
	size_t* arch_net = new size_t[layers_count]();

	for (size_t i = 0; i < layers_count; i++)
	{
		open_file.read((char*)&arch_net[i], sizeof(size_t));
	}

	NN loaded_nn = nn_alloc(NULL, arch_net, layers_count);

	for (size_t l = 0; l < loaded_nn.arch_count - 1; l++)
	{
		float* data_w = new float[loaded_nn.ws[l].cols * loaded_nn.ws[l].rows]();
		float* data_b = new float[loaded_nn.bs[l].cols]();
		open_file.read((char*)data_w, sizeof(float) * loaded_nn.ws[l].cols * loaded_nn.ws[l].rows);
		open_file.read((char*)data_b, sizeof(float) * loaded_nn.bs[l].cols);

		for (size_t i = 0; i < loaded_nn.ws[l].cols * loaded_nn.ws[l].rows; i++)
		{
			loaded_nn.ws[l].elements[i] = data_w[i];
		}

		for (size_t i = 0; i < loaded_nn.bs[l].cols; i++)
		{
			loaded_nn.bs[l].elements[i] = data_b[i];
		}

	}


	for (size_t l = 1; l < loaded_nn.arch_count; l++)
	{
		Act act;
		open_file.read((char*)&act, sizeof(act));
		loaded_nn.as[l].act = act;
	}

	open_file.close();

	printf("Loaded...\n");

	return loaded_nn;
};


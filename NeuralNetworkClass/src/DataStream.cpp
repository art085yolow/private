#include "../include/DataStream.h"

Color::Color() : gray(0){}

Color::Color(unsigned char val)
{
	gray = val + (val << 8) + (val << 16);
}

Color::~Color(){}

Image::Image() : width(0), height(0){}

Image::Image(int width, int height, std::vector<unsigned char> val)
{
	this->width = width;
	this->height = height;
	this->m_char = val;
	
}

std::vector<unsigned int> Image::getColor()
{
	std::vector<unsigned int> colorTab;
	for (size_t i = 0, tt = this->m_char.size(); i<tt; i++)
	{
		unsigned int gray = m_char[i] + (m_char[i] << 8) + (m_char[i] << 16);
		colorTab.push_back(unsigned int(gray));
	}
	return colorTab;
}

Image::~Image() { }

DataStream::DataStream() : magic_number(0), number_Im_Lab(0), rows(0), columns(0){}

DataStream::DataStream(const char* pathImage, const char* pathLabels)
{
	std::ifstream f;
	f.open(pathImage, std::ios::in | std::ios::binary);

	int fileSize = 0;

	if (!f.is_open()) {
		std::cout << "file could not be opened" << std::endl;
		return;
	}

		const int fileHeaderSizeI = 4 * sizeof(int);
		unsigned char fileHeaderI[fileHeaderSizeI];
		f.read(reinterpret_cast<char*>(fileHeaderI), fileHeaderSizeI);

		magic_number = fileHeaderI[3] + (fileHeaderI[2] << 8) + (fileHeaderI[1] << 16) + (fileHeaderI[0] << 24);
		number_Im_Lab = fileHeaderI[7] + (fileHeaderI[6] << 8) + (fileHeaderI[5] << 16) + (fileHeaderI[4] << 24);
		rows = fileHeaderI[11] + (fileHeaderI[10] << 8) + (fileHeaderI[9] << 16) + (fileHeaderI[8] << 24);
		columns = fileHeaderI[15] + (fileHeaderI[14] << 8) + (fileHeaderI[13] << 16) + (fileHeaderI[12] << 24);
	
	
		fileSize = number_Im_Lab;
		
		for (size_t i = 0; i < fileSize; i++)
		{
			this->streamImageLabels.clear();

			for (size_t xy = 0, matrices = this->rows * this->columns; xy < matrices; xy++)
			{
				unsigned char byteFile[1];
				f.read(reinterpret_cast<char*>(byteFile), 1);
				this->streamImageLabels.push_back(unsigned char(*byteFile));
			}

			this->list_images.push_back(new Image(this->rows, this->columns, this->streamImageLabels));
		}
	
		this->streamImageLabels.clear();

	f.close();

	std::ifstream l;
	l.open(pathLabels, std::ios::in | std::ios::binary);

	fileSize = 0;

	if (!l.is_open()) {
		std::cout << "file could not be opened" << std::endl;
		return;
	}

	const int fileHeaderSize = 2 * sizeof(int);
	unsigned char fileHeader[fileHeaderSize];
	l.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

	magic_number = fileHeader[3] + (fileHeader[2] << 8) + (fileHeader[1] << 16) + (fileHeader[0] << 24);
	number_Im_Lab = fileHeader[7] + (fileHeader[6] << 8) + (fileHeader[5] << 16) + (fileHeader[4] << 24);


	fileSize = number_Im_Lab;

	for (size_t i = 0; i < fileSize; i++)
	{
		unsigned char byteFile[1];
		l.read(reinterpret_cast<char*>(byteFile), 1);
		this->streamImageLabels.push_back(unsigned char(*byteFile));
	}
		
	l.close();
}

DataStream::~DataStream() { this->list_images.clear(); }


std::vector<Image*> DataStream::getListOfImages()
{
	return std::vector<Image*>();
}

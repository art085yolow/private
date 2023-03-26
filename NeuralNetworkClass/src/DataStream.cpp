#include "../include/DataStream.h"


template<typename T>
inline std::string DataFile::to_string_with_precision(const T a_value, const int n)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}



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
	width = fileHeaderI[11] + (fileHeaderI[10] << 8) + (fileHeaderI[9] << 16) + (fileHeaderI[8] << 24);
	height = fileHeaderI[15] + (fileHeaderI[14] << 8) + (fileHeaderI[13] << 16) + (fileHeaderI[12] << 24);

	
	fileSize = number_Im_Lab;

	while (!f.eof())
	{
		Image buffImage;
		f.read(reinterpret_cast<char*>(&buffImage), sizeof(Image));
		this->list_images.push_back(buffImage);
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
	
	while (!l.eof())
	{
		unsigned char byteFile[1];
		l.read(reinterpret_cast<char*>(byteFile), 1);
		this->streamImageLabels.push_back(unsigned char(*byteFile));
	}
		
	l.close();
}

DataStream::~DataStream() { this->list_images.clear(); }


std::vector<Image>& DataStream::getListOfImages()
{
	return this->list_images;
}

std::vector<unsigned char>& DataStream::getListOfLabels()
{
	return this->streamImageLabels;
}


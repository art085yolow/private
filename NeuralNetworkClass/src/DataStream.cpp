#include "../include/DataStream.h"




DataStream::DataStream(const char* _pathImage, const char* _pathLabels)
{
	std::ifstream f;
	f.open(_pathImage, std::ios::in | std::ios::binary);

	int fileSize = 0;

	if (!f.is_open()) {
		std::cout << "file could not be opened" << std::endl;
		return;
	}

	const int fileHeaderSizeI = 4 * sizeof(int);
	unsigned char fileHeaderI[fileHeaderSizeI];
	f.read(reinterpret_cast<char*>(fileHeaderI), fileHeaderSizeI);

	m_magic_number = fileHeaderI[3] + (fileHeaderI[2] << 8) + (fileHeaderI[1] << 16) + (fileHeaderI[0] << 24);
	m_number_Im_Lab = fileHeaderI[7] + (fileHeaderI[6] << 8) + (fileHeaderI[5] << 16) + (fileHeaderI[4] << 24);
	m_width = fileHeaderI[11] + (fileHeaderI[10] << 8) + (fileHeaderI[9] << 16) + (fileHeaderI[8] << 24);
	m_height = fileHeaderI[15] + (fileHeaderI[14] << 8) + (fileHeaderI[13] << 16) + (fileHeaderI[12] << 24);

	
	fileSize = m_number_Im_Lab;

	while (!f.eof())
	{
		Image buffImage;
		f.read(reinterpret_cast<char*>(&buffImage.m_image_source), sizeof(buffImage.m_image_source));
		this->m_list_images.push_back(buffImage);
	}


	f.close();

	std::ifstream l;
	l.open(_pathLabels, std::ios::in | std::ios::binary);

	fileSize = 0;

	if (!l.is_open()) {
		std::cout << "file could not be opened" << std::endl;
		return;
	}

	const int fileHeaderSize = 2 * sizeof(int);
	unsigned char fileHeader[fileHeaderSize];
	l.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

	m_magic_number = fileHeader[3] + (fileHeader[2] << 8) + (fileHeader[1] << 16) + (fileHeader[0] << 24);
	m_number_Im_Lab = fileHeader[7] + (fileHeader[6] << 8) + (fileHeader[5] << 16) + (fileHeader[4] << 24);
	
	for (auto& iterImage : this->m_list_images)
	{
		if(!l.eof())
		l.read(reinterpret_cast<char*>(&iterImage.m_label), sizeof(iterImage.m_label));
	}
		
		
	l.close();
}

DataStream::~DataStream() { this->m_list_images.clear(); }


std::vector<Image>& DataStream::getListOfImages()
{
	return this->m_list_images;
}

size_t DataStream::getWidth()
{
	return this->m_width;
}

size_t DataStream::getHeight()
{
	return this->m_height;
}

void DataStream::shuffle_list()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	shuffle(this->m_list_images.begin(), this->m_list_images.end(),std::default_random_engine(seed));
}

size_t DataStream::sizeList()
{
	return this->m_list_images.size();
}

Image const DataStream::operator[](size_t _index)
{
	return this->m_list_images[_index];
}

size_t Image::size()
{
	return size_t(784);
}

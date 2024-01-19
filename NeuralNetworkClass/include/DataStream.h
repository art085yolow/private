#pragma once
#include <vector>
#include <fstream>
#include <ostream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <chrono>


struct Image
{
	// 28*28=784
	unsigned char m_image_source[784];
	unsigned char m_label;
	size_t size();
};

class DataStream
{
public:
	DataStream(const char* _pathImage, const char* _pathLabels);
	~DataStream();


	std::vector<Image>& getListOfImages();

	size_t getWidth();
	size_t getHeight();

	void shuffle_list();

	size_t sizeList();
	Image const operator[](size_t _index);

private:
	unsigned int m_magic_number;
	unsigned int m_number_Im_Lab; // Images or Labels
	unsigned int m_width, m_height;

	std::vector<Image> m_list_images;

};

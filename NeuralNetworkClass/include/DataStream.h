#pragma once
#include <vector>
#include <fstream>
#include <ostream>
#include <iostream>
#include <iomanip>
#include <algorithm>


struct Image
{
	// 28*28=784
	char imageSource[784];
	char label;
	size_t size();
};

class DataStream
{
public:
	DataStream(const char* pathImage, const char* pathLabels);
	~DataStream();


	std::vector<Image>& getListOfImages();

	size_t getWidth();
	size_t getHeight();

	void shuffle_list();

	size_t sizeList();
	Image const operator[](size_t index);

private:
	unsigned int magic_number;
	unsigned int number_Im_Lab; // Images or Labels
	unsigned int width, height;

	std::vector<Image> list_images;

};

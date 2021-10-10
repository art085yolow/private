#pragma once
#include <vector>
#include <fstream>
#include <ostream>
#include <iostream>
#include <iomanip>


struct Color
{
	unsigned int gray;

	Color();
	Color(unsigned char val);

	~Color();
}; 

struct Image
{
	int width;
	int height;
	std::vector<unsigned char> m_char;

	Image();
	Image(int width, int height, std::vector<unsigned char> val);
	
	std::vector<unsigned int> getColor();
	double getDoubleValue(unsigned int nrOnList);
	~Image();
};

class DataStream
{
public:
	DataStream();
	DataStream(const char* pathImage, const char* pathLabels);
	~DataStream();


	std::vector<Image*>& getListOfImages();
	std::vector<unsigned char>& getListOfLabels();

private:
	int magic_number;
	int number_Im_Lab; // Images or Labels
	int rows, columns;

	std::vector<unsigned char> streamImageLabels;
	std::vector<Image*> list_images;

};

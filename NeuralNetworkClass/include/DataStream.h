#pragma once
#include <vector>
#include <fstream>
#include <ostream>
#include <iostream>
#include <iomanip>


class DataFile
{		
public:

	DataFile() = default;

	inline static bool Read(DataFile& df, const std::string& sFileName);

	inline static bool Write(const DataFile& df, const std::string& sFileName);

private:

	template <typename T>
	std::string to_string_with_precision(const T a_value, const int n = 16);


};


struct Image
{
	// 28*28=784
	char imageSource[784];
};

class DataStream
{
public:
	DataStream(const char* pathImage, const char* pathLabels);
	~DataStream();


	std::vector<Image>& getListOfImages();
	std::vector<unsigned char>& getListOfLabels();

private:
	unsigned int magic_number;
	unsigned int number_Im_Lab; // Images or Labels
	unsigned int width, height;

	std::vector<unsigned char> streamImageLabels;
	std::vector<Image> list_images;

};

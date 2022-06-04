#include "BitDataMemo.h"

BitDataMemo::BitDataMemo(){}

BitDataMemo::~BitDataMemo(){}

void BitDataMemo::createSpaceMemory(unsigned int idxSize)
{
	// streamDataSize = idxSize * sizeof(double);
	this->indexSizeInMemory = idxSize;
	this->memoBit.resize(idxSize * sizeof(double), '\0');
}

void BitDataMemo::setValue(unsigned int idxPosition, double value)
{
	if (idxPosition < this->indexSizeInMemory)
	{
		unsigned char arrayFromDouble[8];
		memcpy(arrayFromDouble, &value, sizeof(double));

		for (size_t i = 0; i < 8; i++)
		{
			this->memoBit[(idxPosition * sizeof(double)) + i] = arrayFromDouble[i];
		}
	}
}

double BitDataMemo::getValue(unsigned int idxPosition)
{
	if (idxPosition < this->indexSizeInMemory)
	{
		unsigned char arrayToDouble[8];

		for (size_t i = 0; i < 8; i++)
		{
			arrayToDouble[i] = this->memoBit[(idxPosition * sizeof(double)) + i];
		}

		double ret = *(double*)arrayToDouble;

		return ret;
	}
	return 0.0;
}

bool BitDataMemo::setDataStream(std::string dataArray)
{
		this->memoBit = dataArray;
		if (this->memoBit == dataArray) return true;

		return false;
}

std::string BitDataMemo::getData()
{
		return this->memoBit;
}

unsigned int BitDataMemo::size()
{
	return this->indexSizeInMemory;
}

void BitDataMemo::reverse()
{
	BitDataMemo toRevers = *this;

	// go reverse

	for (size_t indexPos = 0; indexPos < this->indexSizeInMemory; indexPos++)
	{
		this->setValue((this->indexSizeInMemory - 1 - indexPos), toRevers.getValue(indexPos));
	}

	// revesing 

	this->reversedState = !this->reversedState;

}

bool BitDataMemo::reversed()
{
	return this->reversedState;
}


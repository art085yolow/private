#pragma once
#include <string>
class BitDataMemo {
public: 

	/// Zapisywaæ plki w formie: neurons bias(*.bn), weights(*.wg). Indentyfikacyjna nazwa pliku. 
	/// Plik g³ówny - head - wielkoœæ sieci i warstw.


	BitDataMemo();
	~BitDataMemo();

	// memory reservation
	void createSpaceMemory(unsigned int idxSize);

	// get/set value
	void setValue(unsigned int idxPosition, double value);
	double getValue(unsigned int idxPosition);

	// set data from save file
	bool setDataStream(std::string dataArray);

	// get data for save file
	std::string getData();

	// get size
	unsigned int size();

	// get revers indexed memo data - how and what you need for that?
	void reverse();

	bool reversed();

private:
	unsigned int indexSizeInMemory;
	std::string memoBit;
	bool reversedState = false;
};
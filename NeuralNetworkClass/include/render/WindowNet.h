#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include "../DataStream.h"
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



class WindowNet
{
public:
	WindowNet();
	WindowNet(const WindowNet&) = delete;
	WindowNet& operator =(const WindowNet&) = delete;
	~WindowNet();

	void windowSize(HWND hWnd);

	bool ProcessMsg();

	void render();

	void processInputs(float dt);
	
	void pushImage(std::vector<Image*> imageList);

	void renderImage(int xPos, int yPos, int sizeUp);

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	
	std::vector<Image*> _imageList;
	size_t valImage = 0;

	std::vector<unsigned int> imageStream;

	int width = 800;
	int height = 600;

};
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

	// render buffor -- TODO / re-organize
	void render();

	// key inputs - TODO
	void processInputs(float dt);
	

	void pushImage(std::vector<Image*> imageList); //delete after

	// for render image -- TODO / refactor
	void renderImage(float xPos, float yPos, float size, int width, int height, std::vector<unsigned int> image);


	std::vector<unsigned int> imageStream;
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	
	std::vector<Image*> _imageList;
	size_t valImage = 0;


	int width = 800;
	int height = 600;

	// keep background in middel value
	unsigned int background = 0xdddddd;

};
#pragma once
#include <Windows.h>
#include <iostream>
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
	
	void colorUp();

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	
	int width = 800;
	int height = 600;

};
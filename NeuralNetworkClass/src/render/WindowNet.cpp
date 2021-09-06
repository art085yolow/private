#include "../../include/render/WindowNet.h"
#include <iostream>

#include "../utils.cpp"


struct Render_State
{
	int width, height;
	void* memory;

	BITMAPINFO bitmapinfo;
};

GLOBAL_VARIABLE Render_State render_state;
HDC hdc;

#include "renderer.cpp"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_SIZE: {
		RECT rect;
		GetClientRect(hWnd, &rect);
		render_state.width = rect.right - rect.left;
		render_state.height = rect.bottom - rect.top;
		int buffer_size = render_state.width * render_state.height * sizeof(unsigned int);

		if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
		render_state.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	
		render_state.bitmapinfo.bmiHeader.biSize = sizeof(render_state.bitmapinfo.bmiHeader);
		render_state.bitmapinfo.bmiHeader.biWidth = render_state.width;
		render_state.bitmapinfo.bmiHeader.biHeight = render_state.height;
		render_state.bitmapinfo.bmiHeader.biPlanes = 1;
		render_state.bitmapinfo.bmiHeader.biBitCount = 32;
		render_state.bitmapinfo.bmiHeader.biCompression = BI_RGB;

	}	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	default:
		result = DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return result;
}

WindowNet::WindowNet()
	: m_hInstance(GetModuleHandle(nullptr))
{
	const wchar_t* CLASS_NAME = L"WindowClass";

	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_hInstance;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = WindowProc;
	
	RegisterClass(&wndClass);

	DWORD style = WS_CAPTION | WS_OVERLAPPEDWINDOW | WS_SYSMENU;

	RECT rect;
	rect.left = 250;
	rect.top = 250;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	AdjustWindowRect(&rect, style, false);

	m_hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"NeuralNetwork",
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);

	hdc = GetDC(m_hWnd);

	ShowWindow(m_hWnd, SW_SHOW);

}

WindowNet::~WindowNet()
{
	const wchar_t* CLASS_NAME = L"WindowClass";

	UnregisterClass(CLASS_NAME, m_hInstance);
}

void WindowNet::windowSize(HWND hWnd) // later delete
{
		RECT rect;
		GetClientRect(hWnd, &rect);
		render_state.width = rect.right - rect.left;
		render_state.height = rect.bottom - rect.top;
		int buffer_size = render_state.width * render_state.height * sizeof(unsigned int);

		if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
		render_state.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

bool WindowNet::ProcessMsg()
{
	MSG msg = {};
	while (PeekMessage(&msg,nullptr, 0u,0u,PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

void WindowNet::render()
{
	 clear_screen(0x990022);
	 //draw_rect_in_pixels(50, 50, 200, 500, 0x00ff22);
	 draw_rect(-10, -20, 10, 30, 0x00ff22);
	 draw_rect(10, 10, 5, 15, 0x114522);
	 draw_rect(0, 0, 20, 20, 0x223377);


	StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
}


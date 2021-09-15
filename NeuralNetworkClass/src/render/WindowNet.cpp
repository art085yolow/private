#include "../../include/render/WindowNet.h"


#include "../utils.cpp"
#include "../controls.cpp"

struct Render_State
{
	int width, height;
	void* memory;

	BITMAPINFO bitmapinfo;
};

GLOBAL_VARIABLE Render_State render_state;
HDC hdc;

Input input = {};

#include "renderer.cpp"
unsigned int background = 0xffffff;

bool renderOne = true;
float deltaTime = 0.0f;
LARGE_INTEGER frame_begin_time;
LARGE_INTEGER frame_end_time;

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

	QueryPerformanceCounter(&frame_begin_time);
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
		switch (msg.message)
		{case WM_QUIT:
			return false;
			break;
		case WM_KEYUP:
		case WM_KEYDOWN:
		{
			u32 vk_code = (u32)msg.wParam;
			bool is_down = ((msg.lParam & (1 << 31)) == 0);

			switch (vk_code)
			{
				process_button(BUTTON_UP, VK_UP);
				process_button(BUTTON_DOWN, VK_DOWN);
				process_button(BUTTON_LEFT, VK_LEFT);
				process_button(BUTTON_RIGHT, VK_RIGHT);
				process_button(BUTTON_ESC, VK_ESCAPE);
			}
		}
			break;
		default:
		TranslateMessage(&msg);
		DispatchMessage(&msg);
			break;
		}
	}

	return true;
}

void WindowNet::render()
{

	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	for (s32 i = 0; i < BUTTON_COUNT; i++)
	{
		input.buttons[i].changed = false;
	}

	processInputs(deltaTime);
	
	
	clear_screen(background);
	//draw_rect_in_pixels(50, 50, 200, 500, 0x00ff22);



	renderImage(10, 10, 10);
	
	// draw_rect(3, 3, 3, 3, 0x114522);
	// draw_rect(0, 0, 2, 2, 0x223377);


	StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
	QueryPerformanceCounter(&frame_end_time);

	deltaTime = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
	frame_begin_time = frame_end_time;


}



 void WindowNet::processInputs(float dt)
{
	 // if(is_down(BUTTON_DOWN) player_pos_y-=0.1*dt;

	 if (is_down(BUTTON_UP))
	 {
		 this->valImage++;
	 }

	 if (is_down(BUTTON_DOWN))
	 {
		 renderOne = true;
	 }
	if (is_down(BUTTON_ESC))
		PostQuitMessage(0);
}


 void WindowNet::pushImage(std::vector <Image*> imageList)
 {
	 _imageList = imageList;
 }

 void WindowNet::renderImage(int xPos, int yPos, int sizeUp)
 {
	 /// y=0, y1=10, x=0 , x1=10, one pixel sizeUp 10x;
	 /// send pixelSizeUp to render in position XY;
	 /// new y = y * sizeUp, new y1 = y*sizeUp+sizeUp, x(...) 
	 /// check if x or y are greater at window width/height. resize request. image.width/height * sizeUp < window.width/heigth
	 /// (...).render_in_pixel(xPos+x,xPos+x1,yPos+y,yPos+y1, _imageList[valImage]->getColor();
	 
	 int width = this->_imageList[valImage]->width;
	 int height = this->_imageList[valImage]->height;
	 int size = sizeUp;

	 int imageSizeX = width * size;
	 int imageSizeY =render_state.height;

	 this->imageStream = _imageList[valImage]->getColor();
	 /*
	 //check if sizeUp is not too large
	 if (!(width * size + xPos < render_state.width))
	 {
		 size = (render_state.width - xPos) / width;			/// 10*10+10<100  /// 100+10<100 /// 110<100 <!> /// (100(monit wisth) - 10(xyPos)) / 10(how many pixels to render) = size 9
	 }
		 
	if ( !(height * size + yPos < render_state.height))
	{
		size = (render_state.height - yPos) / height;
	}
	*/

	size_t count = 0;
	 for(size_t y=0;y<height;y++)
	 {
		 int y1New =imageSizeY - yPos - (y * size);
		 int yNew =y1New - size;
		for (size_t x = 0; x < width; x++)
		{
			int xNew = x*size + xPos;
			int x1New = xNew + size;

			unsigned int color = this->imageStream[count];
			draw_rect_in_pixels(xNew,yNew,x1New, y1New, color);
			count++;
		}

	 }

 }


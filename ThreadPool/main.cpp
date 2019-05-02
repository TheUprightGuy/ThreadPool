#include <iostream>
#include <vector>
#include <windows.h>
#include "ThreadPool.h"
#include "WorkQueue.h"
#include "Task.h"
#include "Mandelbrot.h"
int RunThreads(HWND _hwnd)
{

	RECT _clientRect;
	GetClientRect(_hwnd, &_clientRect);
	const int _iWindowWidth = _clientRect.right;
	const int _iWindowHeight = _clientRect.bottom;

	const int iXMax = 16;
	const int iYMax = 8;

	const int iXDivided = _iWindowWidth / iXMax;
	const int iYDivided = _iWindowHeight / iYMax;

	
	srand((unsigned int)time(0));
	int kiTOTALITEMS = iXMax*iYMax;
	//Create a ThreadPool Object capable of holding as many threads as the number of cores

	ThreadPool& threadPool = ThreadPool::GetInstance();
	//Initialize the pool
	threadPool.Initialize();

	threadPool.Start();

	// The main thread writes items to the WorkQueue
	for (int i = 0; i < iXMax; i++)
	{
		for (int j = 0; j < iYMax; j++)
		{
			threadPool.Submit(CTask(_hwnd, i, j));
		}
			std::this_thread::sleep_for(std::chrono::milliseconds(650));
	}

	//threadPool.Start();

	if (threadPool.getItemsProcessed() == kiTOTALITEMS)
	{
		threadPool.Stop();
	}

	threadPool.DestroyInstance();

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC          hdc = GetDC(hWnd);
	PAINTSTRUCT  ps;

	switch (message)
	{
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		
		EndPaint(hWnd, &ps);
		return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
} // WndProc

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	HWND hWnd;
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	WNDCLASS wndClass;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = TEXT("GettingStarted");

	RegisterClass(&wndClass);

	hWnd = CreateWindow(
		TEXT("GettingStarted"),   // window class name
		TEXT("Getting Started"),  // window caption
		WS_OVERLAPPEDWINDOW,      // window style
		CW_USEDEFAULT,            // initial x position
		CW_USEDEFAULT,            // initial y position
		CW_USEDEFAULT,            // initial x size
		CW_USEDEFAULT,            // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	RunThreads(hWnd);

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (static_cast<int>(msg.wParam));
}
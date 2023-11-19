#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Window.h"
#ifndef GWL_USERDATA
#define GWL_USERDATA GWLP_USERDATA
#endif

Window* window = nullptr;

Window::Window()
{

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
		if (window)
		{
			SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG_PTR)window);
			window->setHWND(hwnd);
			window->onCreate();
		}
		break;

	}
	case WM_DESTROY:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWL_USERDATA);
		if (window)
		{
			window->onDestroy();
			delete window;
		}
		::PostQuitMessage(0);
		break;
	}

	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return NULL;

}

bool Window::init()
{
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"llll";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	if (::RegisterClassEx(&wc) == 0)
		return false;

	if (!window)
		window = this;

	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"Direct X Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL,
	NULL, wc.hInstance, NULL);

	if (!m_hwnd)
		return false;

	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	m_is_run = true;
	return true;
}

bool Window::broadcast()
{
	MSG msg;
	this->onUpdate();
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Sleep(1);
	return true;
}

bool Window::release()
{
	if (m_hwnd)
		if (!::DestroyWindow(m_hwnd))
			return false;

	return true;
}

bool Window::isRun()
{
	return m_is_run;
}
RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}
void Window::setHWND(HWND hwnd)
{
	this->m_hwnd = hwnd;
}
void Window::onCreate()
{

}

void Window::onUpdate() 
{
	
}
void Window::onDestroy()
{
	m_is_run = false;
}

Window::~Window()
{

}

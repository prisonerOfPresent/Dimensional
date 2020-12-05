#pragma once

#include "DimensionalWin32Window.h"

// Window class stuff
DimensionalWindow::WindowClass DimensionalWindow::WindowClass::wndClass;

DimensionalWindow::WindowClass::WindowClass() noexcept : hInstance( GetModuleHandle(nullptr))
{
	// Create WinAPI window class
	WNDCLASSEX wc = { 0 };
	// Setup the window class
	// Setup total size of struct
	wc.cbSize = sizeof(wc);
	//set rendering style -
	// CS_OWNDC is a bit flag that tells windows we need own device contexts for all windows spawned from this app.
	wc.style = CS_OWNDC;
	// Set the window procedure that will handle all messages for this window ( events )
	// Set that to the our WndProc defined above.
	wc.lpfnWndProc = MessageHandlerSetup;
	// Set extra data bytes for the class
	wc.cbClsExtra = 0;
	// Set extra data bytes for window
	wc.cbWndExtra = 0;
	// Set the handle for the class as the handle for current application,
	// provided by Windows via HINSTANCE parameter.
	wc.hInstance = GetInstance();
	// Set icon, smallIcon and cursor as null
	wc.hIcon = static_cast<HICON>(LoadImage(hInstance,MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON,48,48,0));
	wc.hCursor = nullptr;
	wc.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));;
	// Set window background as null, so that we can draw using D3D
	// If we set background as null then windows wont draw a default background.
	wc.hbrBackground = nullptr;
	// Since we dont have any menu, setting menu name also as null.
	wc.lpszMenuName = nullptr;
	// Set the window title.
	wc.lpszClassName = GetName();
	// Register the class with WinAPI
	RegisterClassEx(&wc);
}

DimensionalWindow::WindowClass::~WindowClass() 
{
	UnregisterClass(wndClassName,GetInstance());
}

const char* DimensionalWindow::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE DimensionalWindow::WindowClass::GetInstance() noexcept 
{
	return wndClass.hInstance;
}

// End of window class stuff

DimensionalWindow::DimensionalWindow(int width, int height, const char* title)
{
	RECT windowRect;
	windowRect.left = 100;
	windowRect.right = width + windowRect.left;
	windowRect.top = 100;
	windowRect.bottom = height + windowRect.top;
	if (FAILED(AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE))) 
	{
		throw DIMWND_LAST_EXCEPT();
	}
	this->width = windowRect.right - windowRect.left;
	this->height = windowRect.bottom - windowRect.top;
	hWnd = CreateWindow( 
		WindowClass::GetName(), // Classname
		title, // Window name
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // window style
		CW_USEDEFAULT, // x
		CW_USEDEFAULT, // y
		this->width, // width
		this->height, // height
		nullptr, // parent
		nullptr,  // menu
		WindowClass::GetInstance(), //hInstance
		this //lParam - why passing this? Actually it is a cool mechanism to set the pointer to our class as custom user data
		// In the Win32 window. We can use this later to make our instance method handle our windows messages ( events ) .
	);
	if (hWnd == nullptr) 
	{
		throw DIMWND_LAST_EXCEPT();
	}
	// Show the window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

DimensionalWindow::~DimensionalWindow() 
{
	DestroyWindow(hWnd);
}

// Message handling stuff

LRESULT DimensionalWindow::MessageHandlerSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	// Why this method even exists?
	// Because we cannot directly make instance methods handle windows messages, we are hooking up two static methods,
	// These will intercept window creation message , more specifically, non client region create message, and inject another 
	// message handler stub like this to forward messages to our instance message handling method.
	if (msg == WM_NCCREATE) 
	{
		// We are extracting lParam data as a window creation struct.
		// This struct contains the user data set by us, actually a pointer to current window.
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		// Since we got the create params, we are going to cast that to our window class.
		DimensionalWindow* const pWindow = static_cast<DimensionalWindow*>(pCreate->lpCreateParams);
		// Now we are setting user data as pointer to our window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		// we are also setting the WndProc to be our forwarder method.
		SetWindowLongPtr( hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&DimensionalWindow::MessageForwarder) );
		return pWindow->HandleMessage(hWnd, msg, wParam, lParam);
	}
	// Just accept default behaviour.
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT DimensionalWindow::MessageForwarder(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// We are intercepting all messgaes, and passing it to our instance method.
	// First get the goddamnn pointer to our window from the user data of our win32 window.
	DimensionalWindow* const pWindow = reinterpret_cast<DimensionalWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// Now do what was promised - forward the actual messages.
	return pWindow->HandleMessage(hWnd, msg, wParam, lParam);
}

LRESULT DimensionalWindow::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Now handle the WM_CLOSE gracefully.
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (wParam == 'F')
			SetWindowText(hWnd, "You pressed F key.");
		break;
	case WM_KEYUP:
		if (wParam == 'F')
			SetWindowText(hWnd, "Dimensional");
		break;
	case WM_CHAR:
	{
		static std::string title("");
		title.push_back((char)wParam);
		SetWindowText(hWnd, title.c_str());
		break;
		// Mouse left button click.
		// lets set window text as mouse double click
	}
	case WM_LBUTTONDOWN:
		const POINTS point = MAKEPOINTS(lParam);
		std::ostringstream oss;
		oss << " Dimensional - Mouse clicked at ";
		oss << "(" << point.x << "," << point.y << ")";
		SetWindowText(hWnd, oss.str().c_str());
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Exception stuff

DimensionalWindow::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	: DimensionalException( line, file ),
	hr( hr)
{
}

const char* DimensionalWindow::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl;
	oss << "[Error code ]" << GetErrorCode() << std::endl;
	oss << "[Description]" << GetErrorString() << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* DimensionalWindow::Exception::GetType() const noexcept
{
	return "DimensionalWindow::Exception";
}

std::string DimensionalWindow::Exception::TranslateErrorCode(HRESULT hr)
{
	char* pMessageBuffer = nullptr;
	// I guess this is to format the message.
	DWORD nMessageLength = FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // formatting flags
		nullptr, // source
		hr, // messageID
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Language ID
		reinterpret_cast<LPSTR>(&pMessageBuffer), // buffer
		0, // size
		nullptr // arguments
		);
	if (nMessageLength == 0) 
	{
		return "Unknown error code.";
	}
	std::string errorString = pMessageBuffer;
	LocalFree( pMessageBuffer );
	return errorString;
}

HRESULT DimensionalWindow::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string DimensionalWindow::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}

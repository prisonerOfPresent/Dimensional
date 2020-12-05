#include "DimesnionalWin32.h"
#include <iostream>
#include <sstream>
#include "DimensionalWin32Window.h"



int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{

	try
	{
		// Create our sweet window.
		DimensionalWindow window(800, 300, "Dimensional");

		// Time to handle window messages.
		// Create the MSG struct to receive messages.

		MSG msg;

		// TODO : Lookup GetMessage function
		// needs pointer to message, handle to window - if null, we get all messages for the thread.
		// then min and max filters - todo : look that up.

		// Save the message value to a BOOL ( Windows type )
		// 0 - quit
		// -1 - error
		// > 0 - something else
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			// Now that we are in the loop, translate the messages that windows sends.
			// This takes a pointer to current message struct.
			TranslateMessage(&msg);
			// Once that is done, dispatch the message.
			DispatchMessage(&msg);
		}
		// If error, return -1
		if (gResult == -1)
			return -1;
		// else return wParam of message received, to figure out exit reason.
		else
			return msg.wParam;
	} catch ( const DimensionalException& ex )
	{
		MessageBox( nullptr, ex.what(), ex.GetType(), MB_OK | MB_ICONEXCLAMATION );
	}
	catch (const std::exception ex) 
	{
		MessageBox(nullptr, ex.what(), "std::exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) 
	{
		MessageBox(nullptr, "Unknown exception occurred ", "Unknwon exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}
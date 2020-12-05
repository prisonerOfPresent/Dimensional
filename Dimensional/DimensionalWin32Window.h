#pragma once
#include "DimesnionalWin32.h"
#include <iostream>
#include <sstream>
#include "DimensionalException.h"
#include "resource.h"

class DimensionalWindow 
{
public:
	class Exception : public DimensionalException 
	{
	public: 
		Exception( int line, const char* filr, HRESULT hr ) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode( HRESULT hr );
		HRESULT GetErrorCode()const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
private:
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass& ) = delete;
		static constexpr const char* wndClassName = "Dimensional Win32 Window";
		static WindowClass wndClass;
		HINSTANCE hInstance;
	};
public: 
	DimensionalWindow( int width, int height, const char* title );
	~DimensionalWindow();
	DimensionalWindow(const DimensionalWindow&) = delete;
	DimensionalWindow& operator=( const DimensionalWindow& ) = delete;
private:
	static LRESULT WINAPI MessageHandlerSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT WINAPI MessageForwarder(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

private:
	int width, height;
	HWND hWnd;
};
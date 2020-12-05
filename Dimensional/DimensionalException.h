#pragma once

#include <exception>
#include <string>
#include <sstream>


class DimensionalException : public std::exception 
{
public:
	DimensionalException( int line, const char* file  ) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;
private:
	int line;
	std::string file;
protected:
	mutable std::string whatBuffer;
};

#define DIMWND_EXCEPT( hr ) DimensionalWindow::Exception( __LINE__ , __FILE__, hr )
#define DIMWND_LAST_EXCEPT() DimensionalWindow::Exception( __LINE__ , __FILE__, GetLastError() )
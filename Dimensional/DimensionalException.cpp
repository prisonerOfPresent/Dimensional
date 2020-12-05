#include "DimensionalException.h"

DimensionalException::DimensionalException(int line, const char* file) noexcept
	: line(line),
	file(file)
{}

const char* DimensionalException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* DimensionalException::GetType() const noexcept
{
	return "DimensionalException";
}

int DimensionalException::GetLine() const noexcept
{
	return line;
}

const std::string& DimensionalException::GetFile() const noexcept
{
	return file;
}

std::string DimensionalException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File]" << file << std::endl;
	oss << "[Line]" << line << std::endl;
	return oss.str();
}

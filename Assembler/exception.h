#pragma once

#include <string>

enum class ExceptionType
{
	kUnknownSubroutine,
};

class Exception
{
public:
	inline Exception(
		const std::string& description,
		const ExceptionType& exception_type
	) : address_(0)
		, description_(description)
		, exception_type_(exception_type) {}

	inline Exception(
		const uint16_t&			address,
		const std::string&		description,
		const ExceptionType&	exception_type
	) : address_(address)
		, description_(description)
		, exception_type_(exception_type) {}

	inline const int16_t&		address() { return address_; }
	inline const std::string&	description() { return description_; }
	inline const ExceptionType& exception_type() { return exception_type_; }

private:
	uint16_t		address_;
	std::string		description_;
	ExceptionType	exception_type_;
};

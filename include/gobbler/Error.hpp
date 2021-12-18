#pragma once

#include <stdexcept>
#include <string>

#include "ArgParser.hpp"

namespace goblr {

struct Error : public std::exception {};

enum class OptType {
	short_opt,
	long_opt,
};

struct UnknownOptionError : public Error {
	OptType opt_type;
	std::string opt_name;
	explicit UnknownOptionError(ArgParser::long_opt_t const a_long_opt_name) : opt_type(OptType::long_opt), opt_name{ a_long_opt_name } {}
	explicit UnknownOptionError(ArgParser::short_opt_t const a_short_opt_name) : opt_type(OptType::short_opt), opt_name{ a_short_opt_name } {}
	[[nodiscard]] char const* what() const noexcept override {
		return "goblr::UnknownOptionError";
	}
};

struct ArgumentMismatchError : public Error {
	enum class Reason {
		provided_not_expected,
		expected_not_provided,
	};
	Reason what_happened;
	OptType opt_type;
	std::string opt_name;
	explicit ArgumentMismatchError(Reason a_what_happened, ArgParser::long_opt_t const a_long_opt_name)
		: what_happened(a_what_happened), opt_type(OptType::long_opt), opt_name{ a_long_opt_name } {}
	explicit ArgumentMismatchError(Reason a_what_happened, ArgParser::short_opt_t const a_short_opt_name)
		: what_happened(a_what_happened), opt_type(OptType::short_opt), opt_name{ a_short_opt_name } {}
	[[nodiscard]] char const* what() const noexcept override {
		return "Errors::ArgParse::ArgumentMismatch";
	}
};

}  // namespace goblr

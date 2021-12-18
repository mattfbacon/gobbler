#include <gobbler/ArgParser.hpp>
#include <vector>
#include <gobbler/Error.hpp>
#include <cassert>
#include <iostream>

using Result = goblr::ArgParser::OptionCallbackResult;
struct Callbacks : public goblr::ArgParser::Callbacks {
	// example arguments
	int verbosity = 0;
	bool force = false;
	std::string tool;
	std::vector<std::string> arguments;

	Result short_option_callback(char name) override;
	Result long_option_callback(std::string_view name) override;
	void short_option_argument_callback(char name, std::string_view arg) override;
	void long_option_argument_callback(std::string_view name, std::string_view arg) override;
	void argument_callback(std::string_view arg) override;
};

Result Callbacks::short_option_callback(char const name) {
	switch (name) {
		case 'v':
			++verbosity;
			return Result::no_argument;
		case 'f':
			force = true;
			return Result::no_argument;
		case 't':
			return Result::want_argument;
		default:
			throw goblr::UnknownOptionError{ name };
	}
}

void Callbacks::short_option_argument_callback(char const name, std::string_view const arg) {
	// this is the only one we returned want_argument for
	assert(name == 't');
	tool = arg;
}

Result Callbacks::long_option_callback(std::string_view const name) {
	if (name == "verbose") {
		return short_option_callback('v');
	} else if (name == "force") {
		return short_option_callback('f');
	} else if (name == "tool") {
		return short_option_callback('t');
	} else {
		// this can be constructed with string_view or char so it knows if the option was short or long
		throw goblr::UnknownOptionError{ name };
	}
}

void Callbacks::long_option_argument_callback(std::string_view const name, std::string_view const arg) {
	// ditto
	assert(name == "tool");
	short_option_argument_callback('t', arg);
}

void Callbacks::argument_callback(std::string_view const arg) {
	arguments.emplace_back(arg);
}

static void print_option(goblr::OptType opt_type, std::string_view opt_name) {
	switch (opt_type) {
		case goblr::OptType::short_opt:
			std::clog << "short option -";
			break;
		case goblr::OptType::long_opt:
			std::clog << "long option --";
			break;
		default:
			__builtin_unreachable();
	}
	std::clog << opt_name << std::endl;
}

int main(int argc, char* argv[]) try {
	Callbacks callbacks;
	goblr::ArgParser::parse(std::as_const(argc), std::as_const(argv), callbacks);
	std::cout << "Verbosity: " << callbacks.verbosity << '\n';
	std::cout << "Force? " << (callbacks.force ? "yes" : "no") << '\n';
	std::cout << "Tool: " << callbacks.tool << '\n';
	std::cout << "Arguments:\n";
	for (auto const& argument : callbacks.arguments) {
		std::cout << "  " << argument << '\n';
	}
	return 0;
} catch (goblr::UnknownOptionError const& e) {
	std::clog << "Unknown ";
	print_option(e.opt_type, e.opt_name);
} catch (goblr::ArgumentMismatchError const& e) {
	switch (e.what_happened) {
		using Reason = goblr::ArgumentMismatchError::Reason;
		case Reason::provided_not_expected:
			std::clog << "Argument provided to ";
			print_option(e.opt_type, e.opt_name);
			std::clog << " but not expected";
			break;
		case Reason::expected_not_provided:
			std::clog << "Expected argument not provided to ";
			print_option(e.opt_type, e.opt_name);
			break;
		default:
			__builtin_unreachable();
	}
}

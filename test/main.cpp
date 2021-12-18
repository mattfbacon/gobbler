#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <gobbler/ArgParser.hpp>
#include <variant>
#include <vector>

using Result = goblr::ArgParser::OptionCallbackResult;

struct ShortOption {
	char name;
	bool operator==(ShortOption const& other) const = default;
};
struct LongOption {
	std::string name;
	bool operator==(LongOption const& other) const = default;
};
struct ShortArgumentOption {
	char name;
	std::string arg;
	bool operator==(ShortArgumentOption const& other) const = default;
};
struct LongArgumentOption {
	std::string name, arg;
	bool operator==(LongArgumentOption const& other) const = default;
};
struct PositionalArgument {
	std::string arg;
	bool operator==(PositionalArgument const& other) const = default;
};

using ArgumentList = std::vector<std::variant<ShortOption, LongOption, ShortArgumentOption, LongArgumentOption, PositionalArgument>>;

struct TestCallbacks : public goblr::ArgParser::Callbacks {
	ArgumentList received;
	Result short_option_callback(char name) override;
	void short_option_argument_callback(char name, std::string_view arg) override;
	Result long_option_callback(std::string_view name) override;
	void long_option_argument_callback(std::string_view name, std::string_view arg) override;
	void argument_callback(std::string_view arg);
};

Result TestCallbacks::short_option_callback(char const name) {
	if ((name >= 'a' && name <= 'm') || (name >= 'A' && name <= 'M')) {
		received.emplace_back(ShortOption{ name });
		return Result::no_argument;
	} else {
		return Result::want_argument;
	}
}
Result TestCallbacks::long_option_callback(std::string_view const name) {
	if ((name[0] >= 'a' && name[0] <= 'm') || (name[0] >= 'A' && name[0] <= 'M')) {
		received.emplace_back(LongOption{ std::string{ name } });
		return Result::no_argument;
	} else {
		return Result::want_argument;
	}
}
void TestCallbacks::short_option_argument_callback(char const name, std::string_view const arg) {
	received.emplace_back(ShortArgumentOption{ name, std::string{ arg } });
}
void TestCallbacks::long_option_argument_callback(std::string_view const name, std::string_view const arg) {
	received.emplace_back(LongArgumentOption{ std::string{ name }, std::string{ arg } });
}
void TestCallbacks::argument_callback(std::string_view const arg) {
	received.emplace_back(PositionalArgument{ std::string{ arg } });
}

std::array const argv{ "-abzarg", "--long-no-arg", "--test-equal=arg1", "--test-space", "arg2", "positional1", "-", "--", "--positional2" };

TEST_CASE("Options are received correctly") {
	TestCallbacks callbacks;
	static ArgumentList const expected{
		ShortOption{ 'a' },
		ShortOption{ 'b' },
		ShortArgumentOption{ 'z', "arg" },
		LongOption{ "long-no-arg" },
		LongArgumentOption{ "test-equal", "arg1" },
		LongArgumentOption{ "test-space", "arg2" },
		PositionalArgument{ "positional1" },
		PositionalArgument{ "-" },
		PositionalArgument{ "--positional2" },
	};
	goblr::ArgParser::parse(argv.size(), argv.data(), callbacks);
	// expand the vector comparison algorithm, for a better error message in case of failure
	size_t const size = expected.size();
	REQUIRE(callbacks.received.size() == size);
	for (size_t i = 0; i < size; ++i) {
		CHECK(callbacks.received[i] == expected[i]);
	}
}

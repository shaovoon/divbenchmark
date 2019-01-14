#include <iomanip>
#include <chrono>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>

class timer
{
public:
	timer() = default;
	void start(const std::string& text_)
	{
		text = text_;
		begin = std::chrono::high_resolution_clock::now();
	}
	void stop()
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto dur = end - begin;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		printf("%s timing:%dms\n", text.c_str(), ms);
	}

private:
	std::string text;
	std::chrono::high_resolution_clock::time_point begin;
};

inline std::div_t my_div(int number, int divisior)
{
	return std::div_t{ (number / divisior), (number % divisior) };
}

void print_result(long long total_result)
{
	printf("Random result: %ld <== Ignore this, print here to prevent operation optimized away.\n", total_result);
}

int main()
{
	time_t curr_time = time(NULL);
	const int MAX_LOOPS = 10000000;

	timer stopwatch;
	std::int64_t n = 0;

	long long total_result = 0L;
	std::div_t result;

	int num = 0;
	int divisor = 0;
	srand(curr_time); // reset srand with same seed
	stopwatch.start("Division and Modulus");
	for (int i = 0; i < MAX_LOOPS; ++i)
	{
		num = rand();
		divisor = (num % 29) + 1;
		result.quot = num / divisor;
		result.rem = num % divisor;
		total_result += result.quot + result.rem; // prevent optimize away
	}
	stopwatch.stop();
	print_result(total_result);

	total_result = 0L;
	srand(curr_time); // reset srand with same seed
	stopwatch.start("Custom div function");
	for (int i = 0; i < MAX_LOOPS; ++i)
	{
		num = rand();
		divisor = (num % 29) + 1;
		result = my_div(num, divisor);
		total_result += result.quot + result.rem; // prevent optimize away
	}
	stopwatch.stop();
	print_result(total_result);

	total_result = 0L;
	srand(curr_time); // reset srand with same seed
	stopwatch.start("std::div function");
	for (int i = 0; i < MAX_LOOPS; ++i)
	{
		num = rand();
		divisor = (num % 29) + 1;
		result = std::div(num, divisor);
		total_result += result.quot + result.rem; // prevent optimize away
	}
	stopwatch.stop();
	print_result(total_result);

}

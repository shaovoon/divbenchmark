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

struct TwoNum
{
	int num;
	int divisor;
};

int main()
{
	time_t curr_time = time(NULL);
	const int MAX_LOOPS = 10000000;
	std::vector<TwoNum> vec;
	printf("Initializing array...\n");
	for (int i = 0; i < MAX_LOOPS; ++i)
	{
		vec.push_back({ rand(), (rand()%29)+1 });
	}

	timer stopwatch;
	std::int64_t n = 0;

	long long total_result = 0L;
	std::div_t result;

	stopwatch.start("Division and Modulus");
	for (size_t i = 0; i < vec.size(); ++i)
	{
		TwoNum& a = vec[i];
		result.quot = a.num / a.divisor;
		result.rem = a.num % a.divisor;
		total_result += result.quot + result.rem; // prevent optimize away
	}
	stopwatch.stop();
	print_result(total_result);

	total_result = 0L;
	stopwatch.start("Custom div function");
	for (size_t i = 0; i < vec.size(); ++i)
	{
		TwoNum& a = vec[i];
		result = my_div(a.num, a.divisor);
		total_result += result.quot + result.rem; // prevent optimize away
	}
	stopwatch.stop();
	print_result(total_result);

	total_result = 0L;
	stopwatch.start("std::div function");
	for (size_t i = 0; i < vec.size(); ++i)
	{
		TwoNum& a = vec[i];
		result = std::div(a.num, a.divisor);
		total_result += result.quot + result.rem; // prevent optimize away
	}
	stopwatch.stop();
	print_result(total_result);

}

# C++ 11 std::div Benchmark

C++11 standard introduces std::div() and its siblings on the premise of some compiler can take advantage of the available machine code that compute quotient and remainder of division together. The [C++ reference](https://en.cppreference.com/w/cpp/numeric/math/div) noted,

_On many platforms, a single CPU instruction obtains both the quotient and the remainder, and this function may leverage that, although compilers are generally able to merge nearby / and % where suitable._

We'll put std::div() to test in this benchmark.

## Compiler Tested

* GCC 7.4.0 on Cygwin
* Clang 5.0.1 on Cygwin
* Visual C++ 15.9 Update

OS: Windows 10 Pro

CPU: Intel i76820HQ

Loops: 10 millions

## Benchmark code

Forgive overhead of calling rand() and modulus by 29. This is a necessary evil because if I hardcoded the values, the compiler compute the results in compile-time, resulting a no-op for first 2 benchmarks. It seems that compiler cannot achieve the same feat in compile-time with std::div().

```Cpp
long long total_result = 0L;
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

```

Custom div function is defined as below.

```Cpp
inline std::div_t my_div(int number, int divisior)
{
    return std::div_t{ (number / divisior), (number % divisior) };
}
```

## Unoptimized Benchmark

```
# g++ DivBenchmark.cpp 
Division and Modulus timing:120ms
Custom div function timing:154ms
std::div function timing:106ms

# clang++ DivBenchmark.cpp -std=c++11 -stdlib=libc++
Division and Modulus timing:149ms
Custom div function timing:183ms
std::div function timing:139ms

# VC++ 15.9 Optimization disabled(/Od)
Division and Modulus timing:508ms
Custom div function timing:535ms
std::div function timing:529ms
```

On unoptimized GCC and Clang binary, std::div() is faster than my_div() and slightly faster than the individual division and modulus. For VC++, there is no difference between my_div() and std::div(); individual division and modulus is faster probably due to no function call overhead.

## Optimized Benchmark

```
# g++ DivBenchmark.cpp -O3
Division and Modulus timing:61ms
Custom div function timing:63ms
std::div function timing:81ms

# clang++ DivBenchmark.cpp -O3 -std=c++11 -stdlib=libc++
Division and Modulus timing:71ms
Custom div function timing:77ms
std::div function timing:92ms

# VC++ 15.9 Optimization enabled(/O2)(/Ot)
Division and Modulus timing:385ms
Custom div function timing:368ms
std::div function timing:379ms
```

On optimized GCC and Clang binary, it is a shame that std::div() is consistently slower. For VC++, there is no difference after the score is averaged out multiple runs. In conclusion, today's compiler already does a very good job of computing division and modulus together without resorting to std::div() but std::div() still have the performance lead when it comes to unoptimized build.




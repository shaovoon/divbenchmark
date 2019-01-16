# C++ 11 std::div Benchmark

__Update: rand() overhead in benchmark has been removed by filling the array with random values beforehand.__

C++11 standard introduces std::div() and its siblings on the premise of some compiler can take advantage of the available machine code that compute quotient and remainder of division together. The [C++ reference](https://en.cppreference.com/w/cpp/numeric/math/div) noted, and (updated) according to Petr Kobal&iacute;ček, this function was never about performance but rounding direction of negative operands. We thank him for his comment.

_Until C++11, the rounding direction of the quotient and the sign of the remainder in the built-in division and remainder operators was implementation-defined if either of the operands was negative, but it was well-defined in std::div._

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

```Cpp
long long total_result = 0L;
stopwatch.start("Division and Modulus");
for (size_t i = 0; i < vec.size(); ++i)
{
    TwoNum& a = vec[i];
    result.quot = a.num / a.divisor;
    result.rem = a.num % a.divisor;
    total_result += result.quot + result.rem; // prevent optimize away
}
stopwatch.stop();

total_result = 0L;
stopwatch.start("Custom div function");
for (size_t i = 0; i < vec.size(); ++i)
{
    TwoNum& a = vec[i];
    result = my_div(a.num, a.divisor);
    total_result += result.quot + result.rem; // prevent optimize away
}
stopwatch.stop();

total_result = 0L;
stopwatch.start("std::div function");
for (size_t i = 0; i < vec.size(); ++i)
{
    TwoNum& a = vec[i];
    result = std::div(a.num, a.divisor);
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
GCC Unoptimized
Division and Modulus timing:108ms
Custom div function timing:150ms
std::div function timing:104ms

Clang Unoptimized
Division and Modulus timing:104ms
Custom div function timing:184ms
std::div function timing:102ms

VC++ Unoptimized
Division and Modulus timing:411ms
Custom div function timing:465ms
std::div function timing:427ms
```

On unoptimized GCC and Clang binary, std::div() is faster than my_div() and slightly faster than the individual division and modulus. For VC++, individual division and modulus is faster probably due to no overhead of function call.

## Optimized Benchmark

```
GCC Optimized(O3)
Division and Modulus timing:30ms
Custom div function timing:34ms
std::div function timing:56ms

Clang Optimized(O3)
Division and Modulus timing:31ms
Custom div function timing:32ms
std::div function timing:54ms

VC++ Optimized(O2)(Ot)
Division and Modulus timing:37ms
Custom div function timing:52ms
std::div function timing:66ms
```

On optimized binary, it is a shame that std::div() is consistently slower. In conclusion, today's compiler already does a very good job of computing division and modulus together without resorting to std::div() but std::div() still have the performance lead when it comes to unoptimized build.




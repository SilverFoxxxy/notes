# Testing

## Типы тестов
- **Unit - модульные тесты**\
    Модуль работает согласно требованиям.
    - Функция
    - Класс
    - Модуль
- **Integration - интеграционные тесты**\
    Модули работают вместе согласно требованиям
- **System**\
    Система работает согласно требованиям
- **Acceptance**\
    Система работает в требуемых условиях

- **Performance - тесты производительности**
- **Smoke-тесты**
## Google Test
https://google.github.io/googletest/

_Assertions_ - statements that check whether a condition is true.
An assertion’s result can be:
- _success_
- _nonfatal failure_
- _fatal failure_

Тесты используют _assertions_.

_Test_ использует _assert_ для проверки поведения программы.

_Test suite_ состоит из одного или нескольких тестов (группа тестов).

_Test program_ состоит из нескольких _test suite_.

### Assertions
_Assert_ проверяет условие и может сгенерировать ошибку.

```cpp
# generates nonfatal failures
# don't abort the current function
# preferred to allow multiple failures
EXPECT_*

# generates fatal failure
# aborts the current function
ASSERT_*
```

```cpp
EXPECT_TRUE(my_condition) << "My condition is not true";
```

```cpp
ASSERT_EQ(x.size(), y.size()) << "Vectors x and y are of unequal length";

for (int i = 0; i < x.size(); ++i) {
  EXPECT_EQ(x[i], y[i]) << "Vectors x and y differ at index " << i;
}
```

Список _assert_-ов:
[Assertions Reference](https://google.github.io/googletest/reference/assertions.html)

### Tests

```cpp
TEST(TestSuiteName, TestName) {
  ... test body ...
}
```

```cpp
int Factorial(int n);  // Returns the factorial of n
```

A test suite for this function might look like:

```cpp
// Tests factorial of 0.
TEST(FactorialTest, HandlesZeroInput) {
  EXPECT_EQ(Factorial(0), 1);
}

// Tests factorial of positive numbers.
TEST(FactorialTest, HandlesPositiveInput) {
  EXPECT_EQ(Factorial(1), 1);
  EXPECT_EQ(Factorial(2), 2);
  EXPECT_EQ(Factorial(3), 6);
  EXPECT_EQ(Factorial(8), 40320);
}
```

### TestSuite (~~TestCase~~) - набор тестов
```cpp
// Tests factorial of 0.
TEST(FactorialTest, HandlesZeroInput) {
  EXPECT_EQ(Factorial(0), 1);
}

// Tests factorial of positive numbers.
TEST(FactorialTest, HandlesPositiveInput) {
  EXPECT_EQ(Factorial(1), 1);
  EXPECT_EQ(Factorial(2), 2);
  EXPECT_EQ(Factorial(3), 6);
  EXPECT_EQ(Factorial(8), 40320);
}

// Tests negative input.
TEST(IsPrimeTest, Negative) {
  // This test belongs to the IsPrimeTest test suite.

  EXPECT_FALSE(IsPrime(-1));
  EXPECT_FALSE(IsPrime(-2));
  EXPECT_FALSE(IsPrime(INT_MIN));
}

// Tests some trivial cases.
TEST(IsPrimeTest, Trivial) {
  EXPECT_FALSE(IsPrime(0));
  EXPECT_FALSE(IsPrime(1));
  EXPECT_TRUE(IsPrime(2));
  EXPECT_TRUE(IsPrime(3));
}

// Tests positive input.
TEST(IsPrimeTest, Positive) {
  EXPECT_FALSE(IsPrime(4));
  EXPECT_TRUE(IsPrime(5));
  EXPECT_FALSE(IsPrime(6));
  EXPECT_TRUE(IsPrime(23));
}
```

### Test Fixtures: Using the Same Data Configuration for Multiple Tests
```c++
TEST_F(_TestFixtureName_, _TestName_) {
  ... _statements_ ...
}
```

`ctest` запускает тесты независимо друг от друга.
Чтобы заработало `SetUpTestSuite` стоит добавить в файл с тестами:
```c++
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

И запускать получающийся исполняемый файл, а не `ctest`.

```c++
class TestPrimeFixtures : public ::testing:Test {
  public:
    static vector <int> primes_;
    vector <int> base_primes_;

    // for multiple tests
    static void SetUpTestSuite();

    // for single test (constructor can be used)
    void SetUp() override;

    // for single test (destructor can be used)
    void TearDown() override;

    // for multiple tests
    static void TearDownTestSuite();
}

void TestPrimeFixture::SetUp() {
    base_primes_ = {2, 3, 5, 7, 11, 13, 998244353, 1000000007};
}

void TestPrimeFixture::SetUpTestSuite() {
    // get primes to 1e7 into base_primes_
}

TEST_F(TestPrimeFixture, BasePrimes) {
    //
}

TEST_F(TestPrimeFixture, ALotOfPrimes) {
    //
}
```

### Death tests
Note that a death test only cares about three things:
1. does `statement` abort or exit the process?
2. (in the case of `ASSERT_EXIT` and `EXPECT_EXIT`) does the exit status satisfy `predicate`? Or (in the case of `ASSERT_DEATH` and `EXPECT_DEATH`) is the exit status non-zero? And
3. does the stderr output match `matcher`?

```c++
int Factorial(int n) {
    long long fact = 1;
    for (int i = 2; i <= n; i++) {
        fact *= i;
        if (fact > INT_MAX) {
            std::cerr << "Factorial is too big!" << std::endl;
            std::exit(1);
        }
    }
    return fact;
}
```

```c++
TEST(FactorialDeathTest, FactorialTooBig) {
  // This death test uses a compound statement.
  EXPECT_DEATH({
    int n = 100;
    Factorial(n);
  }, "Factorial is too big!");

  EXPECT_EXIT({
    int n = 200;
    Factorial(n);
  }, testing::ExitedWithCode(1), "Factorial is too big!");
}
```

### Mock tests
Add `gmock_main` to `CMakeLists`:
```cmake
target_link_libraries(
  my_math_test
  my_math
  GTest::gmock_main
  GTest::gtest_main
)
```

Add `#include <gmock/gmock.h>` to `my_math_test.cc`
```c++
#include <gmock/gmock.h>
```

Add `mock test` to `my_math_test.cc`
```c++
class BaseLogger {
public:
    virtual void Log(const string& s) = 0;

    virtual int GetBufferSize() = 0;
};

class MockLogger : public BaseLogger {
public:
    MOCK_METHOD(void, Log, (const string& s), (override));
    MOCK_METHOD(int, GetBufferSize, (), (override));
};

bool IsPrime(int n, BaseLogger& logger) {
    logger.Log("IsPrime called!");
    if (n <= 1) {
        return false;
    }

    for (int i = 2; i < n; i++) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

TEST(LoggerTest, SimpleTest) {
    MockLogger logger;
    EXPECT_CALL(logger, Log)
        .Times(testing::AtLeast(1));

    EXPECT_TRUE(IsPrime(7, logger));
}
```

# Примеры кода

## 1. Переменные и арифметические операции
```cpp
#include <iostream>

using std::cin;
using std::cout;

int main() {
  int a = 0;
  int b = 0;
  // Enter two numbers:
  cin >> a >> b;
  int sum = a + b;
  int diff = a - b;
  int prod = a * b;
  int quot = a / b;  // integer division
  cout << "Sum: " << sum << "\n";
  cout << "Difference: " << diff << "\n";
  cout << "Product: " << prod << "\n";
  cout << "Quotient: " << quot << "\n";
  return 0;
}
```

## 2. Условные конструкции
```cpp
#include <iostream>

using std::cin;
using std::cout;

int main() {
  int x = 0;
  cout << "Enter a number: ";
  cin >> x;
  if (x > 0) {
    cout << "Positive number\n";
  } else if (x == 0) {
    cout << "Zero\n";
  } else {
    cout << "Negative number\n";
  }
  return 0;
}

```

## 3. Циклы
```cpp
#include <iostream>

using std::cout;

int main() {
  cout << "For loop: ";
  for (int i = 1; i <= 5; ++i) {
    cout << i << " ";
  }
  cout << "\n";

  cout << "While loop: ";
  int j = 1;
  while (j <= 5) {
    cout << j << " ";
    ++j;
  }
  cout << "\n";
  return 0;
}

```

## 4. Вектор
```cpp
#include <iostream>
#include <vector>

using std::cout;
using std::vector;

int main() {
  vector<int> numbers = {1, 2, 3, 4, 5};
  for (int number : numbers) {
    cout << number << " ";
  }
  cout << "\n";
  return 0;
}

```

## 5. Функции
```cpp
#include <iostream>

using std::cout;

int Sum(int a, int b) {
  return a + b;
}

int main() {
  int result = Sum(3, 4);
  cout << "Sum: " << result << "\n";
  return 0;
}

```

## 6. Передача аргумента по ссылке
```cpp
#include <iostream>
#include <vector>

using std::cout;

void Swap(int& a, int& b) {
  int temp = a;
  a = b;
  b = temp;
}

int main() {
  vector<int> values = {10, 20};
  cout << "Before swap: " << values[0] << ", " << values[1] << "\n";
  Swap(values[0], values[1]);
  cout << "After swap: " << values[0] << ", " << values[1] << "\n";
  return 0;
}

```

```cpp
#include <iostream>
#include <vector>

using std::cout;
using std::vector;

// Function replaces each element with its square, vector passed by reference
void SquareElements(vector<int>& numbers) {
  for (int& num : numbers) {
    num = num * num;
  }
}

int main() {
  vector<int> data = {1, 2, 3, 4, 5};
  SquareElements(data);
  for (int num : data) {
    cout << num << " ";
  }
  cout << "\n";
  return 0;
}
```

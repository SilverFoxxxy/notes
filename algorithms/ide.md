# IDE

**IDE** - Integrated Development Environment.

## 1. Установка компилятора.

Создайте файл (в любом текстовом редакторе) `A.cpp` и вставьте следующий код:

```cpp
#include <iostream>

using std::cout;

int main() {
    cout << "Hello, World!" << "\n";
}

```

Текущая цель - скомпилировать и запустить этот код.

Открываем терминал, находим файл `A.cpp`.

Запускаем команду `g++ A.cpp`.

`g++` - команда компилятора.

### 1.2. Установка компилятора.

Нужно чтобы команда `g++ -v` выводила версию компилятора.

Как этого добиться:\
Открываем [perplexity](https://www.perplexity.ai/) и спрашиваем "что делать если у меня g++ не распознаётся как команда, моя операционная система - Windows / Mac OS / Ubuntu (нужное подчеркнуть)".

Короткая версия:


<details>
<summary> Ubuntu </summary>
GCC обычно установлен по умолчанию.

Если нет, установите через терминал:
```sh
$ sudo apt update
$ sudo apt install build-essential
```

Проверьте командой:
```sh
g++ --version
```
</details>

<details>
<summary> Windows </summary>
Если нужен именно gcc/g++, можно установить MinGW или MSYS2.

Для MinGW: скачайте и установите MinGW, затем добавьте его bin-папку в переменную окружения PATH.

Проверьте установку в консоли командой:
```sh
g++ --version
```

</details>

<details>
<summary> Mac OS </summary>

Установите Homebrew, введя в терминале команду:
```sh
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

После установки Homebrew установите GCC командой:
```sh
brew install gcc
```

Для использования gcc как g++ по умолчанию выполните:
```sh
cd /opt/homebrew/bin
ln -s g++-12 g++
```

Проверьте установку:
```sh
g++ --version
```

</details>



### 1.3. Первый запуск.

Когда команда `g++ -v` у вас заработала:

```sh
$ g++ -v
# Вывод на Mac OS
Apple clang version 15.0.0 (clang-1500.3.9.4)
Target: arm64-apple-darwin23.6.0
Thread model: posix
InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin

$ g++ A.cpp
# Появился файл a.out

$ ls
A.cpp a.out # <- вот он

 # Запускаем программу:
$ ./a.out
Hello, World!
```

### 2. Настройка IDE

В качестве **IDE** рассмотрим применимый во множестве областей редактор **Visual Studio Code**, обладающий множеством расширений под различные нужды разработчиков.

1. Устанавливаем **VS Code**.
    - Именно **VS Code**, а не **Visual Studio**!
2. Устанавливаем расширение **Code Runner** и настраиваем его:
    - Заходим в настройки Code Runner.
    - Находим опцию `Run in Terminal`, ставим галочку.
    - Находим `Executor Map`, заменяем там строчку для запуска `.cpp` файлов на следующую:
    ```json
    "cpp": "g++ -std=c++20 $fileName && ./a.out",
    ```
    - Готово! Теперь при открытии `A.cpp` в IDE и нажатии на треугольник `Run` (из опций Run нужно выбрать Code Runner) ваша программа скомпилируется и запустится в терминале. (Или выдаст ошибку компиляции).
3. Настройки в IDE:
    - Стоит включить опции `Trim Trailing Whitespace` и `replace tabs with spaces`. На большинстве курсов tab = 2 пробела.

### 3. Опции компилятора

Полезные опции компилятора:

```sh
# Выбираем стандарт C++ 20-го года
$ g++ -std=c++20 A.cpp && ./a.out

# Выбираем оптимизацию O2. На большинстве тестирующих систем она используется.
$ g++ -O2 A.cpp && ./a.out

# Запускаем с санитайзером. Помогает найти утечки памяти и обращения не к своей памяти. (Т.е. когда ловим Runtime Error). Замедляет работу программы, на большом тесте просто упадёт из-за оверхеда санитайзера, используйте на маленьких тестах.
$ g++ -fsanitize=address -g A.cpp && ./a.out
```

## Сборка проекта на C++
https://thedevscave.com/ru/posts/c_building_process.html#assembly
Этапы сборки проекта на C++:
1. **Препроцессинг**:
    - На этом этапе обрабатываются директивы препроцессора, такие как `#include` и `#define`.
    - Выполняется подстановка файлов, замена символов и другие преобразования.
2. **Компиляция**:
    - Компилятор преобразует предобработанный код в ассемблерный код.
    - Происходит проверка на лексические и синтаксические ошибки.
    - Результатом является ассемблерный файл (.s или .asm).
3. **Ассемблирование**:
    - Ассемблер преобразует ассемблерный код в объектный код.
    - Создаются объектные файлы (.obj).
4. **Компоновка (Линковка)**:
    - Компоновщик объединяет объектные файлы в исполняемый файл (EXE), динамическую библиотеку (DLL) или статическую библиотеку (LIB).
    - Разрешаются зависимости и создается конечный исполняемый файл.
### Препроцессинг
На этом этапе заменяются все **директивы препроцессора**:
- `#include`
- `#define`
- `#pragma`
- `#if`
- `#error`

Пример:
```sh
echo ';' > semicolon
```

`main.c`
```c
int main(void) {
    int a = 10
#include "semicolon"
    int b = 15
#include "semicolon"
    return a + b
#include "semicolon"
}
```

```sh
gcc main.c -o no_semicolon
./no_semicolon

# выводим код завершения последней выполненной команды
echo $?
```
#### cpp - C Pre Processor
```sh
cpp main.c
```

#### define
```c
#ifndef HELL_C_LANGUAGE
#define HELL_C_LANGUAGE

#define HELL_C_MAIN_FUNC main
#define HELL_C_GET_RESULT return
#define HELL_C_INTEGER int
#define HELL_C_BLANK void
#define HELL_C_SUCCESS 0
#define HELL_C_FAIL 1
#define HELL_C_CONDITION if
#define HELL_C_ELSE_CONDITION else

#endif /* HELL_C_LANGUAGE */
```

И после этого мы сможем писать вот такие программы:
```c
#include "hell_c_language.h"

HELL_C_INTEGER HELL_C_MAIN_FUNC(HELL_C_BLANK) {
    HELL_C_INTEGER a = 10;
    HELL_C_CONDITION (a % 2 == 0) {
        HELL_C_GET_RESULT HELL_C_SUCCESS;
    } HELL_C_ELSE_CONDITION {
        HELL_C_GET_RESULT HELL_C_FAIL;
    }
}
```

https://vkvideo.ru/video-170661777_456243921

Такой формат используется для того, чтобы избежать ошибок при множественном копировании файла:
```
#ifndef FILENAME_H
#define FILENAME_H
...
#endif /* FILENAME_H */
```
### Компиляция
`hello.c`
```c
#include <stdio.h>

int main(void) {
    printf("Hello World\n");
    return 0;
}
```

```sh
$ ls                    # Получаем список файлов в текущей директории
hello.c
$ cpp hello.c > hello.i # Сохраняем исходный код после препроцессора в hello.i
$ /usr/lib/gcc/x86_64-pc-linux-gnu/13.2.1/cc1 hello.i -o hello.s  # Собираем код на языке Ассемблера
$ ls                    # Получаем список файлов в текущей директории
hello.c  hello.i  hello.s
```

### Ассемблирование
Из кода на ассемблере получаем объектный файл.
```sh
as hello.s -o hello.o
```

### Линковка
Объединяет несколько объектных файлов воедино и собирает исполняемый файл.\
Линковщик выполняет это в 2 этапа:
1. Нахождение ссылок (Symbols resolution)
2. Перемещение ссылок (Symbols relocation)

```sh
ld hello.o

--

ld: warning: cannot find entry symbol _start; defaulting to 0000000000401000
ld: hello.o: in function `main':
hello.c:(.text+0xf): undefined reference to `puts'
```

Компилятор самостоятельно заменяет точку входа на `main`.
Выполним замену в IDE.

```
ld hello.o

ld: hello.o: in function `main':
hello.c:(.text+0xf): undefined reference to `puts'
```

### Библиотеки
#### Статические

#### Динамические

## Make
`main.c`
```c
#include <stdio.h>
#include "my_lib.h"

int main(int argc, char *argv[]) {
    printf("foo(5) = %d\n", foo(5));
    return 0;
}
```

`my_lib.h`
```c
#ifndef MY_LIB_H
#define MY_LIB_H

int foo(int x);

#endif
```

`my_lib.c`
```c
int foo(int x) {
    return x * x;
}
```

### Makefile
```makefile
target : dependency_1 dependency_2
    command_to_get_target_1
    command_to_get_target_1

target_2 : dependency_3
```

`Makefile`
```makefile

MyProject : main.o my_lib.o
    gcc main.o my_lib.o -o MyProject

main.o : main.c
    gcc -c main.c -o main.o

my_lib.o : my_lib.c
    gcc -c my_lib.c -o my_lib.o
```

```sh
make MyProject
```

Если видите ошибку - попробуйте заменить в Makefile пробелы на табы.

При повторном вызове команды `make`:
```sh
make
--
make: `MyProject' is up to date.
```

Добавим новую цель: `clean`
```makefile
MyProject : main.o my_lib.o
    gcc main.o my_lib.o -o MyProject

main.o : main.c
    gcc -c main.c -o main.o

my_lib.o : my_lib.c
    gcc -c my_lib.c -o my_lib.o

clean :
    rm MyProject *.o
```

Используем переменные:
```makefile
TARGET = MyProject
CC = gcc

$(TARGET) : main.o my_lib.o
    $(CC) main.o my_lib.o -o $(TARGET)

main.o : main.c
    $(CC) -c main.c -o main.o

my_lib.o : my_lib.c
    $(CC) -c my_lib.c -o my_lib.o

clean :
    rm $(TARGET) *.o
```

Получим список всех файлов с кодом:
```makefile
TARGET = MyProject
CC = gcc

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

$(TARGET) : $(OBJ)
    $(CC) $(OBJ) -o $(TARGET)

main.o : main.c
    $(CC) -c main.c -o main.o

my_lib.o : my_lib.c
    $(CC) -c my_lib.c -o my_lib.o

clean :
    rm $(TARGET) *.o
```

```makefile
TARGET = MyProject
CC = gcc

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

$(TARGET) : $(OBJ)
    $(CC) $(OBJ) -o $(TARGET)

%.o : %.c
    $(CC) -c $< -o $@

clean :
    rm $(TARGET) *.o

```

```sh
make clean

make
```

Переместим файлы в соответствующие папки.
```makefile
TARGET = MyProject
CC = gcc

PREF_SRC = ./src/
PREF_OBJ = ./obj/

SRC = $(wildcard $(PREF_SRC)*.c)
OBJ = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC))

$(TARGET) : $(PREF_OBJ)$(OBJ)
    $(CC) $(PREF_OBJ)$(OBJ) -o $(TARGET)

$(PREF_OBJ)%.o : $(PREF_SRC)%.c
    $(CC) -c $< -o $@

clean :
    rm $(TARGET) $(PREF_OBJ)*.o
```

Проблемы make:
- Привязка к платформе
- Возможно что сборка начнётся и в конце обнаружит что необходимая библиотека отсутствует
## CMake
### Пример 0
```sh
.
├── CMakeLists.txt
└── main.cpp
```

```cmake
cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.

add_executable(main main.cpp)		# Создает исполняемый файл с именем main
									# из исходника main.cpp
```

```sh
$ mkdir tmp
$ cd tmp
tmp $ cmake ..
tmp $ ls
tmp $ make
tmp $ ls
tmp $ ./main
```
### Пример 1. Подключение библиотеки
```sh
.
├── CMakeLists.txt
├── foo.cpp
├── foo.h
└── main.cpp
```

```cmake
cmake_minimum_required(VERSION 2.8)	 # Проверка версии CMake.
										# Если версия установленной программы
										# старее указаной, произойдёт аварийный выход.

project(hello_world)			# Название проекта

set(SOURCE_EXE main.cpp)		# Установка переменной со списком исходников для исполняемого файла

set(SOURCE_LIB foo.cpp)			# Тоже самое, но для библиотеки

add_library(foo STATIC ${SOURCE_LIB})	# Создание статической библиотеки с именем foo

add_executable(main ${SOURCE_EXE})	# Создает исполняемый файл с именем main

target_link_libraries(main foo)		# Линковка программы с библиотекой
```

Объявление переменных (списки):
```cmake
set(SOURCE main.cpp foo.cpp)
set(HEADER main.h
			foo.h)
```

Подстановка переменных:
```cmake
${var_name}
```

Подключение библиотеки:
```cmake
# Подключение статической библиотеки
add_library(foo STATIC ${SOURCE_LIB})

# Подключение динамической библиотеки
add_library(foo SHARED ${SOURCE_LIB})
```

При линковке указываем необходимые библиотеки:
```cmake
target_link_libraries(main  foo
							ogg
							vorbis)
```
### Пример 2.  Подпроекты

```sh
.
├── CMakeLists.txt
├── foo
│   ├── CMakeLists.txt
│   ├── foo.cpp
│   └── foo.h
└── main.cpp
```

`CMakeLists.txt`
```cmake
cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленной программы
									# старее указаной, произойдёт аварийный выход.

project(hello_world)				# Название проекта

set(SOURCE_EXE main.cpp)			# Установка переменной со списком исходников

include_directories(foo)			# Расположение заголовочных файлов

add_executable(main ${SOURCE_EXE})	# Создает исполняемый файл с именем main

add_subdirectory(foo)				# Добавление подпроекта, указывается имя дирректории

target_link_libraries(main foo)		# Линковка программы с библиотекой
```

`foo/CMakeLists.txt`
```cmake
cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
# Если версия установленной программы
# старее указаной, произойдёт аварийный выход.

project(foo) # Название проекта

set(SOURCE_LIB foo.cpp) # Установка переменной со списком исходников

add_library(foo STATIC ${SOURCE_LIB})# Создание статической библиотеки
```

Указываем где искать заголовочные файлы:
```cmake
include_directories(foo)
```

Указываем директорию с подпроектом:
```cmake
add_subdirectory(foo)
```

### Пример 3. Включаем доп опции

Синтаксис добавления булевой опции:
```cmake
option(<variable> "<help_text>" [value])

# <variable> - имя опции
# "<help_text>" - описание, будет показано при конфигурации
# [value] - ON / OFF значение, по умолчанию OFF
```

Пример:
```
option(USE_MYMATH "Use Module Math" ON)
```

`main.cpp`
```cpp
#include <iostream>

using namespace std;

#ifdef USE_MYMATH
#include <MathFunctions.h>
#else
#include <cmath>
#endif

int main() {
#ifdef USE_MYMATH
    cout << MySqrt(-9) << endl;
#else
    cout << sqrt(-9.0) << endl;
#endif

    return 0;
}
```

`CMakeLists.txt`
```cmake
cmake_minimum_required(VERSION 3.10)
project(Math)

option(USE_MYMATH "Use Module Math" ON)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}.bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

if (USE_MYMATH)
    add_definitions(-DUSE_MYMATH)
    include_directiories("${PROJECT_SOURCE_DIR}/MathFunctions")
    add_subdurectory(MathFuctions)
    set(EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
endif(USE_MYMATH)

add_executable(Math main.cpp)
target_link_libraries(Math ${EXTRA_LIBS})

install(TARGETS Math DESTINATION bin)
```


### Пример 4. Поиск библиотек
CMake обладает достаточно развитыми средствами поиска установленых библиотек, правда они не встроеные, а реализованы в виде отдельных модулей. В стандартной поставке довольно много модулей, но некоторые проекты (например Ogre) поставляют свои. Они позволяют системе автоматически определить наличие необходимых для линковки проекта библиотек.
На debian модули располагаются в /usr/share/cmake-2.8/Modules/ (у вас версия может отличаться). За поиск библиотек отвечают модули, называющиеся FindNAME.cmake, где NAME — имя библиотеки.
find_package(SDL REQUIRED)
```cmake
if(NOT SDL_FOUND)
	message(SEND_ERROR "Failed to find SDL")
	return()
else()
	include_directories(${SDL_INCLUDE_DIR})
endif()
##########################################################
find_package(LibXml2 REQUIRED)
if(NOT LIBXML2_FOUND)
	message(SEND_ERROR "Failed to find LibXml2")
	return()
else()
	include_directories(${LIBXML2_INCLUDE_DIR})
endif()
##########################################################
find_package(Boost COMPONENTS thread-mt REQUIRED)
if(NOT Boost_FOUND)
	message(SEND_ERROR "Failed to find boost::thread-mt.")
	return()
else()
	include_directories(${Boost_INCLUDE_DIRS})
endif()
##########################################################
target_link_libraries(${TARGET}
								${SDL_LIBRARY}
								${LIBXML2_LIBRARIES}
								${Boost_LIBRARIES})
```

 Первый и второй блок — поиск библиотеки. Если в системе её нет, выведется сообщение об ошибке и завершается выполнение cmake. Третий блок похож, только он ищет не целый пакет библиотек, а лишь необходимый компонент.

Каждый такой автоматизированый поиск определяет после выполнения как минимум 3 переменные:\
SDL_FOUND, LIBXML2_FOUND, Boost_FOUND — признак присутствия бибилиотеки;\
SDL_LIBRARY, LIBXML2_LIBRARIES, Boost_LIBRARIES — имена библиотек для линковки;\
SDL_INCLUDE_DIR, LIBXML2_INCLUDE_DIR, Boost_INCLUDE_DIRS — пути к заголовочным файлам.


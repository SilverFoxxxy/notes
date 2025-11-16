# Настройка запуска скриптами

Создадим два скрипта - один для обычного запуска, другой для отладки.

`run`
```sh
#!/bin/bash

g++ -std=c++20 -O2 "${1}.cpp" && ./a.out
```

`dbg`

```sh
#!/bin/bash

g++ -std=c++20 -O2 -fsanitize=address -g "${1}.cpp" && ./a.out
```

После создания скриптов нужно сделать файлы исполняемыми:
```sh
chmod +x run
chmod +x dbg
```

Теперь код `A.cpp` можно в терминале запускать так!

```sh
./run A
```

Или с дебагом:
```sh
./dbg A
```

Пример кода в файле `A.cpp`:

```cpp
#include <bits/stdc++.h>

using namespace std;

int main() {
    cout << "Hello, World!\n";
}
```

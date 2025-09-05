## Предпосылки

Хотим разрабатывать ПО!

**Жизненный цикл разработки:**
- Анализ требований
- Проектирование
- Разработка
- Тестирование и отладка
- Эксплуатация и сопровождение
- Вывод из эксплуатации

## Примеры

### Пример 1
Что делает этот код?
```python
def proc(lst, x):
    res = []
    for i in lst:
        if i % x == 0:
            res.append(i)
    return res
```

#### Решение
```python
def filter_multiples(numbers, divisor):
    multiples = []
    for number in numbers:
        if number % divisor == 0:
            multiples.append(number)
    return multiples
```

### Пример 2
Что стоит сделать с этим кодом?
```cpp
#include <vector>

bool contains(const vector <T>& v, const T& x) {
    for (auto& y: v) {
        if (x == y) {
            return true;
        }
    }
    return false;
}

contains({1, 2, 3}, 5);

contains({"abra", "cadabra"}, "curse");
```

#### Решение
Ничего, пока мы не заметим, что он работает медленно!\
На массивах небольшой длины работает за приемлемое время.

### Пример 3
```python
class Database:
    def __init__(self):
        self.connection_string = "database-connection-string"

    def query(self, sql):
        # Executes SQL query using the connection string
        print(f"Querying database with connection {self.connection_string}")
        print(f"Execute SQL: {sql}")

class BusinessLogic:
    def __init__(self):
        self.db = Database()
    def process_data(self):
        threshold = 10
        sql = f"SELECT * FROM records WHERE value > {threshold}"
        self.db.query(sql)
logic = BusinessLogic()
logic.process_data()
```

#### Решение
Видим высокую связность между классами (один класс опирается на внутреннюю реализацию другого).

Упражнение:
- Добавить поддержку другой базы данных
- Что если объединить логику в один класс? (нарушит принцип single responsibility)

## Итого:
Если код менять и поддерживать сложно - пора его переписывать!

**Красивый код** - это **код**:
- Работающий
- Читаемый
- Расширяемый
- Тестируемый

**Красивая система** - это **система**:
- Работающая
- Расширяемая
- Тестируемая

Как такие системы проектировать и писать в них код?\
Разберёмся на курсе!

На этапе **Анализ требований** учитываем:
- Функциональные требования
    - Личный кабинет
    - Сервис оплаты
    - Просмотр картинок
- Нефункциональные требования
    - Uptime 99.9%
    - 1000 пользователей / сутки

Хотим чтобы код был **расширяемый** (изменяемый).\
Для этого стоит учесть, какие **новые** требования появятся через год жизни продукта.

Получаем таблицу:
|Требования |Сейчас | Через год|
|-|-|-|
|Функциональные|Личный кабинет<br>Текстовые сообщения<br>...|Посты<br>Картинки<br>Голосовые сообщения|
|Нефункциональные|Uptime 99%<br>1000 пользователей / час<br>1 сообщение в день (среднее)<br>10 сообщений в день (макс)|Uptime 99.99%<br>1000000 пользователей / час<br>10 сообщений в день (среднее)<br>100 сообщений в день (макс)|

На курсе говорим про **функциональные требования**.

Чтобы разобраться с нефункциональными - вам на курс **Проектирование высоконагруженных систем (Highload)**.


<!-- ### Некоторые принципы (примеры)
- Чего хотим от класса (модуля)?
- Инкапсуляция
- Open-Closed Principle
- Single Responsibility
- Low Coupling, High Cohession -->

<!-- ## Design patterns


### 1. [Creational](creational) -->

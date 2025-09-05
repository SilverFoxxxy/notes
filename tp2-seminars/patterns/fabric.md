# Паттерны проектирования

## Задача: Система логирования

**Задача:** Нужно реализовать систему логирования, которая может записывать сообщения в разные места: в файл, в консоль, в базу данных или отправлять по сети. Место назначения лога определяется динамически: конфигурационным файлом, переменной окружения или даже содержимым самого сообщения (например, критические ошибки сразу отправлять в Telegram).

<details>
<summary> Подсказка 1</summary>

> Воспользуйтесь **наследованием** для нескольких реализаций одного и того же функционала.
</details>

<details>
<summary> Решение 1</summary>

### Решение 1 - наследование
```java
// Общий интерфейс для всех логгеров
interface Logger {
    void log(String message);
}

// Конкретные реализации
class FileLogger implements Logger {
    private String filePath;
    public FileLogger(String path) { this.filePath = path; }
    public void log(String message) { /* запись в файл */ }
}

class ConsoleLogger implements Logger {
    public void log(String message) { /* вывод в консоль */ }
}

class DatabaseLogger implements Logger {
    private Connection dbConnection;
    public DatabaseLogger(Connection conn) { this.dbConnection = conn; }
    public void log(String message) { /* запись в БД */ }
}
```

Использование:
```java
String loggerType = Config.get("logger_type"); // Читаем из конфига

if ("file".equals(loggerType)) {
    String path = Config.get("logger_file_path"); // Еще параметр
    this.logger = new FileLogger(path);
} else if ("db".equals(loggerType)) {
    String connectionString = Config.get("db_connection");
    Connection conn = DriverManager.getConnection(connectionString); // Сложная логика создания
    this.logger = new DatabaseLogger(conn);
} else {
    this.logger = new ConsoleLogger();
}
```

</details>

<br>

<details>
<summary> Подсказка 2</summary>

> Проблемы:
> - Клиентский код (Application) должен знать ВСЕ детали создания ВСЕХ логгеров.
> - Каждый раз когда логгируем - пишем полотно кода с условными операторами.

> Упражнение:
> - Добавить новый способ логирования
> - Протестировать функцию выше
</details>

<details>
<summary> Решение 2</summary>

Хотим спрятать сложную логику создания логгера в метод.

### Решение 2 - наследование + static методы создания
```java
// Класс со статическими методами-создателями
class LoggerCreator {
    public static Logger createFileLogger() {
        return new FileLogger(Config.getPath()); // Уже лучше, детали внутри
    }

    public static Logger createDatabaseLogger() {
        Connection conn = DriverManager.getConnection(Config.getDbUrl());
        return new DatabaseLogger(conn);
    }
}

// Клиентский код
Logger fileLog = LoggerCreator.createFileLogger();
Logger dbLog = LoggerCreator.createDatabaseLogger();
```

</details>

<br>

<details>
<summary> Подсказка 3</summary>

> Вынесем логику создания в **отдельный класс**.

</details>

<details>
<summary> Решение 3</summary>

### Решение 3 - наследование + фабрика
```java
// Фабрика инкапсулирует знание о том, КАКОЙ логгер и КАК создать.
class LoggerFactory {
    public static Logger getLogger() {
        String loggerType = Config.get("logger_type");

        // Вся "грязь" и сложность создания изолирована здесь, в одном месте
        if ("file".equals(loggerType)) {
            String path = Config.get("logger_file_path");
            return new FileLogger(path);
        } else if ("db".equals(loggerType)) {
            String connectionString = Config.get("db_connection");
            Connection conn = DriverManager.getConnection(connectionString);
            return new DatabaseLogger(conn);
        } else if ("telegram".equals(loggerType)) {
            String botToken = Config.get("telegram_bot_token");
            String chatId = Config.get("telegram_chat_id");
            return new TelegramLogger(botToken, chatId); // Добавили новый логгер
        } else {
            return new ConsoleLogger();
        }
    }
}
```

</details>

## Задача: Кроссплатформенный UI

Нужно создать приложение, которое будет работать и выглядеть аутентично как на Windows, так и на macOS. Для этого нужны разные реализации кнопок, чекбоксов, текстовых полей.

### Создаем семейства продуктов (Интерфейсы и реализации)

Сначала определим общие интерфейсы для всех элементов UI:

<details>
<summary> Подсказка 1</summary>

> Воспользуйтесь **наследованием** для нескольких реализаций одного и того же функционала.
</details>

<details>
<summary> Решение 1</summary>

Получим следующий код.

```java
// 1. Интерфейс кнопки
interface Button {
    void render();
    void onClick();
}

// Реализация для Windows
class WindowsButton implements Button {
    public void render() {
        System.out.println("Отрисована кнопка в стиле Windows 11");
    }
    public void onClick() {
        System.out.println("Звук клика Windows");
    }
}

// Реализация для macOS
class MacOSButton implements Button {
    public void render() {
        System.out.println("Отрисована кнопка в стиле macOS");
    }
    public void onClick() {
        System.out.println("Звук клика macOS");
    }
}

// 2. Интерфейс чекбокса
interface Checkbox {
    void render();
}

// Реализация для Windows
class WindowsCheckbox implements Checkbox {
    public void render() {
        System.out.println("Отрисован чекбокс в стиле Windows");
    }
}

// Реализация для macOS
class MacOSCheckbox implements Checkbox {
    public void render() {
        System.out.println("Отрисован чекбокс в стиле macOS");
    }
}
```

</details>

<br>

<details>
<summary> Подсказка 2</summary>

> Вынесем логику выбора из клиентского кода.
</details>

<details>
<summary> Решение 2</summary>

Получим фабрику со сложной логикой - в процессе добавления новой ОС / нового элемента UI можно упустить часть логики, читаемость и тестирование усложнены.
</details>

<br>

<details>
<summary> Подсказка 3</summary>

> Полностью разделим реализацию для разных ОС!
</details>

<details>
<summary> Решение 3</summary>

Определим **Абстрактную фабрику** - интерфейс, который любая фабрика должна реализовывать (создавать кнопки и чекбоксы).

Тогда `WindowsFactory` и `MacOSFactory`, реализующие этот интерфейс, будут обязаны определить соответствующий функционал!

Логика полностью разделена - подумайте, как в неё вносятся изменения (добавить поддержку нового элемента UI или новой ОС).

```java
// Абстрактная фабрика объявляет методы для создания ВСЕХ продуктов семейства
interface GUIFactory {
    Button createButton();
    Checkbox createCheckbox();
}
```

```java
// Фабрика для Windows создает все элементы в стиле Windows
class WindowsFactory implements GUIFactory {
    public Button createButton() {
        return new WindowsButton();
    }
    public Checkbox createCheckbox() {
        return new WindowsCheckbox();
    }
}

// Фабрика для macOS создает все элементы в стиле macOS
class MacOSFactory implements GUIFactory {
    public Button createButton() {
        return new MacOSButton();
    }
    public Checkbox createCheckbox() {
        return new MacOSCheckbox();
    }
}
```
</details>


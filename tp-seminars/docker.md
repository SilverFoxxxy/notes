## Контейнеризация и Docker
Проблема: *У меня на машине работает*
Решение: *Пользовательское окружение*

- Упаковка приложения в контейнер (или в несколько контейнеров)
- Упрощает тестирование
- Упрощает локальную разработку
- Повышает скорость применения

## Виртуализация vs контейнеризация
| -            | Виртуализация      | Контейнеризация                                   |
| ------------ | ------------------ | ------------------------------------------------- |
| Ресурсы      | Забирает полностью | Использует по мере необходимости                  |
| Как работает | Эмулирует железо   | Одно ядро ОС на всех + механизмы ОС для изоляции: |
|              |                    | - cgroups (cpu, cpuset, memory, device)           |
|              |                    | - namespaces (PID, MNT, IPC, UTS, NET)            |

## Виртуализация
**Виртуальная ОС на эмулированном железе**
- Повышение изоляции и уровня безопасности
- Распределение ресурсов
- Постоянная доступность
- Повышение качества администрирования
### Виды виртуализации
- Аппаратная
    Сервер -> Гипервизор -> OS1, OS2, ...
- Программная
    Сервер -> ОС -> Гипервизор -> OS1, OS2, ...
- Паравиртуализация
    (?) Особое железо
- Контейнерная ~~виртуализация~~
    Перестали считать видом виртуализации

*Гипервизор* - способ виртуализации железа

## Контейнеризация
### Механизмы контейнеризации
- [Раньше] Использовали контейнеры как виртуальные машины.
- [Стало (пришёл docker и придумал)] Изолируем один процесс в одном контейнере.

## Docker
- Система управления контейнерами.
Возможности:
- Упаковка приложений и зависимостей в контейнеры
- Минимальное потребление ресурсов
- Скоростное развёртывание
- Изоляция кода и процессов
- Простое масштабирование
- Удобный запуск

| Client                                      | DOCKER_HOST   | Registry             |
| ------------------------------------------- | ------------- | -------------------- |
| docker build                                | docker daemon | images / base images |
| docker pull                                 | - containers  |                      |
| docker run                                  | - images      |                      |
| Может работать<br>с удалённым docker daemon |               |                      |
|                                             |               |                      |

DOCKER_HOST - переменная окружения\
Можно выставить DOCKER_HOST на удалённом сервере.

Registry может быть приватным с доступом через docker login.

`docker build` - создаст image и сохранит его в local registry\
`docker push` - скопирует image из local registry в удалённый registry\
`docker pull` - загрузит image из удалённого registry\
`docker run` - запустит контейнер из образа

- Если нет образа локально - сначала подгрузит из удалённого registry

**Dockerfile**

Содержит поэтапные инструкции
- Основа будущего docker-образа
- Текстовый файл со списком инструкций для сборки образа
- Каждая инструкция создаёт промежуточный слой образа
- Инструкции кешируются в образах

Сборку делает демон Docker, а не Docker CLI

**Контекст сборки**\
**Контекст** - набор файлов расположенных по пути или URL. Процесс сборки может обращаться к файлам в контексте.\
**PATH-контекст** - путь до директории. Чаще всего `.`\
**URL-контекст** - git-репозитории, tarball-архивы.

**Docker image**\
Шаблон для создания контейнеров.\
Исполняемый пакет со всем необходимым для запуска.\
Состоит из слоёв.

**Docker container**

## Docker CLI
`docker help` - выдаст список команд.\
`docker build --help` описание команды\
`docker build` - сборка образа из dockerfile\
    `docker build .` - указываем "контекст сборки / build context"\
    `docker build -f Dockerfile` - указываем путь к Dockerfile\
`docker build -t my_project:v.1 .` - задаст тег

`build` кеширует каждый следующий созданный слой, что ускоряет сборку.

`docker images` - список локальных образов, alias для `docker image ls`

`docker tag <image_id> my_project:v.2`

`docker ps` - process list (список запущенных контейнеров)\
`docker ps -a` - process list, в т.ч. не запущенные

`docker run`\
`docker run --name service -d nginx`\
- `docker exec -it service bash`

- запускает контейнер из образа
- миллион флагов\
    `--help`\
    `--name`\
    `--detach`, `-d`\
    `--env` - set environment variables\
    `--env-file` - same as file\
    `--mount` - mount directories\
    `--volume`, `-v` - mount volumes\
    `--restart` (orchestration level)\
    `--rm`\
    `--memory bytes`, `-m` - limit resources\
    `--cpus` - limit resources\
    `-p 8080:80` - publish container `80` port to system `8080` port.\
`docker start / stop / restart`\
`docker attach`\
`docker exec`\
    `docker exec -it <container-id> /bin/bash` - интерактивно подключимся к контейнеру\
`docker logs`\
`docker pull` - загрузить image из registry\
`docker push` - загрузить image в registry

`docker rm -f $(docker ps -aq)`\
`docker rmi -f $(docker image ls -aq)`

`docker volumes`\
`docker networks`

Примеры:
```
# mount
docker run --read-only --mount type=volume,target=/icanwrite busybox touch /icanwrite/here

docker run -t -i --mount type=bind,src=/data,dst=/data busybox sh

docker run -d \
  -it \
  --name devtest \
  --mount type=bind,source="$(pwd)"/target,target=/app,readonly \
  nginx:latest

# tag
docker build . -t mynginx:latest -t mynginx:v0.2

# open terminal
docker exec -it <container-id> /bin/bash
# open terminal for alpine
docker exec -it <container-id> ash
```

## Docker Hub

```bash
docker manifest inspect <registry_url>/<image_name>:<tag>
```

**Типичная ошибка для новичка**
```
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠔⠋⢧⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠔⠊⠀⠀⠀⠸⡄⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡐⠀⢀⡤⠚⠁⠀⠀⠀⠀⠀⠀⢧⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠤⠐⠀⠀⠀⢐⣤⣤⣒⠒⠒⠒⠲⣤⣤⣶⣶⣶⡟⠡⠒⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠤⠞⠛⣶⡀⠀⠀⠀⠀⠀⠀⠈⢦⣒⠒⠢⠼⣿⣿⣿⣾⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⢀⣤⣾⣿⡇⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠶⠋⠁⠀⠀⠀⠀⠙⠂⠐⠒⢒⡒⢢⠀⠀⠀⠉⠒⡄⠈⠻⣿⣿⣿⡹⡟⠳⣄⣠⡴⢊⣥⣾⣿⠿⠛⠁⢹⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠰⠋⢀⠆⠀⠀⠀⠀⠀⠀⠀⣀⠀⠀⠈⠃⠳⣄⣀⠀⠀⣷⠀⠀⠘⠿⣿⣷⣜⢦⣳⡀⣀⣵⡿⠛⠁⠀⠀⠀⠈⡇
⠀⠀⠀⠀⠀⠀⠀⠀⣤⠐⠁⠀⠀⠀⠀⣀⠔⠊⠉⠀⠀⠀⠀⠀⠀⠀⠈⡟⢉⠝⠋⠉⠉⠑⣌⣿⣿⣿⣿⣿⣿⣁⠀⠀⠀⠀⠀⠀⠀⢱
⠀⠀⠀⠀⠀⠀⠀⢠⠃⠀⠀⣀⡤⣤⠞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⠚⡄⡜⣠⡇⠀⠀⠀⢹⣿⣿⣿⡉⠉⠉⠻⣷⡀⠀⠀⠀⠀⠀⠘
⠀⠀⠀⠀⠀⠀⢠⣧⠔⠊⠉⢀⠞⠁⣀⣴⣾⣿⡟⠛⢻⡗⢲⠤⣄⡀⢀⣇⠹⡉⠉⠒⠄⠀⢸⡇⠀⠙⠿⣆⠀⠀⠈⠁⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⢸⠳⡄⠀⠀⡞⣶⢾⣿⣿⡄⢻⣿⠀⠀⡇⠀⠀⠀⠈⠻⣄⠉⠋⠓⠒⠒⢒⡿⡇⠀⠀⠀⠀⠑⠢⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⢺⠀⠈⣁⣠⣿⣧⠀⢻⣿⣧⠈⣿⠀⣠⡃⠀⠀⠀⠀⠀⠈⠙⠒⠒⠒⠒⠉⢀⠇⠀⢠⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠸⡆⢸⣿⣎⢻⣿⣧⠀⢿⣿⠀⢹⢻⠃⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡜⢀⡴⢹⠀⣠⠤⠒⢒⠖⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢣⡀⢿⣿⣧⡙⣿⡆⠘⣿⡇⠚⢸⡖⣠⠀⠀⠀⠀⠀⠀⣰⠀⠀⢀⡄⢈⣿⠏⢀⠟⠉⠀⠀⠀⠁⠀⠀⠀⠀⠀⡼⠀
⠀⠀⠀⠀⠀⠀⠚⠉⠉⠙⢿⣿⣷⡸⣿⠀⠿⠃⢠⠻⡅⠁⠀⣀⢀⠄⢠⣴⠛⡤⣤⢋⠟⠁⠀⠀⠃⠀⠀⠀⢀⠀⠀⠀⡀⠀⠀⠚⠑⠂
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⡟⣿⠿⠟⢁⣀⣀⣨⣧⠹⠀⠚⢟⣫⣶⣿⣿⠀⣿⠇⠀⠀⠀⣠⣦⡴⠾⢛⣉⣡⠔⠊⠁⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣿⣿⣧⠤⠊⠀⠀⠀⠈⢙⣷⣶⣶⣿⣿⣿⡇⠀⠙⠀⠀⢀⠞⢋⠁⣠⣖⡿⠋⠀⠀⣀⣤⠄⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢀⣴⠚⠙⠧⣌⢻⣄⠀⠀⠀⣀⣴⣿⣿⣿⣿⣿⣿⠿⠇⠀⠀⠀⠠⠋⡰⢃⣼⡿⢋⣀⠤⢖⡾⠋⠁⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⢀⣤⡾⠋⢹⠀⠀⠀⠈⠳⣽⣿⣷⡿⠟⣿⣿⣿⣿⡿⠋⠀⢺⠀⠀⡄⠀⠀⣴⣽⣿⠏⠀⠀⠀⠐⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⢀⣠⡾⠛⠁⠀⠀⢸⣆⣀⣀⣀⠠⠔⠋⢉⣔⣻⣿⣿⡿⠉⠀⠀⠀⡌⢠⠞⠁⣠⣾⠿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⡾⠟⠁⠀⠀⠀⠀⠀⢸⠙⠒⠤⢤⣤⠄⠀⠀⢻⣿⣿⣿⠁⠀⠀⠀⢰⣷⣧⣶⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⡴
⠀⠀⠀⠀⢀⣠⠀⠀⢸⣄⡤⣖⡉⠀⠀⠀⢠⠞⢻⣿⣇⠀⠀⢀⣶⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡴⠟⠁⠀⠀
⠀⢀⡤⠒⠉⢸⠀⠀⢸⡇⠶⣖⡋⠁⠀⠀⠸⡀⣾⣿⣿⡆⠀⢸⣿⣿⣿⣿⡟⠀⠀⠀⠀⠀⢀⠔⠂⠀⠀⠀⠀⠀⣠⠾⠋⠀⠀⠀⠀⠀
⠒⣁⡴⠂⠀⢸⠀⠀⠘⡗⠀⠀⠙⢦⡀⠀⠀⢃⢹⣿⣿⡇⣀⣾⠿⠿⢿⣿⠁⠀⣀⣠⣤⡶⣷⠶⠶⠶⢤⣀⣀⣼⠋⠀⠀⠀⠀⠀⠀⠀
⠚⢉⣠⠀⠀⢸⠀⠀⠀⡇⣠⣤⣤⣴⣿⣦⡀⠈⢸⣿⣿⡴⠛⠁⠀⠀⠘⠛⠲⠛⠋⠉⢻⣿⠀⠀⠀⠀⠀⠀⠉⠀⠀⠀⠀⠀⠀⠀⢀⣠
```

Сломать зависимости из-за обновления образа.
Например используя образ `python.3` при обновлении с python.3.11 на python.3.12 всё сломается.

Нужно указывать версию более точно.

**Simple tags**
```
3.14.0a6-bookworm, 3.14-rc-bookworm⁠
3.14.0a6-slim-bookworm, 3.14-rc-slim-bookworm, 3.14.0a6-slim, 3.14-rc-slim⁠
3.14.0a6-bullseye, 3.14-rc-bullseye⁠
```

**Shared tags**
```
3.14.0a6-windowsservercore, 3.14-rc-windowsservercore:
    3.14.0a6-windowsservercore-ltsc2025⁠
    3.14.0a6-windowsservercore-ltsc2022⁠
    3.14.0a6-windowsservercore-1809⁠

3.13.2, 3.13, 3, latest:
    3.13.2-bookworm⁠
    3.13.2-windowsservercore-ltsc2025⁠
    3.13.2-windowsservercore-ltsc2022⁠
    3.13.2-windowsservercore-1809⁠
```

## Dockerfile
**FROM** - указывает базовый образ, на основе которого мы строим свою сборку.
Начинаем сборку с последнего уровня базового уровня.
`FROM <image>[:<tag>]`
Пример:
```Dockerfile
FROM ubuntu:22.04
FROM python:3
FROM nginx

# С указанием хранилища
FROM quay.io/vektorlab/ctop
FROM my_base_image:1.0
```

**LABEL** - задаёт метаданные для нашего образа:
```Dockerfile
LABEL <key>=<value> [<key>=<value> ...]
```

Пример:
```Dockerfile
LABEL maintainer="ivan-belkov2@yandex.ru" version="1.0.3"
```

**COPY** - копирует файлы из "контекста сборки" в образ:
```Dockerfile
COPY <src> [<src> ...] <dest>
# или
COPY ["<src>", ... "<dest>"]

# <src> - source
# <dest> - destination dir inside the container
```

Примеры:
```Dockerfile
COPY start* /startup/
COPY httpd.conf magicfile /etc/httpd/conf
```

**ADD (deprecated)** - копирует _любые_ файлы из интернета / другого места в образ.
Зачем нужна? (не надо, бан, небезопасно)

**ENV** - задаёт переменные окружения
```Dockerfile
ENV <key> <value>
```

Примеры:
```Dockerfile
ENV LOG_LEVEL debug
ENV DB_HOST 10.10.50.2:3389
```

**ARG** - задаёт переменные окружения при сборке образа, которые может получить из командной строки:
```Dockerfile
ARG <key>[=<default value>]

ARG VERSION=latest
FROM ubuntu:${version}
ARG BUILD
ENV BUILD=v1.0.0
RUN ECHO $BUILD
```

```bash
$ docker build \
    --build-arg VERSION=22.04 \
    --build-arg BUILD=v2.0.0 .
```

**WORKDIR** и **USER**
```Dockerfile
WORKDIR <path>

# <path> - путь внутри контейнера (автоматически создаётся)

USER <user>[:<group>]
# or
USER <UID>[:<GID>]

# <user> / <UID>
# <group> / <GID>
```

Пример:
```Dockerfile
WORKDIR /app

USER www-data
USER postgres
```

**RUN** - выполняет команду при сборке образа
```Dockerfile
RUN <command>
```

Пример:
```Dockerfile
RUN apt update && apt install python
RUN ["bash", "-c", "rm", "-rf", "/tmp/dir"]
RUN ["myscript.py", "arg1", "arg2"]
```

**CMD** - команда, которая выполняется при **старте контейнера**
```Dockerfile
CMD <command>
```

Примеры:
```Dockerfile
CMD /start.sh
CMD ["echo", "Hello World"]
CMD ["python3", "manage.py", "runserver"]
```

**ENTRYPOINT** - команда, которая выполняется при **старте контейнера**
```Dockerfile
ENTRYPOINT <command>
```

Примеры:
```Dockerfile
ENTRYPOINT exec top -b
ENTRYPOINT ["/usr/sbin/apache2ctl", "-D", "FOREGROUND"]
ENTRYPOINT ["/bin/sh", "/docker-entrypoint.sh"]
```

Как работают **ENTRYPOINT** и **CMD**
Обе инструкции имеют две формы:
- **exec**
- **shell**
При этом CMD имеет третью форму - в качестве параметров по умолчанию для ENTRYPOINT.

|                            | Без ENTRYPOINT             | ENTRYPOINT exec_entry p1_entry | ENTRYPOINT ["exec_entry", "p1_entry"]             |
| -------------------------- | -------------------------- | ------------------------------ | ------------------------------------------------- |
| Без CMD                    | не поддерживается          | /bin/sh -c exec_entry p1_entry | exec_entry p1_entry                               |
| CMD ["exec_cmd", "p1_cmd"] | exec_cmd p1_cmd            | /bin/sh -c exec_entry p1_entry | exec_entry p1_entry exec_cmd p1_cmd               |
| CMD exec_cmd p1_cmd        | /bin/sh -c exec_cmd p1_cmd | /bin/sh -c exec_entry p1_entry | exec_entry p1_entry<br>/bin/sh -c exec_cmd p1_cmd |
**VOLUME**
Инструкция **VOLUME** позволяет указать точки для монтирования томов внутри образа и создаёт том с содержимым директории на хостовой машине:
```Dockerfile
VOLUME <dest> [<dest> ...]
```

Примеры:
```Dockerfile
VOLUME /app /db /data
# or
VOLUME ["/var/www", "/var/log/apache", "/etc/apache2"]
```

**EXPOSE**
Инструкция **EXPOSE** информирует какие порты слушает сервис в запущенном контейнере.
Используется как средство коммуникации между тем, кто собирает образ и запускает контейнер. Не отвечает за реальное открытие портов.
```Dockerfile
EXPOSE <port>[/<proto>] [<port>[/<proto>] ...]

# <port> - порт сервиса внутри контейнера
# <proto> - tcp или upd, опционально
```

Примеры:
```Dockerfile
EXPOSE 5050
EXPOSE 8080/tcp 3389/udp
```

```sh
# will expose listed ports
docker run -P
```

**HEALTHCHECK**
Инструкция **HEALTHCHECK** говорит docker как проверять, что контейнер всё ещё работает.
Обычно HEALTHCHECK происходит на уровне docker compose или kubernetes.
```Dockerfile
HEALTHCHECK [OPTIONS] CMD command
# OPTIONS - параметры
# --interval=DURATION (default: 30s)
# --timeout=DURATION (default: 30s)
# --start-period=DURATION (default: 0s)
# --retries=N (default: 3)
#
# <command> - команда для проверки работоспособности
```

Примеры:
```Dockerfile
HEALTHCHECK --interval=5m --timeout=3s CMD curl -f http://localhost/ || exit 1
```

### Структура слоёв
|               |                                 |      |                                              |
| ------------- | ------------------------------- | ---- | -------------------------------------------- |
| Контейнер     | 513bda0c8feb<br>0 bytes         | CMD  | Итоговый слой, доступный для чтения и записи |
| Образ         | 06937bc9f3c4<br>188.1 Megabytes | COPY | Доступны только для чтения                   |
| Образ         | 423e1a71df5e<br>194 Kilobytes   | RUN  |                                              |
| Образ         | d361da3541c0<br>1.8 Kilobytes   | RUN  |                                              |
| Базовый образ | ubuntu:22.04                    | FROM | Базовый образ                                |
### OverlayFS
|                                            |        | Delete              | Overwrite       | Create |
| ------------------------------------------ | ------ | ------------------- | --------------- | ------ |
| Virtual<br>(Merged)                        | File 1 |                     | File 3 (edited) | File 4 |
| Read-write data partition<br>(Upper level) |        | ~~File 2~~ (delete) | File 3 (edit)   | File 4 |
| Read-only root fs<br>(Lower level)         | File 1 | File 2              | File 3          |        |
### Виды слоёв
Изменяют состояние файловой системы
- RUN
- COPY
- ADD
Изменяют метаданные
- ENV
- CMD/ENTRYPOINT
- USER
- WORKDIR
- EXPOSE

Максимум слоёв: 127
### Кэширование
- Важно для реализации быстрых сборок
- Если есть образ с тем же хешом - берём из кеша
- ADD, COPY - файлы по умолчанию кешируются (при изменении файлов будет пересобираться)
- Для остальных инструкций, включая RUN проверяется только изменения параметров
- `RUN apt-get -y update` не проверяет обновления постоянно, только в первый раз
- Пропустить и перестроить кэш можно командой
    `docker build --no-cache`

Сначала **COPY**, потом **RUN**
```Dockerfile
ENV LOG_LEVEL INFO                 # cache hit
COPY app /src/app                  # cache miss
RUN apt install -y nginx php-fpm   # cache miss
CMD ["/bin/app"]                   # cache miss
```

Сначала **RUN**, потом **COPY**
```Dockerfile
ENV LOG_LEVEL INFO                 # cache hit
RUN apt install -y nginx php-fpm   # cache hit
COPY app /src/app                  # cache miss
CMD ["/bin/app"]                   # cache miss
```

Меньшее количество слоёв:
```Dockerfile
# two images:
RUN apt update
RUN apt install -y nginx

# one image:
RUN apt update && \
    apt install -y nginx
```

Удаляйте неиспользуемые файлы до их записи в образ
```Dockerfile
# two images:
RUN apt update && \
    apt install -y nginx
RUN rm -rf /var/lib/apt/lists/*

# one image:
RUN apt update && \
    apt install -y nginx && \
    rm -rf /var/lib/apt/lists/*
```
Иначе эти файлы навсегда останутся в образе

###  Уменьшение размера образа

```Dockerfile
# this image size will be 1GB
FROM python:3

# this image size will be 50MB
FROM python:3-alpine
```
Имеем скомпилированное приложение без зависимостей:
```Dockerfile
FROM ubuntu:20.04
COPY ./hello-world .
EXPOSE 8080
CMD ["./hello-world"]
```

Проверим размер образа созданного из такого Dockerfile:

| REPOSITORY | TAG | SIZE  |
| ---------- | --- | ----- |
| hello-app  | 1.0 | 194MB |
| ubuntu     |     | 188MB |
Нужен ли нам образ ОС в данном случае?
```Dockerfile
FROM scratch
COPY ./hello-world .
EXPOSE 8080
CMD ["./hello-world"]
```

| REPOSITORY | TAG | SIZE   |
| ---------- | --- | ------ |
| hello-app  | 2.0 | 5.85MB |
| hello-app  | 1.0 | 194MB  |
| ubuntu     |     | 188MB  |
### Общие рекомендации
- Избегайте установки лишних пакетов и упаковки лишних данных в образы при сборке
- Используйте связанные команды для RUN-инструкций
- Следим за последовательностью описания Dockerfile и уменьшаем cache miss
- Уменьшаем число слоёв
- Один контейнер - одна задача
- Чистите за собой
- Используйте multi-stage сборки

### docker build --squash
В финале сборки собирает все слои в один.

### Multi-stage build
- Многоступенчатая сборка
```Dockerfile
FROM golang:1.16 AS builder
WORKDIR /code/
RUN go get -d -v golang.org/x/net/hmtl
COPY app.go ./
RUN CGO_ENABLED=0 go build -a -installsuffix cgo -o app .

FROM alpine:latest
RUN apk --no-cache add ca-certificates
WORKDIR /root/
COPY --from=builder /code/app ./
CMD ["./app"]
```

Скопировать файл из публичного образа:
```Dockerfile
COPY --from=nginx:latest /etc/nginx/nginx.conf /nginx.conf
```

Использовать предыдущий stage как новый:
```Dockerfile
FROM alpine:lastest AS builder
RUN apk --no-cache add build-base

FRIM builder AS build-stage1
RUN g++ -o /binary source.cpp
```

Управление через ARG
```Dockerfile
FROM alpine
ARG src=stage1
COPY --from=build-${src} . .
```

## Работа с файловой системой, Volumes
- Volumes (тома данных) - хранятся в части файловой системы хоста, не меняются не-Docker процессами.
- Bind mounts (монтирование каталогов с хоста) могут храниться где угодно на хосте.
- tmpfs mounts (временное хранилище) хранятся в памяти хоста и не сохраняются в файловую систему.
### Volumes
- Рекомендованный способ хранения данных.
- Доступ только через контейнер.
- Создаются и управляются средствами Docker.
- В контейнере том видно как каталог.
- Один том может быть примонтирован в несколько контейнеров (м.б. в ноль). Когда никто не использует том он продолжает существовать.
- Можно выбрать специальный драйвер и хранить данные на удалённом сервере или облаке.
```sh
docker volume create my-vol
docker volume inspect my-vol

docker run -d --name devtest -v my-vol:/app nginx:latest
```

## Bind mount
- Файл или каталог с хоста просто монтируется в контейнер. Используется чтобы пробросить конфигурационные файлы с хоста.
- Запись в примонтированный каталог могут вести программы как в контейнере, так и на хосте.
- Лучше указывать полный путь к файлу или каталогу на хосте.
- Если приложение в контейнере запущено от root, а каталог с ограниченными правами - может возникнуть проблема с правами на файлы.
```sh
docker run -d -it --name devtest -v "$(pwd)"/target:/app nginx:latest
docker inspect devtest
```

## tmpfs
Не сильно используюся, пояснения не будет .-.

## Работа с сетью, Network
### Docker Network Drivers
Существуют сети по умолчанию:
```sh
$ docker network ls
NETWORK ID     NAME                               DRIVER    SCOPE
bdde14602f26   bridge                             bridge    local
048530fe9414   host                               host      local
9ea314d6b8b1   none                               null      local
```
### Driver: None
- Создаётся свой network namespace
- Есть только loopback интерфейс
- Сеть контейнера полностью изолирована
### Driver: Host
- Удаляет сетевую изоляцию между контейнером и хостом, используя сеть напрямую
- Контейнер использует network namespace хоста
- Сеть не управляется самим Docker
- Два сервиса в разных контейнерах не могут слушать один и тот же порт
- Производительность сети контейнера равна производительности сети хоста
- `docker run --name blog --network host`
### Driver: Bridge
- Стандартный сетевой драйвер
- Используется для связи контейнеров на одном хосте
- Позволяет коммуницировать контейнерам в одном сетевом мосту
- Есть разница между **default** и **user-defined**

**Default Bridge Network**
- Назначается по умолчанию
- Нельзя вручную назначать IP-адреса
- Нет Service Discovery
- Можно использовать общие переменные окружения через --link

**User-Defined Bridge Network**
- Предоставляет автоматический DNS между контейнерами
- Контейнер мб подключён к нескольким Bridge сетям
- Работает Service Discovery
- Произвольные диапазоны IP-адресов
```sh
docker network create -d bridge --subnet 10.0.0.0/24 my_bridge
docker run --name c1 ubuntu
docker run --network my_bridge --name c2 ubuntu
docker run --network my_bridge --name c3 ubuntu
```

### Driver: Overlay
- Позволяет объединить в одну сеть контейнеры нескольких Docker-хостов
- Работает поверх VXLAN
- Необходимо хранить состояние распределённой сети

### Driver: Macvlan
Без подробных пояснений
- Более производительный, чем Bridge
- Легко исчерпать пул DHCP
- Много MAC адресов в L2 сегменте

## Docker-compose
Проблематика:
- Одно приложение состоит из нескольких контейнеров
- Один контейнер зависит от другого
- Порядок запуска имеет значение
- docker build/run/create - много сложных инструкций с опциями и нигде не хранится

Docker-compose:
- Декларативное описание Docker инфраструктуры в YAML-формате
- Управление многоконтейнерными приложениями
- docker-compose.yml - конфигурация проекта

`docker-compose.yml`
```yaml
version: "3"

services:
    mongo_db:
        image: mongo:3.2
        volumes:
            - db:/data/db
        networks:
            - reddit
    ui:
        build: ./ui
        image: ${USERNAME}/ui:1.0
        ports:
            - 9292:9292/tcp
        networks:
            - reddit
    volumes:
        db:
        sample_volume:
```

Работа с окружениями:
- Один docker-compose.yml на все окружения (под проект или приложение)
- Файл docker-compose.override.yml с:
    - Произвольные точки монтирования для быстрого изменения кода внутри контейнера
    - ENV переменными
    - Заменой command инструкций
    - Перезаписью выводимых портов

Healthcheck
```yaml
healthcheck:
    test: ["CMD", "curl", "-f", "http://localhost:5000/healthcheck"]
    interval: 1m30s
    timeout: 10s
    retries: 3
```

# Терминал
### Command prompt
```sh
username@hostname:path/to/dir$
```
Эта строка на английском называется `command prompt` ('обращение командной строки').

`username` - имя пользователя.\
`hostname` - имя машины, на которой запущен терминал.\
`path/to/dir` - путь к рабочей директории.

Что показывает ваш `command prompt` - зависит от конфигурации (например можно настроить чтобы prompt показывал текущую ветку git-репозитория).
```
# Возможные конфигурации
bash: bash-git-prompt
zsh: oh-my-zsh
powershell: posh-git
```
### Первые команды
`username@hostname:~$` - изначально находимся в домашней директории пользователя, она обозначается коротко через `~`.

Исполняем команды:\
`pwd` - path to working directory.
```sh
belkovanya@linux-desktop:~$ pwd
/home/belkovanya
```
`whoami` - имя текущего пользователя.
```sh
belkovanya@linux-desktop:~$ whoami
belkovanya
```
`mkdir` - make directory\
`cd` - change directory\
Создадим директорию (папку) и перейдём в неё.
```sh
~$ mkdir my_dir
~$ cd my_dir
~/my_dir$
```
Чтобы выйти из папки перейдём на один уровень выше.
```sh
~/my_dir$ cd ..
~$
```

### Путь: относительный и абсолютный
Так мы перейдём в корневую директорию:
```
cd /
```

Перейдём по абсолютному пути:
```
cd /var/log
```

Так мы поднимемся на два уровня выше:
```
cd ../..
```

Перейдём из текущей директории во вложенную:
```
cd dir/subdir/subsubdir
```

### Команды
#### Общий вид команды
```sh
command arg1 arg2 -option optionarg --option2
```

#### Примеры
```sh
# Создать несколько директорий (указываем несколько аргументов)
mkdir dir1 dir2 dir3

# Вывести список файлов в текущей директории
ls

# Опция -p команды mkdir позволяет создавать вложенные папки
mkdir -p dir1/dir2/dir3
```

#### Команды для работы с файлами
```sh
# create file
touch filename

# print file
cat filename

# copy file
cp filename filename2

# remove file
rm filename

# remove directory
rm -r dirname

# move (rename) file
mv oldname newname

stat filename

# open nano editor
nano filename

# open vim editor (vim is highly configurable)
vim filename
# quit vim: `q`, `esc, q`, `esc, q!`

# show file data with ability to search and scroll
less
	:\слово - поиск по слову (или строке)
	:n - поиск вперёд
	:shift+n - поиск назад

```

#### Команды, связанные с директорией
```sh
# print working directory
pwd

# print current user
whoami

# change directory
cd path/to/dir

# echo the text you pass to this command
echo 'Hello, World!'

# list files in working directory
ls
# list all files
ls -a
# list files with detailed information
ls -l
# use both options
ls -la

# print files hierarchy as a tree structure
tree
# dirs only
tree -d

# find files
find
```

#### Команды перенаправления ввода-вывода
```sh
# ">" write output to file
ls > output.txt

# ">>" add output of the command to the end of file
ls >> output.txt

# "|" (pipe) pass output of cmd1 to cmd2
cmd1 | cmd2
# examples
history | grep cd
ls -la | grep "^-"

# "|" (pipe) - pass output to other command
history | grep 'cd'

```

#### Команды *разные*
```sh
# count number of words in input
wordcount

# find pattern in input (works with regex)
grep

# print history
history

# use of environment variables
echo $USER

```

#### Команды для мониторинга
```sh
# list processes
ps

# list all processes
ps -e

# current
htop

# run command every x seconds and
watch results
watch

# check which process listen on port
netstat -tuln
```

#### Забавные команды (их необходимо устанавливать)
```sh
# cowsay - cow will say the text you pass to the command
cowsay hello
cowsay -f dragon hello

# steam locomotive)
sl

# this command will add color
lolcat
ls -la | lolcat
```

У команд есть различные опции и аргументы.\
Чтобы узнать про команду побольше есть разные способы:
### Справочная информация

Вывести подробную информацию:
```sh
# Открыть manual для команды
man cd

# Выйти из man: `q`
```

Вывести справочную информацию о команде:
```
cowsay -h
cowsay --help
```

Из `cowsay -h` узнаём про опции:
```
-b, -d, -g, -s, -t, -w, and -y activate Borg, dead, greedy, sleepy, tired, wired, and young appearance modes, respectively.

-e <eyes> defines a custom eye appearance. <eyes> should be a two-character string. It is up to you whether they actually look like eyes.
```

```
~$ cowsay -d 'I'm tired!'
 ____________
< I'm tired! >
 ------------
        \   ^__^
         \  (xx)\_______
            (__)\       )\/\
             U  ||----w |
                ||     ||

~$ cowsay -e -o 'Good luck!'
 ____________
< Good luck! >
 ------------
        \   ^__^
         \  (-o)\_______
            (__)\       )\/\
                ||----w |
                ||     ||
```

```
-f <cowfile> selects an alternate cow picture. `cowsay -l` will list the names of available cows.
```

Эту команду используйте самостоятельно
```
~$ cowsay  -f dragon 'Hello'
```

### pipe, "|"
Используйте pipe для передачи вывода одной команды на вход другой:
```sh
history | grep 'cd'
```


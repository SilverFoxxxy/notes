# Git
*Распределённая система контроля версий*

**Контроль версий:**\
Хранит версии файлов с возможностью вернуться к прошлой версии.

**Распределённая:**\
У разработчика версии хранятся локально (с возможностью отправлять сделанные изменения на сервер (удалённый репозиторий)).

**Безопасная:**\
В смысле повреждения файлов. Для каждого файла хранит check sum, что позволяет сразу заметить повреждение.

## Установка
```console
sudo apt install git
```

`ssh-keygen -t ed25519`

## Команды
`git do_smth` - общий вид команды

Информация о команде:\
`git command_name -h` - справочная информация о команде\
`git help command_name` - подробная информация

Чтобы в командном приветствии была видна информация о `git`:\
[git completion](https://github.com/git/git/blob/master/contrib/completion/git-prompt.sh)
```
bash: bash-git-prompt
zsh: oh-my-zsh
powershell: posh-git
```

`git init` - создать репозиторий. (Создаёт служебную директорию `.git`)\
Чтобы директория перестала быть репозиторием - удаляем `.git`
### config
`git config user.name "User Name"` - чтобы было видно кто сделал изменение.\
`git config user.email user@mail.com`

`cat .git/config` - проверяем `config`

Уровни конфигов:
```bash
# for system
--system
	/etc/gitconfig

# for user
--global
	~/.gitconfig

# for repository
--local (default)
	<project>/.git/config
```
`git` ищет данные снизу вверх.

Ставим `--global`.

`git config --list` - укажет всё: `local`, `global` и `system`.\
`git config --list --global`

Убрать локальные настройки:\
`git config --unset user.name`\
`git config --unset user.email`\
Чтобы убрать одной командой:\
`git config --remove-section user`

Проверяем:\
`git config --list`

Также можно вручную редактировать файл `.git/config`

Можно выбрать редактор сообщений (по умолчанию - vim).

`git config --global core.editor cmd-for-your-editor`
#### alias
`git config --global alias.c config`\
`git c --list`\
`git config alias.sayhi '!echo "hello"; echo "from git"'`\
`git sayhi`\
`git config --global alias.commitall '!git add .;git commit'`

`git config --global alias.graph 'log --oneline --decorate --graph --all'`

`git config --global alias.commitall '!git add A;git commit'`

Часто много alias-ов на уровне системы, например:\
[Набор alias-ов из ohmyzsh](https://github.com/ohmyzsh/ohmyzsh/blob/master/plugins/git/git.plugin.zsh)\
`alias g='git'`

### status
Показывает текущую ветку и список изменённых файлов.
### add
Добавить файл в отслеживаемые\
Добавляет файл в **Index**\
`git add path-to-file` - добавляет файл.\
`git add .` - добавляет все файлы текущей директории.\
`git add A` - добавляет все файлы репозитория.\
`git add dir` - добавит все файлы директории `dir`.\
`git add -p filename` - внутри файла выбираем изменения.

*see also: .gitignore*

`-force` - опция `git add` для добавления в **Index** игнорируемых файлов.\
`git add -force .vscode/config`\
`git add -f file`
### reset
`git reset HEAD file.txt` - удалит изменения из `index`.\
`git reset HEAD` - сбрасывает index!\
`git reset file.txt` - сбрасывает index для file.txt (удаляет из индекса)
### commit
Сохранить (закоммитить) внесённые изменения.\
(Редактор сообщения коммита можно настроить в config).\
`git commit` - коммит.\
`git commit -m 'My commit message'` - коммит без открытия редактора.\
`git commit -a` - добавление в индекс и коммит всех изменений (в уже добавленных файлах).\
`git commit --all` (то же что `-a`)\
`git commit <paths>` - коммитим только изменения в указанных файлах.

`git commit -v`

`git config --global commit.verbose true`

Пример сообщения:
```vim
Create welcome page

* Add feature A
* Fix feature B
# This comment won't be in commit message
# And this comment too!!
```
Первая строка - короткая, после неё - одна пустая строка. Она будет заголовком сообщения коммита.

После коммита видим вывод команды:
```sh
[main (root-commit) e719856] Your commit message title
1 file changed, 9 insertions(+)
create mode 100644 index.html
```
#### show
`git show --pretty=fuller` - последний коммит.\
`git show e123511` - коммит с таким хешом.

Видим Author, Commit\
Создавший изменения и закоммитивший может отличаться. Простой пример - opensource разработка.

Можно выставить конкретного автора\
`git commit --author='John Smith <john@me.com>' --date='...'`\
При желании (н-р через переменные окружения) можно выставить любого коммитера.\
(Н-р Дед Мороз в январе прошлого года).
### log
`git log` - история коммитов.\
`git log --oneline --decorate --graph --all` -  вывести граф изменений.\

Или расширение Git Graph в Visual Studio Code.

`git log`\
`git log --oneline` (от HEAD)\
`git log master`\
`git show`\
`git show master` / `git show HEAD` / `git show <commit>`\
`git show HEAD~`\
`git show HEAD~~`\
`git show HEAD~~ --quiet`\
`git show HEAD~3`\
`git show @~3` (@ - сокращение для HEAD, не работает в power shell)

Чтобы посмотреть предыдущее состояние файла, но не менять его:\
`git show @~:index.html`


### Права на файл
В сообщении коммита 100644 - тип файла и права на него.\
Либо 644, либо 755.

Изменения прав учитываются в репозитории (и в коммите).

Под windows - выставляется `git config core.fileMode false` , т.е. на права не смотрим (у всех 644).\
Чтобы сделать исполняемым (под windows):\
`git update-index --chmod=+x index.html`\
`git add --chmod=+x index.html`

### .gitignore
Список игнорируемых файлов (и директорий). Поддерживает регулярные выражения.
```.gitignore
*.pyc
*.ipynb_checkpoints*
other_repo
stands/*
!stands/example_stands/
datasets/
runs/
checkpoints/
```
#### global .gitignore
Стоит именно здесь учитывать специфичные для IDE файлы (они не имеют отношения к репозиторию).\
`git config --global core.excludesFile ~/.gitignore`
### Удаление и переименование
Удаление
1. Удалить файл и `git add filename` чтобы учесть удаление.
2. `git rm -r src` - то же одной командой.
3. `git rm -r --cached src` - удаляет только из индекса, не меняя рабочую директорию.
4. `git reset filename` - удалить из индекса
5. `git rm -f <files>` - удалить файлы с несохранёнными изменениями

Переименование
5. `mv a.txt b.txt`
6. `git add ..`
7. `git mv a.txt b.txt`

### remote
**Check Remote URL**:
`git remote -v`\
The URL should look like this: `git@gitlab.com:username/repository.git`.\
If it’s incorrect, update it using:\
`git remote set-url origin git@gitlab.com:username/repository.git`

## Процесс работы
**Working Directory** - рабочая директория. Здесь мы меняем файлы
**Index** - хранит список отслеживаемых файлов; промежуточное хранилище изменений. (`git add`  это про него).\
**Repository** - хранит полную историю изменений.\
**.gitignore** - список игнорируемых файлов (и директорий). Поддерживает регулярные выражения.
```.gitignore
*.pyc
*.ipynb_checkpoints*
other_repo
stands/*
!stands/example_stands/
datasets/
runs/
checkpoints/
```

`git status` - показывает статус Working Directory - внесённые изменения и изменения, учтённые в Index.\
**Index** хранит только изменения файлов, но не директории (т.е. пустую директорию закоммитить не получится). Можно добавить .gitkeep файл (или любой другой), чтобы директория сохранилась.

#### commit "без add"
- `git commit -a`
- `git commit <paths>`
- `alias.commitall '!git add .; git commit'`
	- `alias.commitall '!git add A; git commit'`

## Хороший коммит
*Commit early. Commit often.*
#### Atomic
Привносит одно логическое изменение. Исправляет баг X или добавляет фичу Y.

Такими коммитами легко управлять. (Коммиты можно отменять, применять снова, пересылать коллеге).

*Bad practice*: долгая разработка и один большой коммит.
#### Consistent
Коммит переводит систему из одного корректного состояния в другое.

#### Message
```
Merge branch '...'

Text text text ..
```

```
add -p: fix ..

Text text text ..
```
Или
`fix: ...`
`test: ...`
`refactor: ...`
`build: ...`

## Branches
Ветка - изолированный поток разработки.\
Изменения (коммиты) в одной ветке изолированы от изменений в других.

Создать новую ветку:\
`git branch new-branch-name`

Создать ветку от `main` ветки:\
`git checkout -b dev main` - создали ветку `dev`

`git switch dev` - переключились в ветку `dev`

`git checkout -b new-branch dev` - создали ветку `new-branch` от `dev`

`git branch` - list all branches

### Применение веток
#### Тематические ветки
Фичи - разработка экспериментальных фич независимо от проекта.\
Так мы можем продолжать разработку основного проекта не обращая внимания на ветки с фичами.\
Далее можем фичу влить в основную ветку либо безболезненно откинуть.
```
                feature -- c4 -- c5 -- c7
                /                       \
main -- c1 -- c2 -- c3 -------- c6 ------ main
                    \
                     another-feature -- c8 -- c9 -- X
```

#### Релизные ветки
Подготовку релиза выделим в отдельную ветку, а в main продолжим работу с проектом.
```
                v1.0 -- c4 -- c6
                /
main -- c1 -- c2 -- c3 -- c5
```

#### cherry-pick
Ошибка, допущенная в main-ветке, будет в обеих ветках.\
Исправление можно закоммитить в main-ветке, а затем применить сохранённые изменения в другой ветке через cherry-pick.
```
                v1.0 -- c4 -- c6 -- c7
                 /           :
                /        : {cherry-pick}
               /      :
main -- c1 -- c2 -- c3 -- c5
```
#### Релизные ветки - пример 2
```
                                v1.1
                                /
                v1.0 -- c4 -- c6 -- c9
                /
main -- c1 -- c2 -- c3 -- c5 --- c7 ------ c11
                            \
                            v2.0 -- c8 -- c10
```

#### Git Flow
Существуют разные подходы к управлению ветками разработки, почитать про один из них (Git Flow) можно здесь:\
[nvie.com/posts/...](https://nvie.com/posts/a-successful-git-branching-model/)\
**git flow вкратце**\
Ветки:
- `master` (main) at `origin` - production-ready state.
- `develop` - latest delivered development changes for the next release.
- `release-*` - only bug fixes
- `hotfix-*` - branch to distribute hotfixes
- `feature` - branch off from the `develop` branch
![](git-model@2x.png)

### Практика
```
git init
git add index.html
git commit -m 'Initial commit'
```

Создана ветка по умолчанию - main(master).

Список веток:
```
git branch

git branch -v
```

Технически ветка - ссылка на коммит.
```sh
cat .git/refs/heads/main
# Увидим идентификатор соответствующего коммита.
```

#### HEAD
```sh
cat .git/refs/HEAD
# Текущая ветка (в особом случае - текущий коммит)
```

При создании коммита:
```
# c3ef - родительский коммит
# 1913 - новый коммит

c3ef <- master <- HEAD


$ git commit =>
1913 <- master <- HEAD
  |
c3ef


$ git commit =>
54a4 <- master <- HEAD
  |
1913
  |
c3ef


$ git branch feature =>
 feature -> 54a4 <- master <- HEAD
              |
            1913
              |
            c3ef

$ git switch feature
# раньше писали так: git checkout feature
=>
 HEAD -> feature -> 54a4 <- master
                      |
                    1913
                      |
                    c3ef

$ echo 'f-change' > f-file.txt
$ git commit -am 'Create file'
=>
 HEAD -> feature -> 2702
                        \
                        54a4 <- master
                          |
                        1913
                          |
                        c3ef
```
`git checkout -b feature` - create and switch to new branch.

`git switch master`: (~~git checkout master~~)
- Вернёт `HEAD` на `master`
- Перенесёт в рабочую директорию версии файлов из ветки `master`.

#### checkout при незакоммиченных изменениях
```
$ git switch main  
error: Your local changes to the following files would be overwritten by checkout:

back-end/web-server/server.mjs
front-end/game-battle/src/http.js

Please commit your changes or stash them before you switch branches.

Aborting

# Перезаписать изменения
$ git switch --force main
$ git switch -f main

# Откатить все незакоммиченные изменения
$ git switch -f HEAD
$ git switch -f

# Сохранить изменения
$ git stash
$ git switch main
$ git switch feature
$ git stash pop
```

#### ! важно
`git stash` можно вызвать на одной ветке\
`git stash pop` - на другой.

Тогда мы применим изменения из одной ветки к файлам другой ветки.

`git switch main` успешно выполнится, если версии изменённых файлов у веток совпадают.\
Тогда мы перейдём в другую ветку, а изменения сохранятся => изменения перейдут в другую ветку!

Пример:\
Изменения можно не применять к текущей ветке, а вынести в отдельную.

#### Перенос веток
Понимаем что последние изменения должны быть в отдельной ветке, а `master` должен указывать на `54a4`.\
Одной командой:\
`git checkout -B master 54a4`

```
                    4594 <- master <- HEAD
                      |
                    2fad
feature ->  2c11      |
              |       |
            2702      |
                 \    |
                    54a4
                      |
                    1913
                      |
                    c3ef

$ git branch fix
                         <- fix
                    4594 <- master <- HEAD
                      |
                    2fad
feature ->  2c11      |
              |       |
            2702      |
                 \    |
                    54a4
                      |
                    1913
                      |
                    c3ef

$ git branch master 54a4
fatal: A branch named 'master' already exists.

$ git branch -f master 54a4
fatal: Cannot force update the current branch.

$ git switch fix

$ git branch -f master 54a4

                    4594 <- fix <- HEAD
                      |
                    2fad
feature ->  2c11      |
              |       |
            2702      |
                 \    |
                    54a4 <- master
                      |
                    1913
                      |
                    c3ef

$ git branch -f master fix
                         <- fix <- HEAD
                    4594 <- master
                      |
                    2fad
feature ->  2c11      |
              |       |
            2702      |
                 \    |
                    54a4
                      |
                    1913
                      |
                    c3ef

$ git checkout -B master 54a4

                           4594 <- fix <- HEAD
                            |
                           2fad
feature ->  2c11            |
              |             /
            2702           /
                 \        /
                    54a4 <- master
                      |
                    1913
                      |
                    c3ef
```


#### detached HEAD
**состояние отделённой HEAD**\
`git checkout 1913` -> HEAD переключится на соответствующий коммит.\
или `git switch --detached <commit>`\

Зачем:\
Посмотреть как выглядел проект на момент коммита.

Если сделали коммит в состоянии detached HEAD - можем применить его в нужной ветке с помощью cherry-pick.

```
$ git checkout 1913
                           4594 <- fix
                            |
                           2fad
feature ->  2c11            |
              |             /
            2702           /
                 \        /
                    54a4 <- master
                      |
                    1913 <- HEAD
                      |
                    c3ef

$ git commit
                           4594 <- fix
                            |
                           2fad
feature ->  2c11            |
              |             /       ... <- HEAD
            2702           /         |
                 \        /         9e28
                    54a4 <- master   |
                      |              /
                    1913  ----------/
                      |
                    c3ef
$ git switch fix
                           4594 <- fix <- HEAD
                            |
                           2fad
feature ->  2c11            |
              |             /       ...
            2702           /         |
                 \        /         9e28
                    54a4 <- master   |
                      |              /
                    1913  ----------/
                      |
                    c3ef

$ git cherry-pick 9e28
```

#### Прошлая версия файла
~~`git checkout`~~
`git restore`

```sh
$ git checkout 54a4 index.html ...
```

```sh
$ git checkout HEAD index.html
```
или (checkout из индекса)
```sh
$ git checkout file.txt
```

Неоднозначность парсинга, если имя файла == имя ветки
1. Сначала будет считаться что имеется ввиду ветка
2. Укажем -- чтобы обознать что нужен checkout файла
```
$ git checkout master
$ git checkout -- master
```

#### git merge

```
git checkout master
git merge fix
```

```
$ git switch master
                           4594 <- fix
                            |
                           2fad
feature ->  2c11            |
              |             /
            2702           /
                 \        /
                    54a4 <- master <- HEAD
                      |
                    1913
                      |
                    c3ef

$ git merge fix
Fast forward
                                <- master <- HEAD
                           4594 <- fix
                            |
                           2fad
feature ->  2c11            |
              |             /
            2702           /
                 \        /
                    54a4
                      |
                    1913
                      |
                    c3ef
$ cat .git/ORIG_HEAD
<commit>

$ git switch fix
$ git branch -f master ORIG_HEAD
                           4594 <- fix <- HEAD
                            |
                           2fad
feature ->  2c11            |
              |             /
            2702           /
                 \        /
                    54a4 <- master
                      |
                    1913
                      |
                    c3ef

$ git checkout -B master fix
                                <- master
                           4594 <- fix <- HEAD
                            |
                           2fad
feature ->  2c11            |
              |             /
            2702           /
                 \        /
                    54a4
                      |
                    1913
                      |
                    c3ef

```

#### Удаление ветки
`git branch -d fix` - удалит ветку только если она объединена с текущей.\
Представим что мы хотим удалить ветку feature\
`git branch -D feature` - удалит ветку.

Возможно восстановить ветку, точнее создать новую на существующем коммите. (Однако подвисшие коммиты будут со временем удалены git-ом, так что через долгое время может и не сработать)\
`git branch feature 2c11`

```
$ git checkout -B master fix
                                <- master
                           4594 <- fix <- HEAD
                            |
                           2fad
            2c11            |
              |             /
            2702           /
                 \        /
                    54a4
                      |
                    1913
                      |
                    c3ef

$ git branch feature 2c11
                                <- master
                           4594 <- fix <- HEAD
                            |
                           2fad
feature ->  2c11            |
              |             /
            2702           /
                 \        /
                    54a4
                      |
                    1913
                      |
                    c3ef
```
#### git reflog
Локальные логи изменения ссылок на коммиты.

`cat .git/logs/HEAD`

`git reflog master`

`git reflog` (reflog для HEAD)

`git help reflog`:
- Удаление log-ов
- Перезапись
`git log --oneline -g`

`git reflog --date=iso`

`git branch feature HEAD@{'2017-09-12 22:49:08 +0200'}`

`reflog` не бесконечен:
```
gc.reflogExpire="90 days ago"
gc.reflogExpireUnreachable="30 days ago"
```

`git checkout @{-1}` - смотрит в reflog и переходит на прошлую ветку.
`git checkout -`

### Восстановление удалённой ветки
`git reflog` - ищем соответствующий коммит (н-р последний перед `checkout: moving from feature to master`)\
`git branch feature HEAD@{6}` / под windows: `git branch feature 'HEAD@{6}'`

### git show, git graph - визуализация коммитов

Расширение для VS Code\
`git log`\
`git log --oneline` (от HEAD)\
`git log master`\
`git show`\
`git show master` / `git show HEAD` / `git show <commit>`\
`git show HEAD~`\
`git show HEAD~~`\
`git show HEAD~~ --quiet`\
`git show HEAD~3`\
`git show @~3` (@ - сокращение для HEAD, не работает в power shell)

`git log --oneline --decorate --graph --all` выведет граф коммитов.

```sh
➜  show-graph-repo git:(main) git log --oneline --decorate --graph --all

*   6fa9f2a (HEAD -> main) Merge branch 'feature'
|\
| * 2cb9295 (feature) Feature 1 added
* | d1ab896 Added new-file.txt
* | 63b9ac4 main v2
|/
* 1ffaa4b Initial commit
```

Чтобы посмотреть предыдущее состояние файла, но не менять его:\
`git show @~:index.html`\
`git show fix:index.html`

Посмотреть файл из индекса:\
`git show fix:index.html`

`git show :/sayBye` - поиск (как в less) по описаниям коммитов

### Восстановление изменённых файлов
`git checkout -f` - вернёт все файлы к состоянию `HEAD` и очистит Index.\
`git checkout index.html` - вернёт index.html к состоянию `HEAD`.

`git reset --hard`

#### Для файлов, не отслеживаемых git
`git clean`\
`git clean -d` удаление директорий\
`git clean -x` удаление всего что и в .gitignore\
`git clean -f`\
`git clean -dxf`

## Reset
Отмена нескольких коммитов.\
`git reset <commit>`\
`git reset @~` (на один коммит назад)

### reset --hard
`git reset --hard <commit>`\
Отменяет коммиты и приводит репозиторий к состоянию на момент `<commit>` (отменённые коммиты становятся 'подвисшими').

Чтобы отменить `git reset`:\
Найдём прошлое значени через `reflog`
```
$ git reflog master
<commit>
$ git reset --hard <commit>
```
Или через ORIG_HEAD (хранит прошлое значение HEAD перед reset).\
`git reset --hard ORIG_HEAD`

*Незакоммиченные изменения будут удалены!*

`git reset --hard HEAD`\
или\
`git reset --hard`\
=> удалим все незакоммиченные изменения.
### reset --soft
Позволяет изменить последний коммит!

`git reset --soft @~`\
Переместим ссылку, но сохраним изменения в индексе и в рабочей директории.

```
git reset --soft @~
git add script.js
git commit
```

Или чтобы взять сообщение из прошлого коммита:
```
git commit -C ORIG_HEAD
```
Это случай когда автор коммита и время коммита будет отличаться от коммитера!

`git show --quiet --pretty=fuller`

Чтобы поменять автора коммита на себя (из коммита скопируется только сообщение):
`git commit -C ORIG_HEAD --reset-author`

### Правка последнего коммита
`git commit --amend`\
`git commit --amend --reset-author`\
`git commit --amend --no-edit` (без вызова редактора)

```
# 1. Редактируем файлы как нужно
$ echo 'fix' >> file.txt

# 2. Добавляем в индекс
$ git add file.txt

# 3. git commit --amend
$ git commit --amend
# откроется копия сообщения о последнем коммите
```

По сути `git commit --amend` выполняет такие действия:
```
git reset --soft @~
git add script.js
git commit
```

То же для редактирования описания коммита!\
Или\
`git commit --amend -m ..`

### Правка последних коммитов
Через `soft reset` можем вернуться на два коммита назад и например объединить их в один.

### reset --mixed (default)
Передвигает ветку.\
Обнуляет индекс.\
Не трогает рабочую директорию.

|               | working<br>directory | index | current<br>branch |
| ------------- | -------------------- | ----- | ----------------- |
| reset --soft  |                      |       | reset             |
| reset --mixed |                      | reset | reset             |
| reset --hard  | reset                | reset | reset             |

`git reset HEAD` - сбрасывает index!\
`git reset file.txt` - сбрасывает index для file.txt (удаляет из индекса)

### Таблица с действиями reset
`git help reset` - таблица по действиям команды `reset`

## Просмотр
### git diff
| cmd                                  |                            |
| ------------------------------------ | -------------------------- |
| git diff                             | wd vs index                |
| git diff commit                      | git diff HEAD (wd vs HEAD) |
| git diff --cached                    | index vs HEAD              |
| git commit -v                        | index vs HEAD              |
| git diff commit1:path1 commit2:path2 |                            |
| git diff --no-index path1 path2      |                            |
`git diff <commit1> <commit2>`

`git diff -U3` - урезает контекст до трёх строк.

`git diff master feature`\
Связанные понятия:
- hunk
- hunk head

`git diff master..feature` - то же самое

`git diff master...feature` - что поменялось в feature с момента master

`git diff branch` - сравнение состояния рабочей директории с веткой branch\
`git diff HEAD` - сравнение состояния рабочей директории с HEAD. (По сути все последние изменения).

`git diff` - сравнивает рабочую директорию с index

`git diff --cached` / `git diff --staged`\
Увидим изменения, которые собираемся закоммитить. (Разница между индексом и HEAD).

`git commit -v` - выведет в сообщении о коммите после `--- >8 ---`  (ножницы) информацию о вносимых изменениях.\
Эта информация не войдёт в сообщение о коммите.

`git diff HEAD file.txt`\
`git diff master feature file.txt file2.txt`\
`git diff master feature --name-only`


Для коммитов:\
`git diff commit1 commit2`

`git diff commit1...commit2` - покажет изменения в commit2 с момента разделения.

### git log
#### git log
Выводит список коммитов
```sh
# Вывод log вместе с diff
git log --patch
git log -p

# Вывод по файлу
git log filename1.txt

# Вывод по файлу + переименования
git log filename1.txt --follow

# Вывод только ветки мастер, без влитых веток
git log master --first-parent

# Вывод разницы с момента расхождения
# (Диапазоны коммитов)
git log feature ^master
git log master..feature

git log master feature -- file1.txt file2.txt

git log ..feature
git log feature..

git log feature.. --boundary (показать разделяющий коммит)

git log master...feature (xor)
git log master...feature --boundary --graph

# Форматирование вывода
git log --pretty=medium (default)

git log --pretty=fuller

git log --pretty=one-line
git log --oneline

git log --pretty=one-line --decorate=short (default)

git log --oneline --no-decorate

git log --pretty=format:'%h %cd | %s%d [%an]'

git log --pretty=format:'%h %cd | %s%d [%an]' --date=short
git log --pretty=format:'%h %cd | %s%d [%an]' --date=format:'%F %R'

git log --pretty=format:'%h %cr | %s%d [%an]'

# добавим немного цвета
git log --pretty=format:'%C(yellow)%h %C(dim green)%ad %C(reset) | %C(cyan)%s%d %C(#667788)[%an]'

# check format %:
git help log
# check colors:
git help config

git config --global pretty.my format:'%C(yellow)%h %C(dim green)%ad %C(reset) | %C(cyan)%s%d %C(#667788)[%an]'
git log --pretty=my

# set as default
git config --global format.pretty my

# change default date format
git config --global log.date format

git log

# Вывод как обычно
git log --pretty=medium

```

git log использует strftime для форматирования дат.

#### graph
```
git log master

git log master feature

git log master feature --graph

git log --all --graph --oneline

git gui (to show list of gui for graph)
```

### Поиск
`git log --grep Run`

`git log --grep Run HEAD`

`git log --grep Run feature`

`git log --grep Run --grep sayHi` (хотя бы одно из)\
`git log --grep Run --grep sayHi --all-match` (все одновременно)

`git log --grep 'say(Hi|Bye)'`

`git log --grep 'say(Hi|Bye)' -P` (выставит Perl-формат регулярных выражений)\
`git config --gloabl grep.patternType perl`

`git log -F` (отключить парсинг по регулярным выражениям)

`git log -GsayHi` - в каких изменениях встречается 'sayHi'

`git log -G'function sayHi'` - в каких изменениях встречается 'sayHi'\
`git log -G'function sayHi\('` - в каких изменениях встречается 'sayHi'

Поиск изменений в строках с 3 по 6\
`git log -L 3,6:index.html`

Поиск изменений в `<head></head>`\
`git log -L '/<head>/','/<\/head>/':index.html`

`git log -L :sayHi:script.js` - поиск по функции (можно настроить регулярное выражение, дефолтно работает не всегда)

`git log -L '/^function sayHi/','/^}/':script.js`

`git log --author=Ilya`\
`git log --committer=Ilya`

`git log --before '3 months ago'`\
`git log --after '2017-09-13'`

### git blame - кто менял строку?
`git blame file.txt`\
`git blame file.txt --date=short -L 5,8`

## Merge
`git switch master`\
`git merge feature`

https://git-scm.com/docs/git-merge
https://git-scm.com/docs/merge-strategies

### fast forward merge
Слияние перемоткой - просто перемещаем master на feature (коммиты в feature становятся коммитами master), если коммитов в master не было.
```
feature ->  2c11
              |
            2702
                 \
                    54a4 <- master <- HEAD
                      |
                    1913
                      |
                    c3ef

$ git merge feature
Fast forward

feature ->  2c11 <- master <- HEAD
              |
            2702
              |
            54a4
              |
            1913
              |
            c3ef
```
Отмена:\
`git reset --hard ORIG_HEAD`

Чтобы такого не происходило - ставим флаг --no-ff
```
$ git merge --no-ff feature

                    341e <- master <- HEAD
                 /    |
feature ->  2c11      |
              |       |
            2702      |
                 \    |
                    54a4
                      |
                    1913
                      |
                    c3ef
```
Чтобы fast forward никогда не происходило:\
`git config merge.ff false`\
`git config --global branch.master.mergeoptions '--no-ff'`\
Чтобы вернуть возможность ff `git merge --ff feature`

### merge
На момент слияния веток считаем что working tree clean (git status выводит nothing to commit)

```
                   (ours)
                    4594 <- master <- HEAD
                      |
         (theirs)   2fad
feature ->  2c11      |
              |       |
            2702      |
                 \    |
                    54a4 (base)
                      |
                    1913
                      |
                    c3ef
# выведем base
$ git merge-base master feature
54a4..<commit>

$ git merge feature
```

### merge conflict
```
# Пусть попали в CONFLICT
# Тогда мы в состоянии прерванного слияния

$ cat .git/MERGE_HEAD

# Разрешим конфликт
```

```html
<!-- Файл index.html (если оказался конфликтным) -->
<html>
  <head>
<<<<<<< HEAD:index.html
    <link type="text/css" rel="stylesheet" media="all" href="style.css" />
=======
    <title>Hello World Page</title>
>>>>>>> feature:index.html
  </head>
  <body>
    <h1>Hello, World!</h1>
    <p>Let's learn Git together.</p>
  </body>
</html>

```

Если файл менялся только в одной ветке => сохранится изменённая версия.

Если файл менялся в обеих ветках, но изменение 1 было в начале файла, а 2 в конце файла - такое понятно как merge-ить.

Если изменения задели одни и те же строки - conflict (пусть думает разработчик).

`git checkout --ours file.txt`\
`git checkout --theirs file.txt`\
`git checkout --merge index.html`

`git merge feature --log=5` (default=20) - берём в сообщение последние 5 коммитов в feature

Отмена слияния:\
`git reset --hard`\
`git reset --merge`\
`git merge --abort`

Разрешение конфликта:\
`git merge feature`\
`git checkout --conflict=diff3 --merge index.html`
```html
<html>
  <head>
<<<<<<< ours
    <link type="text/css" rel="stylesheet" media="all" href="style.css" />
||||||| base
    Since this moment the real nightmare started here :)
=======
    <title>Hello World Page</title>
>>>>>>> theirs
  </head>
  <body>
    <h1>Hello, World!</h1>
    <p>Let's learn Git together.</p>
  </body>
</html>
```

Можно выставить, чтобы всегда показывало base-версию
```
git config --global merge.conflictStyle diff3
```

Исправляем файл как нам нужно.\
`git commit` - не сработает, т.к. индекс хранит информацию о сливаемых версиях.

`git show :1:index.html` - версия `base`\
`git show :2:index.html` - версия `ours`\
`git show :3:index.html` - версия `theirs`

`git add index.html` - исправили то, что происходит в индексе.

`git commit`\
или\
`git merge --continue`

Конец.

У результирующего коммита 2 родителя!
```
                    38e8 <- master <- HEAD
                   /  |
                  /   |
                 /    |
                /   4594 - old HEAD
 MERGE_HEAD    /      |
           \  /     2fad
feature ->  2c11      |
              |       |
            2702      |
                 \    |
                    54a4 (base)
                      |
                    1913
                      |
                    c3ef
```

Смотрим что получилось:

`git log --all --graph --oneline`

`git show` - показывает изменения, можно увидеть откуда они взялись (показывает конфликты)\
`git show --first-parent` - изменения от первого родителя\
`git show -m`

`git diff HEAD^` - разница с первым родителем\
`git diff HEAD^2` - разница со вторым родителем

`git diff HEAD^^` - разница с первым родителем первого родителя\
`git diff HEAD^2^` - разница с первым родителем второго родителя

! Возможно слияние более чем 2х веток!

`git branch --merged`\
`git branch --no-merged`

### Повторное слияние
```

 feature -> 8071
              |     38e8 <- master <- HEAD
              |    /  |
              |   /   |
              |  /    |
              | /   4594
              |/      |
              /     2fad
            2c11      |
              |       |
            2702      |
                 \    |
                    54a4 (base)
                      |
                    1913
                      |
                    c3ef

$ git merge feature
                    8c9a <- master <- HEAD
                   /  |
                /     |
 feature -> 8071      |
              |     38e8
              |    /  |
              |   /   |
              |  /    |
              | /   4594
              |/      |
              /     2fad
            2c11      |
              |       |
            2702      |
                 \    |
                    54a4 (base)
                      |
                    1913
                      |
                    c3ef
```
`git log master --oneline` - выведет вообще все эти коммиты!\
`git log master --first-parent`

### Отмена merge
Коммит существует только для ветки мастер.

`git reset --hard @~`

```
$ git reset --hard @~
                    8c9a
                   /  |
                /     |
 feature -> 8071      |
              |     38e8 <- master <- HEAD
              |    /  |
              |   /   |
              |  /    |
              | /   4594
              |/      |
              /     2fad
            2c11      |
              |       |
            2702      |
                 \    |
                    54a4 (base)
                      |
                    1913
                      |
                    c3ef

$ git reset --hard @~
                    8c9a
                   /  |
                /     |
 feature -> 8071      |
              |     38e8
              |    /  |
              |   /   |
              |  /    |
              | /   4594 <- master <- HEAD
              |/      |
              /     2fad
            2c11      |
              |       |
            2702      |
                 \    |
                    54a4 (base)
                      |
                    1913
                      |
                    c3ef
```

### Семантический конфликт
Допустим изменения произошли в разных частях файла, но объединять их нельзя.\
Например мы поменяли сигнатуру функции в одной ветке, а в другой - вызвали функцию в новом месте, но со старой сигнатурой.

Ветка main:
```diff
--- index.html
+++ index.html
@@ -1 +1 @@
- work()
+ work(5)

--- script.js
+++ script.js
@@ -1 +1 @@
- function work() {
+ function work(t) {
```

Ветка feature:
```diff
+++ index.html
@@ +1 @@
+ work()
```

`git merge feature --no-commit`\
Останавливаемся перед коммитом, делаем исправления.
```
- work()
+ work(1)
```

`git add index.html`

`git commit --no-edit`

### merge + squash commits
`git merge --squash fix` - добавляет изменения из ветки `fix` в `master`

`git diff --cached` (видим в `master` все изменения из ветки `fix`)

`git commit -m 'fix'`

Сжимает все изменения в один коммит и скрывает историю изменений.

`git branch -D fix` (удаляем ветку `fix`)

#### squash + conflict
`git merge --squash feature` - выдаёт конфликт.

`git merge --abort` - не сработает.

Для отмены используем:\
`git reset --hard`.

Для разрешения конфликта:
1. Вносим изменения в конфликтные файлы
2. git add file.txt
3. git commit
## cherry-pick
`git switch master`\
`git cherry-pick <commit>`\
`git cherry-pick -n` / `git cherry-pick --no-commit` - применить изменения без коммита.

Применит `diff <commit> <commit>~` к `master`

```

feature ->  D       G <- master
            |       |
            C       F
                \   |
                    B
                    |
                    A
$ git switch master
$ git cherry-pick D
                    D' <- master
                    |
feature ->  D       G
            |       |
            C       F
                \   |
                    B
                    |
                    A
```

cherry-pick используют например когда исправление ошибки нужно перенести в другие версии (ветки).\
Тогда один и тот же коммит нужно применить к разным веткам.

#### Как обойтись без `cherry-pick`:
Внести изменения в `base`, а затем смерджить ветки с изменённой `base`.

cherry-pick - очередной случай, когда автор коммита и коммитер могут отличаться.

#### Конфликт
`git cherry-pick abort`\
`git cherry-pick continue`

`git cherry-pick --quit` - остановиться на текущем изменении  (успешно скопированные коммиты останутся)

1. Вносим изменения
2. Вносим изменения в индекс
3. Коммит
#### Отмена
`git reset --hard @~3`

На сколько?
Почитаем reflog
`git reflog --no-decorate -3`

## rebase (use only in local branches)
Перебазироваться!\
То же в документации:\
https://git-scm.com/docs/git-rebase

### Никогда не делайте rebase
Точнее делайте.\
Но если ветка публичная - не делайте (у второго разработчика из-под ветки незаметно уедет фундамент).

`git rebase master feature`

`git switch feature`\
`git rebase master`
```
feature ->  E
            |
            D       G <- master
            |       |
            C       F
                \   |
                    B
                    |
                    A

$ git switch feature
$ git rebase master

feature ->  E
            |
            D
            |
            C
                \
                    G <- master
                    |
                    F
                    |
                    B
                    |
                    A

```

Как на самом деле работает (поэтапно):
```
                    G <- master
                    |
                    F
feature ->  E       |
            |       |
            D       |
            |       |
            C       |
                \   |
                    B
                    |
                    A
---
            E'
            |
            D'
            |
            C'
                \
                    G <- master
feature ->  E       |
            |       F
            D       |
            |       |
            C       |
                \   |
                    B
                    |
                    A
---
feature ->  E'
            |
            D'
            |
            C'
                \
                    G <- master
            E       |
            |       F
            D       |
            |       |
            C       |
                \   |
                    B
                    |
                    A
```

### Конфликт (oh rebase, here we go again)

При конфликте в rebase HEAD остаётся где-то на C' (detached HEAD)

`git rebase --abort` - всё отменяет, возвращает HEAD на место\
`git rebase --quit` - оставляет изменения, оставляет HEAD\
`git rebase --skip` - скип коммита

`rebase` пропускает пустые коммиты (изменения, которые уже были внесены в ветку)

1. Разрешаем конфликт
2. `git rebase --continue`
### Семантический конфликт
Например переименование функции в master.
### Как отменить
`git reset --hard ORIG_HEAD` - если всё ок\
`git reflog feature` - найдём последнее изменение, сделаем `git reset`
### Как избежать
`git switch feature`\
`git merge master`

**Тогда зачем `rebase`?**

Упрощаем историю изменений!

### rebase с тестами, rebase -x
Вызывать команду (н-р тесты) после каждого скопированного коммита. Удобно для проверки на потенциальные семантические конфликты и другие ошибки слияния.\
`git rebase -x ... master`

`git rebase -x 'python3 feature.py' master`

Будет при переносе каждого коммита запускать тесты (в данном случае команду `python3 feature.py`).

Когда встретили ошибку:
1. Исправляем ошибку в файле
    `vim feature.py`
2. Добавляем изменения в индекс
    `git add feature.py`
3. Заменяем последний коммит на исправленный
    `git commit --amend --no-edit`
4. Продолжаем `rebase`
    `git rebase --continue`

**Пример**

```
                            D <- master
                            | "Rename say() -> sayHi()"
HEAD -> feature -> C        |
      "Use say()"  |        |
                   |        |
                   B        |
                      \     |
                         \  |
                            A "Create say()"
```

`lib.py [A]`
```python
def say():
    print('Hello!')
```

`feature.py [C]`
```python
from lib import *

say()
```

`lib.py [D]`
```python
def sayHi():
    print('Hello!')
```

При `rebase`:
```sh
$ git switch feature
$ git rebase master
```

```
# lib.py
def sayHi():
    print('Hello!')

# feature.py
from lib import *

say()
```

```sh
$ python3 feature.py
Traceback (most recent call last):
  File "/.../feature.py", line 3, in <module>
    say()
NameError: name 'say' is not defined
```

```sh
git reset --hard ORIG_HEAD
```

```sh
git rebase -x 'python3 feature.py' master
```

```sh
git rebase -x 'python3 feature.py' main
Executing: python3 feature.py
Executing: python3 feature.py
Traceback (most recent call last):
  File "/Users/belkovanya/coding/mipt/tp/test-rebase-x-repo/feature.py", line 3, in <module>
    say()
NameError: name 'say' is not defined
warning: execution failed: python3 feature.py
You can fix the problem, and then run

  git rebase --continue

```

### rebase --onto (перенос части ветки)
`git rebase --onto master feature`

```
 HEAD -> fix -> F
                |
                |
                E
                  \             G <- master
                     \          |
             feature -> D       |
                        |       |
                        |       B
                        C       |
                          \     |
                             \  |
                                A

$ git rebase --onto master feature

         HEAD -> fix -> F'
                        |
                        |
                        E'
                          \
                             \
                                G <- master
                                |
             feature -> D       |
                        |       |
                        |       B
                        C       |
                          \     |
                             \  |
                                A

```

***Примечание***
Иногда проще использовать `cherry-pick`

### rebase --rebase-merges, перебазирование слияний

При обычном `rebase` будут пропущены коммиты слияния!\
Для учёта коммитов слияния:\
`git rebase --rebase-merges master`\
Для учёта разрешённых конфликтов слияния:\
см. `git rerere`

```
                       G <- master
                       |
     feature -> E      |
                |      F
                D      |
             /  |      |
          /     C      |
idea -> Y         \    |
        |            \ |
        |              B
        X              |
           \           |
               \       |
                   \   |
                       A


$ git rebase master
# Коммит слияния пропущен!!
     feature -> E'
                |
                C'
                |
                Y'
                |
                X'
                    \
                       G <- master
                       |
                E      |
                |      F
                D      |
             /  |      |
          /     C      |
idea -> Y         \    |
        |            \ |
        |              B
        X              |
           \           |
               \       |
                   \   |
                       A

# Вернём как было
$ git reset --hard feature@{1}
```

`git rebase --rebase-merges master`
```
                       G <- master
                       |
     feature -> E      |
                |      F
                D      |
             /  |      |
          /     C      |
idea -> Y         \    |
        |            \ |
        |              B
        X              |
           \           |
               \       |
                   \   |
                       A


$ git rebase --rebase-merges master
     feature -> E'
                |
                D'
               /|
              / C'
             |     \
             /         G <- master
            /          |
           /    E      |
          |     |      F
          /     D      |
         /   /  |      |
        / /     C      |
idea -> Y         \    |
        |            \ |
        |              B
        X              |
           \           |
               \       |
                   \   |
                       A

# Проблема: в D' повторяется конфликт
# В такой ситуации смотрим команду 'rerere'
```

### git rebase --interactive (н-р исправление истории коммитов)

`drop`: по умолчанию при удалении строки коммит учтён не будет.\
Чтобы это отключить:\
`git config rebase.missingCommitsCheck warn.error`

Чтобы при переносе на себя копировались все коммиты (без переиспользования):\
`git rebase --no-ff`

***Пример:***\
В нашей ветке неопрятные коммиты с непонятными комментариями и много коммитов `fix`.

```
$ git rebase -i master
pick c191f90c7 Initial commit
pick 3050fc0de Fix network bug
pick 7b1e3f2a2 Update README
pick 3e4f5d6a7 Commit sensitive data
pick 9a8b7c6d5 Add new feature
pick 6d5c4b3a2 Add new feature
pick 4b3c2d1a0 Update README
pick 5a6b7c8d9 Update README

# Rebase 795b95f..5a6b7c8d9 onto 795b95f (8 command(s))
#
# Commands:
# p, pick <commit> = use commit
# r, reword <commit> = use commit, but edit the commit message
# e, edit <commit> = use commit, but stop for amending
# s, squash <commit> = use commit, but meld into previous commit
# f, fixup <commit> = like "squash", but discard this commit's log message
# x, exec <command> = run command (the rest of the line) using shell
# d, drop <commit> = remove commit
#
# These lines can be re-ordered; they are executed from top to bottom.
#
# If you remove a line here THAT COMMIT WILL BE LOST.
#
# However, if you remove everything, the rebase will be aborted.
#
# Note that empty commits are commented out
```

В процессе `git rebase -i` (при паузе на изменяемом коммите) можем вызвать\
`git rebase --edit-todo` чтобы поменять план изменений.

Перед переносом в `master` хотим исправить историю коммитов.

```sh
git switch
git rebase -i
git reset @~
git add News.md
```

### git rebase --autosquash исправление коммита посередине ветки
```
# Добавляем fix к коммиту 3265
# Создаёт новый коммит с сообщением "fixup! <Commit 3265 message>"
$ git commit -a --fixup=3265

$ git rebase -i --autosquash @~4
# или git config --global rebase.autoSquash true

# Тогда при rebase --autosquash коммиты сольются в один
```

### Разработка без merge, или git rebase --interactive
Вместо того, чтобы делать merge - сделаем rebase коммитов из нашей ветки feature наверх ветки master.

В итоге мы исправим конфликты в тех же коммитах, а "альтернативная история" станет основной!
```
feature ->  E
            |
            D       G <- master
            |       |
            C       F
                \   |
                    B
                    |
                    A

...


feature ->  E'
            |
            D'
            |
            C'
            |
            G <- master
            |
            F
            |
            B
            |
            A

...


feature ->  E' <- master
            |
            D'
            |
            C'
            |
            G
            |
            F
            |
            B
            |
            A

```

## git revert
Новый коммит, отменяющий изменения прошлого коммита.

`git revert 1913`
`git revert A..D`

### git revert с коммитом слияния
`git revert 38e8`
```sh
$ git revert 38e8
error: commit 38e8 is a merge but no -m option was given

# Указываем с каким родителем учитывать diff
$ git revert 38e8 -m 1
```

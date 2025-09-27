# Стресс тестирование

Проблема:\
Мы написали код, он не работает (или какая-то его часть, н-р структура данных).

Основная идея:
1. Пишем простую реализацию (возможно менее эффективную)
2. Генерируем много случайных входных данных
3. Запускаем своё решение и медленное решение, сравниваем ответы

## Пример 1

Тестируем свою функцию `Sort` (MergeSort) с помощью запуска на случайных массивах и сравнением с результатом `SlowSort` (BubbleSort).
Данный код находит контрпример длиной 4 элемента - на нём будет легко найти, что именно ломается.

```cpp
void Test() {
    int n = 4;
    vector <int> a(n);

    for (int i = 0; i < n; i++) {
        a[i] = rand() % 10;
    }

    vector <int> b = Sort(a);
    vector <int> c = MergeSort(a);

    if (b != c) {
        // this is the breaking test!
        cout << "FOUND!\n";

        for (int i = 0; i < n; i++) {
            cout << a[i] << ' ';
        } cout << "\n";

        // to stop execution when the test is found
        exit(-1);
    }
}

int main() {
    int t = 1000;
    while (t--) {
        Test();
    }
}
```

## Пример 2
Более того, можно вставить проверку прямо в решение!

**Задача:**\
Дан массив длины $N$, и $Q$ запросов. Очередной запрос - проверить, есть ли число $x_i$ в массиве.

Напишем решение с помощью бинпоиска.

```cpp
#include <bits/stdc++.h>

void Solve() {
    int n;
    cin >> n;
    vector <int> a(n);

    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    int q;
    cin >> q;

    for (int i = 0; i < q; i++) {
        int x;
        cin >> x;

        int l = -1;
        int r = n - 1;
        while (l + 1 < r) {
            int m = (l + r) / 2;
            if (a[m] >= x) {
                r = m;
            } else {
                l = m;
            }
        }

        if (a[r] == x) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
}

int main() {
    int t = 1;

    while (t--) {
        Solve();
    }
}
```

Ловим WA!

Можно исправить код так:

> **Рекомендация**\
Если основное решение в файле *A.cpp*, создайте файл *A-stress.cpp*.

```cpp
#include <bits/stdc++.h>

void Solve() {
    int n;
    // cin >> n;
    n = 6; // <- set n
    vector <int> a(n);

    for (int i = 0; i < n; i++) {
        // cin >> a[i];
        a[i] = rand() % 10; // <- set random a[i]
    }

    int q;
    // cin >> q;
    q = 100; // set q

    for (int i = 0; i < q; i++) {
        int x;
        // cin >> x;

        x = rand() % 100 - 50; // x from [-50, 49)

        int l = -1;
        int r = n - 1;
        while (l + 1 < r) {
            int m = (l + r) / 2;
            if (a[m] >= x) {
                r = m;
            } else {
                l = m;
            }
        }

        bool ans = (a[r] == x);

        // slow check!
        bool ans2 = false;
        for (int i = 0; i < n; i++) {
            if (a[i] == x) {
                ans2 = true;
            }
        }

        if (ans != ans2) {
            cout << "FOUND!\n";

            for (int i = 0; i < n; i++) {
                cout << a[i] << ' ';
            } cout << " = a\n";
            cout << x << " = x\n";
            cout << ans << ' ' << ans2 << " = ans, ans2\n";

            exit(-1);
        }

        // if (a[r] == x) {
        //     cout << "YES\n";
        // } else {
        //     cout << "NO\n";
        // }
    }
}

int main() {
    int t = 1;
    t = 1000; // set t to find the breaking test

    while (t--) {
        Solve();
    }
}
```

Находим тест.

Разгадка - массив нам могли ввести неотсортированный!

Сортируем массив:
```cpp
sort(a.begin(), a.end());
```
Теперь стрессы ничего не найдут - код заработал как надо.


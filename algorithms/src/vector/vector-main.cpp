#include <iostream>

using namespace std;

int main() {
    size_t capacity = 2;
    size_t size = 0;

    int *a = new int[capacity];

    while (true) {
        string s;
        cin >> s;
        if (s == "exit") {
            return 0;
        }
        if (s == "push") {
            int x;
            cin >> x;

            int idx = size;

            if (idx < capacity) {
                a[idx] = x;
                size++;
                continue;
            }

            int *b = new int[2 * capacity];
            for (int i = 0; i < size; i++) {
                b[i] = a[i];
            }

            b[size] = x;
            size++;
            capacity *= 2;
            delete[] a;

            a = b;
        }

        if (s == "get") {
            int i;
            cin >> i;

            cout << a[i] << "\n";
        }

        if (s == "set") {
            int i, x;
            cin >> i >> x;
            a[i] = x;
        }
    }
}

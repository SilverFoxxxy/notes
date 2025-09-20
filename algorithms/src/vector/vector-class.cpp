#include <iostream>

using namespace std;

class Vector {
public:
    size_t capacity;
    size_t size;

    int *a = nullptr;

    Vector() {
        capacity = 2;
        size = 0;

        a = new int[capacity];
    }

    void DoubleMemory() {
        int *b = new int[2 * capacity];
        for (int i = 0; i < size; i++) {
            b[i] = a[i];
        }

        capacity *= 2;
        delete[] a;

        a = b;
    }

    void Push(int x) {
        int idx = size;

        if (idx >= capacity) {
            DoubleMemory();
        }

        a[idx] = x;
        size++;
    }

    int Get(int i) {
        return a[i];
    }

    void Set(int i, int x) {
        a[i] = x;
    }

    int& operator [](int i) {
        return a[i];
    }
};


int main() {
    Vector a;

    while (true) {
        string s;
        cin >> s;
        if (s == "exit") {
            return 0;
        }
        if (s == "push") {
            int x;
            cin >> x;

            a.Push(x);
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

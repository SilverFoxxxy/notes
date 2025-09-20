#include <iostream>

using namespace std;

struct Vector {
    size_t capacity;
    size_t size;

    int *a = nullptr;
};

void InitVector(Vector* v) {
    v->capacity = 2;
    v->size = 0;

    v->a = new int[v->capacity];
};

void InitVector(Vector& v) {
    v.capacity = 2;
    v.size = 0;

    v.a = new int[v.capacity];
}

void DoubleMemory(Vector& v) {
    int *b = new int[2 * v.capacity];
    for (int i = 0; i < v.size; i++) {
        b[i] = v.a[i];
    }

    v.capacity *= 2;
    delete[] v.a;

    v.a = b;
}

void Push(Vector& v, int x) {
    int idx = v.size;

    if (idx >= v.capacity) {
        DoubleMemory(v);
    }

    v.a[idx] = x;
    v.size++;
}

int Get(Vector& v, int i) {
    return v.a[i];
}

void Set(Vector& v, int i, int x) {
    v.a[i] = x;
}

int main() {
    Vector v;

    InitVector(v);

    while (true) {
        string s;
        cin >> s;
        if (s == "exit") {
            return 0;
        }
        if (s == "push") {
            int x;
            cin >> x;

            Push(v, x);
        }

        if (s == "get") {
            int i;
            cin >> i;

            cout << Get(v, i) << "\n";
        }

        if (s == "set") {
            int i, x;
            cin >> i >> x;

            Set(v, i, x);
        }
    }
}

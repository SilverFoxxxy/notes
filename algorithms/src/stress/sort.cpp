#include <bits/stdc++.h>

using namespace std;

void Merge(
    vector <int>& c,
    vector <int>& a,
    vector <int>& b
) {
    int i = 0;
    int j = 0;

    int n = a.size();
    int m = b.size();

    while (i < n && j < m) {
        if (a[i] < b[j]) {
            c[i + j] = a[i];
            i++;
        } else {
            c[i + j] = b[j];
            j++;
        }
    }
}

void Sort(
    vector <int>& a
) {
    int n = a.size();
    if (n <= 1) return;
    if (n == 2) {
        if (a[1] < a[0]) {
            swap(a[1], a[0]);
        }
        return;
    }

    vector <int> a1((n + 1) / 2);
    vector <int> a2(n / 2);

    for (int i = 0; i < (n + 1) / 2; i++) {
        a1[i] = a[i];
    }

    for (int i = 0; i < n / 2; i++) {
        a2[i] = a[i + (n + 1) / 2];
    }

    Merge(a, a1, a2);
}

void SlowSort(
    vector <int>& a
) {
    int n = a.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j + 1 < n; j++) {
            if (a[j + 1] < a[j]) {
                swap(a[j], a[j + 1]);
            }
        }
    }
}

void Solve() {
    // doing something
}

void Test() {
    int t = 1000;
    int n = 4;
    while (t--) {
        vector <int> a(n);
        for (int i = 0; i < n; i++) {
            a[i] = rand() % 10;
        }

        vector <int> b = a;
        vector <int> a0 = a;

        Sort(a);
        SlowSort(b);

        if (a != b) {
            // this is the breaking test!
            cout << "FOUND!\n";

            // print initial array
            for (int i = 0; i < n; i++) {
                cout << a0[i] << ' ';
            }
            cout << "\n";

            // our result
            for (int i = 0; i < n; i++) {
                cout << a[i] << ' ';
            }
            cout << "\n";

            // slow result
            for (int i = 0; i < n; i++) {
                cout << b[i] << ' ';
            }
            cout << "\n";

            // to stop execution when the test is found
            exit(0);
        }
    }
}

int main() {
    // Solve();
    Test();
}

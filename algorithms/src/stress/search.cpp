#include <bits/stdc++.h>

using namespace std;

void Solve() {
    int n;
    // cin >> n;
    n = 6; // <- set n
    vector <int> a(n);

    for (int i = 0; i < n; i++) {
        // cin >> a[i];
        a[i] = rand() % 10; // <- set random a[i]
    }

    sort(a.begin(), a.end());

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

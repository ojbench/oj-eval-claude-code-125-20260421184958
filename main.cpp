#include <bits/stdc++.h>
using namespace std;

struct FastScanner {
    static const int BUFSIZE = 1 << 20;
    int idx, size;
    char buf[BUFSIZE];
    FastScanner(): idx(0), size(0) {}
    inline char read() {
        if (idx >= size) {
            size = fread(buf, 1, BUFSIZE, stdin);
            idx = 0;
            if (size == 0) return EOF;
        }
        return buf[idx++];
    }
    template<typename T>
    bool nextInt(T &out) {
        char c; T sign = 1; T x = 0;
        c = read();
        if (c == EOF) return false;
        while (c != '-' && (c < '0' || c > '9')) {
            c = read();
            if (c == EOF) return false;
        }
        if (c == '-') { sign = -1; c = read(); }
        for (; c >= '0' && c <= '9'; c = read()) {
            x = x * 10 + (c - '0');
        }
        out = x * sign;
        return true;
    }
} scanner;

struct DSU {
    vector<int> p;
    vector<unsigned char> r; // rank fits in small range
    DSU() {}
    explicit DSU(int n): p(n), r(n, 0) { iota(p.begin(), p.end(), 0); }
    int find(int x) {
        int root = x;
        while (p[root] != root) root = p[root];
        while (x != root) { int pa = p[x]; p[x] = root; x = pa; }
        return root;
    }
    inline void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (r[a] < r[b]) swap(a, b);
        p[b] = a;
        if (r[a] == r[b]) r[a]++;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!scanner.nextInt(t)) return 0;
    string out;
    out.reserve(t * 4);
    for (int caseNo = 0; caseNo < t; ++caseNo) {
        int n; scanner.nextInt(n);
        vector<int> a(n), b(n);
        vector<unsigned char> eq(n);
        a.shrink_to_fit(); b.shrink_to_fit(); // ensure dense storage
        vector<int> ids;
        ids.reserve((size_t)2*n);
        for (int i = 0; i < n; ++i) {
            int ai, bi, ei;
            scanner.nextInt(ai);
            scanner.nextInt(bi);
            scanner.nextInt(ei);
            a[i] = ai; b[i] = bi; eq[i] = (unsigned char)ei;
            ids.push_back(ai);
            ids.push_back(bi);
        }
        sort(ids.begin(), ids.end());
        ids.erase(unique(ids.begin(), ids.end()), ids.end());
        DSU dsu((int)ids.size());
        // First process equalities
        for (int i = 0; i < n; ++i) {
            if (eq[i]) {
                int ia = (int)(lower_bound(ids.begin(), ids.end(), a[i]) - ids.begin());
                int ib = (int)(lower_bound(ids.begin(), ids.end(), b[i]) - ids.begin());
                dsu.unite(ia, ib);
            }
        }
        bool ok = true;
        // Then check inequalities
        for (int i = 0; i < n && ok; ++i) {
            if (!eq[i]) {
                int ia = (int)(lower_bound(ids.begin(), ids.end(), a[i]) - ids.begin());
                int ib = (int)(lower_bound(ids.begin(), ids.end(), b[i]) - ids.begin());
                if (dsu.find(ia) == dsu.find(ib)) ok = false;
            }
        }
        out += (ok ? "YES\n" : "NO\n");
    }
    cout << out;
    return 0;
}

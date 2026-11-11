// f(i, j) (0 <= j < i < n) must be totally monotone
// i-th get() returns {min f(i, *), leftmost argmin}.
// https://github.com/tatyam-prime/kyopro_library/blob/master/LARSCH.hpp
template<class F> struct LARSCH {
    using T = decay_t<invoke_result_t<F&, int, int>>;
    struct S { int n, i = 0, p = 0, b = 0, s = 0; vector<pair<int, int>> c; T mn{}; };
    F f;
    vector<S> a;
    LARSCH(int n, F f) : f(move(f)) {
        assert(n >= 0);
        for (--n; n > 0; n /= 4) a.push_back({n, 0, 0, 0, 0, vector<pair<int, int>>(a.empty() ? 0 : n)});
    }
    auto get() { int j = get_argmin(); return pair{move(a[0].mn), j}; }
    int get_argmin() { assert(!a.empty() && a[0].i < a[0].n); return next<1>(0); }
    void reset() { for (auto& x : a) x.i = x.p = x.b = x.s = 0; }
    template<bool rt> int next(int k) {
        auto& x = a[k];
        int i = x.i++, r = (i + 1) << (2 * k);
        auto col = [&](int j) { if constexpr (rt) return j; else return x.c[j].first; };
        if (i % 4 == 0) {
            x.b = i;
            if (x.n - i >= 4) {
                auto& y = a[k + 1];
                for (int j = max(0, i - 3); j <= i; ++j) {
                    int c = col(j);
                    while (y.s > i / 4) {
                        int row = y.s << (2 * k + 2);
                        if (!(f(row, c) < f(row, y.c[y.s - 1].first))) break;
                        --y.s;
                    }
                    if (y.s < y.n) y.c[y.s++] = {c, j};
                }
                x.b = y.c[next<0>(k + 1)].second;
            }
        }
        int p = i % 4 == 3 ? max(x.p, x.b) : x.p;
        int l = max(p + 1, (i & -4) + 1), ans = p;
        if constexpr (!rt) if (p >= x.b && l > i) return x.p = p;
        auto mn = f(r, col(p));
        auto check = [&](int j) {
            if (auto v = f(r, col(j)); v < mn) mn = move(v), ans = j;
        };
        for (int j = p + 1; j <= x.b; ++j) check(j);
        for (int j = l; j <= i; ++j) check(j);
        x.mn = move(mn);
        return x.p = ans;
    }
};

// f(i, j, k) := prefer A[i][j] to A[i][k]? (0 ≤ j < k < i < n)
// i-th get() returns j s.t. A[i][j] is best
// https://github.com/tatyam-prime/kyopro_library/blob/master/LARSCH.hpp
template<class F> struct LARSCH_cmp {
    struct S { int n, i = 0, p = 0, b = 0, s = 0; vector<pair<int, int>> c; };
    F f;
    vector<S> a;
    LARSCH_cmp(int n, F f) : f(move(f)) {
        assert(n >= 0);
        for (--n; n > 0; n /= 4) a.push_back({n, 0, 0, 0, 0, vector<pair<int, int>>(a.empty() ? 0 : n)});
    }
    int get() { assert(!a.empty() && a[0].i < a[0].n); return next<1>(0); }
    int get_argmin() { return get(); }
    void reset() { for (auto& x : a) x.i = x.p = x.b = x.s = 0; }
    template<bool rt> int next(int k) {
        auto& x = a[k];
        int i = x.i++, r = (i + 1) << (2 * k);
        auto col = [&](int j) { if constexpr (rt) return j; else return x.c[j].first; };
        if (i % 4 == 0) {
            x.b = i;
            if (x.n - i >= 4) {
                auto& y = a[k + 1];
                for (int j = max(0, i - 3); j <= i; ++j) {
                    int c = col(j);
                    while (y.s > i / 4 && f(y.s << (2 * k + 2), y.c[y.s - 1].first, c)) --y.s;
                    if (y.s < y.n) y.c[y.s++] = {c, j};
                }
                x.b = y.c[next<0>(k + 1)].second;
            }
        }
        int p = i % 4 == 3 ? max(x.p, x.b) : x.p, ans = p;
        for (int j = p + 1; j <= x.b; ++j) if (f(r, col(ans), col(j))) ans = j;
        for (int j = max(p + 1, (i & -4) + 1); j <= i; ++j) if (f(r, col(ans), col(j))) ans = j;
        return x.p = ans;
    }
};

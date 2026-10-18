#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <ranges>
#include <utility>
#include <vector>
using ll = long long;
using namespace std;

namespace bm_detail {
using Mat = vector<vector<ll>>;
// Regular s -> s^1 path, with consecutive node/arc mates. O(n^2 + m).
struct Path {
    struct Bud { vector<int> id, p; int e = -1; };
    int n, s;
    vector<Bud> bs;
    vector<vector<int>> g;
    vector<int> to, id, par, col, q;
    vector<char> vis;
    Path(int m, int src) : n(m / 2), s(src), bs(n), g(m), id(from_range, views::iota(0, n)), par(m, -1), col(m), vis(m) {}
    void add(int u, int v, ll c) {
        for (int k = 0; k < min(c, 2LL); ++k) { int e = ssize(to); to.append_range(array{v, u ^ 1}); g[u].push_back(e); g[v ^ 1].push_back(e ^ 1); }
    }
    int hd(int e) const { int v = to[e], k = id[v / 2]; return k < n ? v : 2 * k + (e != bs[k].e); }
    int tl(int e) const { return hd(e ^ 1) ^ 1; }
    void act(int v) { if (!exchange(vis[v], true)) q.append_range(g[v]); }
    static vector<int> rev(vector<int> p) { ranges::reverse(p); for (int& e : p) e ^= 1; return p; }
    vector<int> pref(int k, int v) const {
        if (hd(bs[k].e) == v) return {};
        auto p = bs[k].p;
        for (int i = 0; i < 2; ++i, p = rev(move(p))) {
            auto it = ranges::find(p, v, [&](int e) { return hd(e); });
            if (it != p.end()) return vector(p.begin(), it + 1);
        }
        assert(false); return {};
    }
    vector<int> lift(vector<int> p) {
        for (int k = ssize(bs) - 1; k >= n; --k) {
            id = bs[k].id; vector<int> r;
            for (auto [i, e] : p | views::enumerate) {
                if (i && hd(p[i - 1]) != tl(e)) r.append_range(p[i - 1] == bs[k].e ? pref(k, tl(e)) : rev(pref(k, hd(p[i - 1]) ^ 1)));
                r.push_back(e);
            }
            p.swap(r);
        }
        return p;
    }
    vector<int> find() {
        col[s] = 1; col[s ^ 1] = 2; act(s);
        auto walk = [&](int v) { vector<int> p; for (; v != s; v = tl(par[v])) p.push_back(par[v]); ranges::reverse(p); return p; };
        for (int at = 0; at < ssize(q); ++at) {
            int e = q[at], v = tl(e), w = hd(e);
            if ((v >= 2 * n && v == (w ^ 1)) || col[v] != 1 || col[w] == 1) continue;
            if (!col[w]) { col[w] = 1; col[w ^ 1] = 2; par[w] = e; act(w); continue; }
            auto p = walk(v), r = walk(w ^ 1); auto [i, j] = ranges::mismatch(p, r);
            int be = i == p.begin() ? -1 : i[-1];
            p.erase(p.begin(), i); r.erase(r.begin(), j); p.push_back(e); p.append_range(rev(move(r)));
            if (be < 0) return lift(move(p));
            int k = ssize(bs); bs.push_back({id, move(p), be});
            for (int a : bs[k].p) col[tl(a) & ~1] = col[hd(a) & ~1] = 3;
            par.append_range(array{be, -1}); col.append_range(array{1, 2});
            for (int z = 0; z < n; ++z) if (col[2 * id[z]] == 3) { id[z] = k; act(2 * z); act(2 * z + 1); }
        }
        return {};
    }
};
} // namespace bm_detail

// O(n^3) time / O(n^2) space. b,c >= 0, c symmetric, sum(b) <= LLONG_MAX.
// Returns {size, symmetric multiplicity matrix}. A loop consumes degree 2.
// https://github.com/tatyam-prime/kyopro_library/blob/master/GeneralBMatching.hpp
auto b_matching(vector<ll> b, vector<vector<ll>> c) {
    using namespace bm_detail;
    int n = ssize(b), s = 2 * n, t = s + 1, m = t + 1;
    Mat r(m, vector<ll>(m)), x(n, vector<ll>(n)); vector<ll> ex(m);
    for (int u = 0; u < n; ++u) for (int v = u; v < n; ++v) {
        c[u][v] = c[v][u] = min(c[u][v], u == v ? b[u] / 2 : min(b[u], b[v]));
        r[2 * u][2 * v + 1] = r[2 * v][2 * u + 1] = c[u][v] * (1 + (u == v));
    }
    for (int v = 0; v < n; ++v) r[2 * v][s] = r[2 * v + 1][t] = ex[2 * v] = b[v];
    // Dense relabel-to-front maximum flow.
    vector<int> h(m), it(m); h[s] = m; vector ord(from_range, views::iota(0, s));
    for (int k = 0; k < s; ++k) {
        int v = ord[k], hv = h[v];
        while (ex[v]) {
            if (it[v] == m) {
                int z = 2 * m; for (int w = 0; w < m; ++w) if (r[v][w]) z = min(z, h[w]); h[v] = z + 1; it[v] = 0;
            } else {
                int w = it[v];
                if (!r[v][w] || h[v] != h[w] + 1) { ++it[v]; continue; }
                ll d = min(ex[v], r[v][w]); r[v][w] -= d; r[w][v] += d; ex[v] -= d; ex[w] += d;
            }
        }
        if (h[v] > hv) { ranges::rotate(ord | views::take(k + 1), ord.begin() + k); k = 0; }
    }
    // Euler rounding of half-integral edges.
    vector f(n + 1, vector<char>(n + 1));
    for (int u = 0; u < n; ++u) for (int v = u; v < n; ++v) {
        ll z = r[2 * v + 1][2 * u] + (u == v ? 0 : r[2 * u + 1][2 * v]); x[u][v] = x[v][u] = z / 2;
        if (z % 2) { f[u][v] = f[v][u] = 1; f[u][n] ^= 1; f[v][n] ^= 1; }
    }
    for (int v = 0; v < n; ++v) f[n][v] = f[v][n];
    it.assign(n + 1, 0);
    for (int i = n; i >= 0; --i) {
        vector<int> st{i}; int pre = n, bit = 0;
        while (!st.empty()) {
            int v = st.back(), &w = it[v]; while (w <= n && !f[v][w]) ++w;
            if (w <= n) { f[v][w] = f[w][v] = 0; st.push_back(w); }
            else {
                if (pre == n || v == n) bit = 0;
                else { x[v][pre] += bit; x[pre][v] = x[v][pre]; bit ^= 1; }
                pre = v; st.pop_back();
            }
        }
    }
    ll ans = 0, ub = ex[t] / 2;
    for (int v = 0; v < n; ++v) { ll d = ranges::fold_left(x[v], x[v][v], plus{}); b[v] -= d; ans += d; }
    ans /= 2;
    while (ans < ub) {
        Path sp(m, s);
        for (int v = 0; v < n; ++v) sp.add(s, 2 * v, b[v]);
        for (int u = 0; u < n; ++u) for (int v = u; v < n; ++v) {
            sp.add(2 * u, 2 * v + 1, c[u][v] - x[u][v]); sp.add(2 * v + 1, 2 * u, x[u][v]);
        }
        auto p = sp.find(); if (p.empty()) break;
        for (int e : p) {
            int u = sp.to[e ^ 1] ^ 1, v = sp.to[e];
            if (u == s) --b[v / 2]; else if (v == t) --b[u / 2]; else x[u / 2][v / 2] = x[v / 2][u / 2] += u % 2 ? -1 : 1;
        }
        ++ans;
    }
    return pair{ans, move(x)};
}

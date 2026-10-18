#pragma once
#include <algorithm>
#include <cassert>
#include <climits>
#include <list>
#include <numeric>
#include <ranges>
#include <utility>
#include <vector>


// https://github.com/tatyam-prime/kyopro_library/blob/master/GeneralBMatching.hpp
namespace b_matching_detail {
using ll = long long;
using Matrix = std::vector<std::vector<ll>>;

// Regular s -> s^1 path. Node/arc mates have consecutive IDs.
// Bud trimming, deleting internal arcs: O(N^2 + M) time/space.
struct RegularPath {
    struct Arc { int u, v; };
    struct Bud { std::vector<int> members, children, path; int base = -1; };
    int n, s, stamp = 0;
    std::vector<Arc> arcs;
    std::vector<Bud> buds;
    std::vector<std::vector<int>> adj;
    std::vector<int> owner, parent, side, mark, queue;
    std::vector<char> active;

    RegularPath(int vertices, int source) : n(vertices / 2), s(source),
        buds(n), adj(vertices), owner(n), parent(vertices, -1),
        side(vertices), mark(vertices), active(vertices) {
        std::iota(owner.begin(), owner.end(), 0);
        for (int v = 0; v < n; ++v) buds[v].members.push_back(v);
    }
    void add(int u, int v, ll cap) {
        for (int k = 0; k < std::min<ll>(cap, 2); ++k) {
            int e = int(arcs.size());
            arcs.push_back({u, v}); arcs.push_back({v ^ 1, u ^ 1});
            adj[u].push_back(e); adj[v ^ 1].push_back(e ^ 1);
        }
    }
    int head(int e) const {
        int v = arcs[e].v, g = owner[v / 2];
        return g < n ? v : 2 * g + (e != buds[g].base);
    }
    int tail(int e) const { return head(e ^ 1) ^ 1; }
    void activate(int v) {
        if (!std::exchange(active[v], true))
            queue.insert(queue.end(), adj[v].begin(), adj[v].end());
    }
    static std::vector<int> mirror(std::vector<int> p) {
        std::ranges::reverse(p);
        for (int& e : p) e ^= 1;
        return p;
    }
    std::vector<int> prefix(int g, int target) const {
        if (head(buds[g].base) == target) return {};
        for (auto p : {buds[g].path, mirror(buds[g].path)}) {
            std::vector<int> out;
            for (int e : p) {
                out.push_back(e);
                if (head(e) == target) return out;
            }
        }
        assert(false); return {};
    }
    std::vector<int> expand(std::vector<int> p) {
        for (int g = int(buds.size()) - 1; g >= n; --g) {
            for (int child : buds[g].children)
                for (int v : buds[child].members) owner[v] = child;
            std::vector<int> out;
            for (int k = 0; k < int(p.size()); ++k) {
                int e = p[k];
                if (k && head(p[k - 1]) != tail(e)) {
                    auto bridge = p[k - 1] == buds[g].base
                        ? prefix(g, tail(e)) : mirror(prefix(g, head(p[k - 1]) ^ 1));
                    out.insert(out.end(), bridge.begin(), bridge.end());
                }
                out.push_back(e);
            }
            p.swap(out);
        }
        return p;
    }
    std::vector<int> find() {
        side[s] = 1; side[s ^ 1] = 2; activate(s);
        for (int qi = 0; qi < int(queue.size()); ++qi) {
            int e = queue[qi], a = owner[arcs[e].u / 2], b = owner[arcs[e].v / 2];
            if (a == b && a >= n) continue; // Internal arc of a trimmed bud.
            int v = tail(e), w = head(e);
            if (side[v] != 1 || side[w] == 1) continue;
            if (!side[w]) {
                side[w] = 1; side[w ^ 1] = 2; parent[w] = e;
                activate(w); continue;
            }
            ++stamp;
            for (int z = v;; z = tail(parent[z])) {
                mark[z] = stamp;
                if (z == s) break;
            }
            int lca = w ^ 1;
            while (mark[lca] != stamp) lca = tail(parent[lca]);
            std::vector<int> p, q, children{lca / 2};
            for (int z = v; z != lca; z = tail(parent[z])) {
                p.push_back(parent[z]); children.push_back(z / 2);
            }
            for (int z = w ^ 1; z != lca; z = tail(parent[z])) {
                q.push_back(parent[z] ^ 1); children.push_back(z / 2);
            }
            std::ranges::reverse(p); p.push_back(e); p.insert(p.end(), q.begin(), q.end());
            if (lca == s) return expand(std::move(p));
            Bud bud{{}, std::move(children), std::move(p), parent[lca]};
            for (int child : bud.children)
                bud.members.insert(bud.members.end(), buds[child].members.begin(), buds[child].members.end());
            int g = int(buds.size()); buds.push_back(std::move(bud));
            parent.push_back(buds[g].base); parent.push_back(-1);
            side.push_back(1); side.push_back(2); mark.resize(2 * buds.size());
            for (int z : buds[g].members) owner[z] = g;
            for (int z : buds[g].members) { activate(2 * z); activate(2 * z + 1); }
        }
        return {};
    }
};
} // namespace b_matching_detail

// Exact maximum cardinality b-matching: O(n^3) time, O(n^2) space.
// b >= 0, c >= 0 symmetric, sum(b) <= LLONG_MAX. A loop consumes degree 2.
// Returns {maximum size, symmetric matrix of edge multiplicities}.
inline auto b_matching(const std::vector<long long>& b,
                       std::vector<std::vector<long long>> c) {
    using namespace b_matching_detail;
    int n = int(b.size()), s = 2 * n, t = s + 1, N = t + 1;
    ll total = 0;
    assert(int(c.size()) == n);
    for (int u = 0; u < n; ++u) {
        assert(b[u] >= 0 && b[u] <= LLONG_MAX - total); total += b[u];
        assert(int(c[u].size()) == n);
    }
    for (int u = 0; u < n; ++u) for (int v = u; v < n; ++v) {
        assert(c[u][v] >= 0 && c[u][v] == c[v][u]);
        c[u][v] = c[v][u] = std::min(c[u][v], u == v ? b[u] / 2 : std::min(b[u], b[v]));
    }
    Matrix r(N, std::vector<ll>(N)), x(n, std::vector<ll>(n));
    for (int v = 0; v < n; ++v) r[s][2 * v] = r[2 * v + 1][t] = b[v];
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
        r[2 * u][2 * v + 1] = c[u][v] * (1 + (u == v));

    // Dense relabel-to-front maximum flow.
    std::vector<ll> excess(N);
    std::vector<int> height(N), next(N);
    height[s] = N;
    for (int v = 0; v < N; ++v) {
        ll d = std::exchange(r[s][v], 0);
        r[v][s] += d; excess[v] += d; excess[s] -= d;
    }
    std::list<int> order;
    for (int v = 0; v < s; ++v) order.push_back(v);
    for (auto it = order.begin(); it != order.end(); ++it) {
        int v = *it, old = height[v];
        while (excess[v]) {
            if (next[v] == N) {
                int h = 2 * N;
                for (int w = 0; w < N; ++w) if (r[v][w]) h = std::min(h, height[w]);
                height[v] = h + 1; next[v] = 0;
            } else {
                int w = next[v];
                if (!r[v][w] || height[v] != height[w] + 1) { ++next[v]; continue; }
                ll d = std::min(excess[v], r[v][w]);
                r[v][w] -= d; r[w][v] += d; excess[v] -= d; excess[w] += d;
            }
        }
        if (height[v] > old) { order.splice(order.begin(), order, it); it = order.begin(); }
    }
    ll upper = excess[t] / 2;

    // Round half-integral edges along Euler trails, starting with floor.
    std::vector<std::pair<int, int>> ends;
    std::vector<std::vector<int>> adj(n + 1);
    auto add = [&](int u, int v) {
        int e = int(ends.size()); ends.emplace_back(u, v);
        adj[u].push_back(e); adj[v].push_back(e);
    };
    for (int u = 0; u < n; ++u) for (int v = u; v < n; ++v) {
        ll twice = r[2 * v + 1][2 * u] + (u == v ? 0 : r[2 * u + 1][2 * v]);
        x[u][v] = x[v][u] = twice / 2;
        if (twice % 2) add(u, v);
    }
    for (int v = 0; v < n; ++v) if (adj[v].size() % 2) add(v, n);
    std::vector<bool> used(ends.size());
    next.assign(n + 1, 0);
    for (int start = n; start >= 0; --start) {
        std::vector<int> vs{start}, es{-1}, trail;
        while (!vs.empty()) {
            int v = vs.back();
            while (next[v] < int(adj[v].size()) && used[adj[v][next[v]]]) ++next[v];
            if (next[v] == int(adj[v].size())) {
                vs.pop_back(); if (es.back() != -1) trail.push_back(es.back()); es.pop_back();
            } else {
                int e = adj[v][next[v]++]; used[e] = true;
                auto [a, z] = ends[e]; vs.push_back(a ^ z ^ v); es.push_back(e);
            }
        }
        int parity = 0;
        for (int e : trail | std::views::reverse) {
            auto [u, v] = ends[e];
            if (u == n || v == n) { parity = 0; continue; }
            x[u][v] += parity; x[v][u] = x[u][v]; parity ^= 1;
        }
    }
    ll size = 0;
    for (;;) {
        std::vector<ll> degree(n);
        size = 0;
        for (int u = 0; u < n; ++u) for (int v = u; v < n; ++v) {
            assert(0 <= x[u][v] && x[u][v] <= c[u][v]);
            size += x[u][v]; degree[u] += x[u][v]; degree[v] += x[u][v];
        }
        for (int v = 0; v < n; ++v) assert(degree[v] <= b[v]);
        if (size == upper) break;
        RegularPath search(N, s);
        for (int v = 0; v < n; ++v) search.add(s, 2 * v, b[v] - degree[v]);
        for (int u = 0; u < n; ++u) for (int v = u; v < n; ++v) {
            search.add(2 * u, 2 * v + 1, c[u][v] - x[u][v]);
            search.add(2 * v + 1, 2 * u, x[u][v]);
        }
        auto path = search.find();
        if (path.empty()) break;
        for (int e : path) {
            auto [u, v] = search.arcs[e];
            if (u == s || v == t) continue;
            ll& z = x[u / 2][v / 2]; z += u % 2 ? -1 : 1;
            x[v / 2][u / 2] = z;
        }
    }
    return std::pair{size, std::move(x)};
}

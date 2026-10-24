// using ll = long long;


// Unit-capacity minimum-cost b-flow.
// b[v] = outflow - inflow; all b[v] = 0 gives minimum-cost circulation.
// Negative costs/cycles, loops, parallel edges and incremental edits allowed.
// Cost is a signed integer. ALL intermediate arithmetic must fit, including
// reduced costs, distance candidates, potentials and total-cost changes.
// Reachability is stored separately; there is no INF sentinel or max()/4 limit.
//
// add_edge(u,v,c) -> stable ID; set_cost(id,c); erase_edge(id);
// set_enabled(id,true) restores a deleted edge (capacity is always 0 or 1).
// set_supply(v,b); add_supply(v,delta); supply(v); get_edge(id); edge_count().
// For s-t flow F: set_supply(s,F), set_supply(t,-F), then solve().
// solve() returns the minimum TOTAL cost, or nullopt if infeasible.
// After edits or nullopt, flow conservation is not guaranteed until success.
//
// M = #IDs ever added, K = #unordered endpoint pairs ever added.
// Memory O(N+M+K); no lazy-deletion garbage. Disabled IDs/groups are retained.
// Edits O(log(M+1)); add_edge also O(log(K+1)).
// Each repair O((N+K)log(N+1) + N log(M+1)); no full-N scans per repair.
// From an optimum, q edge edits need <= q repairs (+ at most 1 failed search).
// Balanced supply changes add <= sum(abs(delta_b))/2 repairs.
// Cold start needs <= #negative_edges + sum(max(b[v],0)) repairs.
// https://github.com/tatyam-prime/kyopro_library/blob/master/HeapMCF.hpp
template<class Cost = ll>
class HeapMinCostFlow {
    static_assert(numeric_limits<Cost>::is_integer && numeric_limits<Cost>::is_signed);
    struct Node { Cost cost; int id; };
    struct Arc { Cost cost; int to, id; }; // Hot data for Dijkstra.
    struct Item { Cost cost; int arc, pos; bool flow; };
    int n;
    vector<Arc> a;
    vector<vector<Node>> heap;
    vector<Item> e;
    vector<vector<int>> g;
    map<pair<int, int>, int> index;
    vector<ll> b, ex;
    vector<Cost> p, d;
    vector<char> seen; // 0: unseen, 1: tentative, 2: settled.
    vector<int> prev, positive, pos, touched, settled, zero;
    vector<pair<Cost, int>> q;
    Cost total = 0;
    ll balance = 0;

    void top(int k) {
        a[k].id = heap[k].empty() ? -1 : heap[k][0].id;
        if (a[k].id >= 0) a[k].cost = heap[k][0].cost;
    }
    // Hole-based sift: write each moved entry/position only once.
    void fix(int k, int j) {
        auto& h = heap[k];
        auto x = h[j];
        if (j && x.cost < h[(j - 1) / 2].cost) {
            do {
                int u = (j - 1) / 2;
                h[j] = h[u]; e[h[j].id].pos = j;
                j = u;
            } while (j && x.cost < h[(j - 1) / 2].cost);
        } else {
            for (int u; (u = 2 * j + 1) < int(h.size()); j = u) {
                if (u + 1 < int(h.size()) && h[u + 1].cost < h[u].cost) ++u;
                if (x.cost <= h[u].cost) break;
                h[j] = h[u]; e[h[j].id].pos = j;
            }
        }
        h[j] = x; e[x.id].pos = j;
        top(k);
    }
    void insert(int i) {
        auto& x = e[i];
        int k = x.arc ^ int(x.flow);
        x.pos = int(heap[k].size());
        heap[k].push_back({x.flow ? -x.cost : x.cost, i});
        fix(k, x.pos);
    }
    void remove(int i) {
        int k = e[i].arc ^ int(e[i].flow), j = e[i].pos;
        auto& h = heap[k];
        auto x = h.back();
        h.pop_back();
        e[i].pos = -1;
        if (j == int(h.size())) { top(k); return; }
        h[j] = x;
        fix(k, j);
    }
    // O(1) maintenance of vertices with positive excess.
    void add_ex(int v, ll delta) {
        ex[v] += delta;
        if (ex[v] > 0 && pos[v] < 0) {
            pos[v] = int(positive.size());
            positive.push_back(v);
        } else if (ex[v] <= 0 && pos[v] >= 0) {
            int w = positive.back();
            positive[pos[v]] = w; pos[w] = pos[v];
            positive.pop_back(); pos[v] = -1;
        }
    }
    void update(int i, Cost cost, bool enabled) {
        auto& x = e[i];
        int u = a[x.arc ^ 1].to, v = a[x.arc].to;
        Cost c = cost + p[u] - p[v];
        bool f = enabled && (c < 0 || (c == 0 && x.flow));
        if (x.pos >= 0 && (!enabled || f != x.flow)) remove(i);
        total += (f ? cost : Cost(0)) - (x.flow ? x.cost : Cost(0));
        if (f != x.flow) {
            int delta = int(f) - int(x.flow);
            add_ex(u, -delta); add_ex(v, delta);
        }
        x.cost = cost; x.flow = f;
        if (!enabled) return;
        if (x.pos < 0) insert(i);
        else {
            int k = x.arc ^ int(f);
            heap[k][x.pos].cost = f ? -cost : cost;
            fix(k, x.pos); // Same direction: update in place.
        }
    }
    void push(Cost dist, int v) {
        q.emplace_back(dist, v);
        ranges::push_heap(q, greater{});
    }
    void clear_search() {
        for (int v : touched) seen[v] = false;
        touched.clear(); settled.clear(); zero.clear(); q.clear();
    }

public:
    struct Edge { int from, to; Cost cost; bool flow, enabled; };
    explicit HeapMinCostFlow(int n)
        : n(n), g(n), b(n), ex(n), p(n), d(n), seen(n), prev(n), pos(n, -1) {}

    // Optional allocation hint.
    void reserve(int edges, int pairs = 0) {
        e.reserve(edges); a.reserve(2 * pairs); heap.reserve(2 * pairs);
    }
    int add_edge(int u, int v, Cost cost) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        auto [it, fresh] = index.try_emplace(minmax(u, v), int(a.size()));
        int k = it->second;
        if (fresh) {
            int x = min(u, v), y = max(u, v);
            a.push_back({0, y, -1}); a.push_back({0, x, -1});
            heap.emplace_back(); heap.emplace_back();
            g[x].push_back(k); g[y].push_back(k ^ 1);
        }
        int i = int(e.size());
        e.push_back({cost, k ^ int(u > v), -1, false});
        update(i, cost, true);
        return i;
    }
    void set_cost(int i, Cost cost) {
        if (e[i].cost != cost) update(i, cost, e[i].pos >= 0);
    }
    void set_enabled(int i, bool enabled) {
        if ((e[i].pos >= 0) != enabled) update(i, e[i].cost, enabled);
    }
    void erase_edge(int i) { set_enabled(i, false); }
    int edge_count() const { return int(e.size()); }
    Edge get_edge(int i) const {
        const auto& x = e[i];
        return {a[x.arc ^ 1].to, a[x.arc].to, x.cost, x.flow, x.pos >= 0};
    }
    ll supply(int v) const { return b[v]; }
    void set_supply(int v, ll value) {
        ll delta = value - b[v];
        balance += delta; b[v] = value;
        add_ex(v, delta);
    }
    void add_supply(int v, ll delta) { set_supply(v, b[v] + delta); }

    optional<Cost> solve() {
        if (balance != 0) return nullopt;
        while (!positive.empty()) {
            int s = positive.back(), t = -1;
            Cost du = 0;
            d[s] = 0; seen[s] = true; touched.push_back(s); zero.push_back(s);
            while (!q.empty() || !zero.empty()) {
                int u;
                if (zero.empty()) {
                    ranges::pop_heap(q, greater{});
                    tie(du, u) = q.back(); q.pop_back();
                } else { u = zero.back(); zero.pop_back(); }
                if (du != d[u]) continue;
                seen[u] = 2;
                if (ex[u] < 0) { t = u; break; }
                settled.push_back(u);
                for (int k : g[u]) {
                    const auto& z = a[k];
                    if (z.id < 0 || seen[z.to] == 2) continue;
                    int v = z.to;
                    Cost c = z.cost + p[u] - p[v], nd = du + c;
                    assert(c >= 0);
                    if (seen[v] && nd >= d[v]) continue;
                    if (!seen[v]) { seen[v] = true; touched.push_back(v); }
                    d[v] = nd; prev[v] = k;
                    if (c == 0) zero.push_back(v); else push(nd, v);
                }
            }
            if (t < 0) { clear_search(); return nullopt; }
            // Unsettled vertices have distance >= d[t]; their price stays.
            for (int v : settled) p[v] += d[v] - d[t];
            total += p[t] - p[s];
            for (int v = t; v != s;) {
                int k = prev[v], i = a[k].id;
                v = a[k ^ 1].to;
                remove(i); e[i].flow = !e[i].flow; insert(i);
            }
            // Internal path vertices have no net excess change.
            add_ex(s, -1); add_ex(t, 1);
            clear_search();
        }
        return total;
    }
};

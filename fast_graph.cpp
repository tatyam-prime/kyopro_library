// compressed sparse row format

class fast_graph{
    using I = vector<int>::iterator;
    struct edges{
        I l, r;
        bool empty() const { return l == r; }
        size_t size() const { return r - l; }
        auto begin() const { return l; }
        auto end() const { return r; }
        int operator[](size_t i) const { return l[i]; }
    };
    
    vector<int> mem;
    vector<edges> g;
public:
    fast_graph() {}
    fast_graph(int n, vector<pair<int, int>> edge) : g(n), mem(edge.size() * 2) {
        for(auto [a, b] : edge){
            g[a].r++;
            g[b].r++;
        }
        I sum = mem.begin();
        for(int i = 0; i < n; i++){
            tie(g[i].l, g[i].r, sum) = tuple{sum, sum, sum + (g[i].r - g[i].l)};
        }
        for(auto [a, b] : edge){
            *g[a].r++ = b;
            *g[b].r++ = a;
        }
    }
    bool empty() const { return g.empty(); }
    size_t size() const { return g.size(); }
    auto begin() const { return g.begin(); }
    auto end() const { return g.end(); }
    const auto& operator[](size_t i) const { return g[i]; }
};

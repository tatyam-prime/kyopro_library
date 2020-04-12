
struct WeightedEdge{
    ll to, cost;
    WeightedEdge(){}
    WeightedEdge(ll to, ll cost): to(to), cost(cost){}
    operator ll() const { return to; }
};
struct WeightedGraph{
    using E = WeightedEdge;
    vector<vector<E>> g;
    WeightedGraph(){}
    WeightedGraph(ll n): g(n){}
    vector<E>& operator[](ll at){ return g[at]; }
    operator vector<vector<E>>&(){ return g; }
    auto begin() const { return g.cbegin(); }
    auto end() const { return g.cend(); }
    ll size() const { return g.size(); }
    const vector<E>& operator[](ll at) const { return g[at]; }
    operator const vector<vector<E>>&() const { return g; }
    void resize(ll a){ g.resize(a); }
    void add_edge(ll a, ll b, ll cost){
        g[a].emplace_back(b, cost);
        g[b].emplace_back(a, cost);
    }
    void add_directed_edge(ll from, ll to, ll cost){
        g[from].emplace_back(to, cost);
    }
    template<ll start_index = 1, bool directed = false> void input_graph(ll m){
        while(m--){
            ll a, b, c;
            scanf("%lld%lld%lld", &a, &b, &c);
            a -= start_index;
            b -= start_index;
            g[a].emplace_back(b, c);
            if(!directed) g[b].emplace_back(a, c);
        }
    }
    template<ll start_index = 1> void input_tree(ll n){ input_graph<start_index>(n - 1); }
};

struct UnWeightedEdge{
    ll to;
    static constexpr ll cost = 1;
    UnWeightedEdge(){}
    UnWeightedEdge(ll to): to(to){}
    operator ll() const { return to; }
};
struct UnWeightedGraph{
    using E = UnWeightedEdge;
    vector<vector<E>> g;
    UnWeightedGraph(){}
    UnWeightedGraph(ll n): g(n){}
    vector<E>& operator[](ll at){ return g[at]; }
    operator vector<vector<E>>&(){ return g; }
    auto begin() const { return g.cbegin(); }
    auto end() const { return g.cend(); }
    ll size() const { return g.size(); }
    const vector<E>& operator[](ll at) const { return g[at]; }
    operator const vector<vector<E>>&() const { return g; }
    void resize(ll a){ g.resize(a); }
    void add_edge(ll a, ll b){
        g[a].emplace_back(b);
        g[b].emplace_back(a);
    }
    void add_directed_edge(ll from, ll to){
        g[from].emplace_back(to);
    }
    template<ll start_index = 1, bool directed = false> void input_graph(ll m){
        while(m--){
            ll a, b;
            scanf("%lld%lld", &a, &b);
            a -= start_index;
            b -= start_index;
            g[a].emplace_back(b);
            if(!directed) g[b].emplace_back(a);
        }
    }
    template<ll start_index = 1> void input_tree(ll n){ input_graph<start_index>(n - 1); }
};

template<class Graph> vector<ll> Dijkstra(const Graph& g, ll start){
    vector<ll> cost(g.size(), LINF);
    vector<bool> used(g.size());
    pq<pll> q;
    cost[start] = 0;
    q.emplace(0, start);
    while(q.size()){
        ll at = q.top().second;
        q.pop();
        if(used[at]) continue;
        used[at] = 1;
        each(i, g[at]) if(chmin(cost[i], cost[at] + i.cost)) q.emplace(cost[i], i);
    }
    return cost;
}

vector<ll> BFS(const UnWeightedGraph& g, ll start){
    vector<ll> cost(g.size(), LINF);
    queue<ll> q;
    cost[start] = 0;
    q.push(start);
    while(q.size()){
        ll at = q.front();
        q.pop();
        each(i, g[at]) if(chmin(cost[i], cost[at] + i.cost)) q.push(i);
    }
    return cost;
}

template<class Graph> vector<vector<ll>> WarshallFloyd(const Graph& g){
    ll n = g.size();
    vector<vector<ll>> cost(n, vector<ll>(n, LINF));
    queue<ll> q;
    for(ll i = 0; i < n; i++) cost[i][i] = 0;
    for(ll i = 0; i < n; i++) for(auto& j : g[i]) cost[i][j] = j.cost;
    for(ll k = 0; k < n; k++) for(ll i = 0; i < n; i++) for(ll j = 0; j < n; j++) chmin(cost[i][j], cost[i][k] + cost[k][j]);
    return cost;
}

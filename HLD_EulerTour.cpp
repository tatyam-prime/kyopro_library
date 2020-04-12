
template<class Graph> struct HLDecomposition{
    ll n;
    const Graph& g;
    vector<ll> index, invex, light_root, heavy_root, siz;
    HLDecomposition(const Graph& g, ll root = 0): n(g.size()), g(g), index(n), invex(n), light_root(n), heavy_root(n), siz(n, 1){
        dfs(-1, root);
        n = 0;
        dfs2(-1, root, -1, 0);
        vector<ll> siz2(n);
        for(ll i = 0; i < n; i++) siz2[index[i]] = siz[i];
        swap(siz, siz2);
    }
    void dfs(ll from, ll at){
        for(auto& i : g[at]) if(i != from){
            dfs(at, i);
            siz[at] += siz[i];
        }
    }
    void dfs2(ll from, ll at, ll light, ll heavy){
        invex[n] = at;
        light_root[n] = light;
        heavy_root[n] = heavy;
        index[at] = n++;
        ll max = -1, maxdex = -1;
        for(auto& i : g[at]) if(i != from && chmax(max, siz[i])) maxdex = i;
        if(maxdex == -1) return;
        dfs2(at, maxdex, light, heavy);
        for(auto& i : g[at]) if(i != from && i != maxdex) dfs2(at, i, index[at], n);
    }
    ll lca(ll a, ll b){
        a = index[a]; b = index[b];
        while(true){
            if(a > b) swap(a, b);
            if(heavy_root[a] == heavy_root[b]) return invex[a];
            b = light_root[b];
        }
    }
    template<class T> void path_node(ll a, ll b, const T& f){
        a = index[a]; b = index[b];
        while(true){
            if(a > b) swap(a, b);
            if(heavy_root[a] == heavy_root[b]){
                f(a, b + 1);
                return;
            }
            f(heavy_root[b], b + 1);
            b = light_root[b];
        }
    }
    template<class T> void path_edge(ll a, ll b, const T& f){
        a = index[a]; b = index[b];
        while(true){
            if(a > b) swap(a, b);
            if(a == b) return;
            if(heavy_root[a] == heavy_root[b]){
                f(a, b);
                return;
            }
            f(heavy_root[b] - 1, b);
            b = light_root[b];
        }
    }
    template<class T> void subtree_node(ll a, const T& f){
        a = index[a];
        f(a, a + siz[a]);
    }
    template<class T> void subtree_edge(ll a, const T& f){
        a = index[a];
        f(a, a + siz[a] - 1);
    }
};



// https://www.hackerrank.com/contests/yfkpo3-1/challenges/bananas-multiplier-hard/problem

int main(){
    ll n;
    cin >> n;
    vector<array<ll, 3>> edge(n - 1);
    for(auto& i : edge) cin >> i[0] >> i[1] >> i[2];
    UnWeightedGraph g(n);
    for(auto& i : edge) g.add_edge(--i[0], --i[1]);
    HLDecomposition<UnWeightedGraph> hld(g);
    SegmentTree<Modint> seg(n);
    for(auto& i : edge) hld.path_edge(i[0], i[1], [&](ll l, ll r){ seg.set(l, i[2]); });
    ll q;
    cin >> q;
    for(ll i = 0; i < q; i++){
        ll a, b, c;
        cin >> a >> b >> c;
        a--; b--;
        Modint ans = c;
        hld.path_edge(a, b, [&](ll l, ll r){ ans *= seg.get(l, r); });
        cout << ans << '\n';
    }
}

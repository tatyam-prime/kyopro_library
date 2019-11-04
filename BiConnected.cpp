struct UnionFind{
    vector<ll>data;
    UnionFind(ll n):data(n,-1){}
    bool unite(ll a,ll b){
        a=root(a);b=root(b);
        if(a==b)return 1;
        if(data[a]>data[b])swap(a,b);
        data[a]+=data[b];
        data[b]=a;
        return 0;
    }
    bool find(ll a,ll b){return root(a)==root(b);}
    ll root(ll a){return data[a]<0?a:data[a]=root(data[a]);}
    ll size(ll a){return -data[root(a)];}
    ll operator[](ll a){return root(a);}
};
struct BiConnected{
    ll cnt = 0, n;
    const vector<vector<ll>>& _g;
    vector<vector<ll>> g;
    vector<ll> low, pre, id, siz;
    UnionFind uf;
    BiConnected(const vector<vector<ll>>& _g): n(_g.size()), _g(_g), low(n), pre(n, LINF), id(n), uf(n){
        dfs(-1, 0);
        cnt = 0;
        for(ll i = 0; i < n; i++) if(uf.data[i] < 0) id[i] = cnt++;
        siz.resize(cnt);
        for(ll i = 0; i < n; i++){
            id[i] = id[uf[i]];
            siz[id[i]]++;
        }
        g.resize(cnt);
        for(ll i = 0; i < n; i++) for(auto& j : _g[i]) if(id[i] != id[j]) g[id[i]].push_back(id[j]);
    }
    void dfs(ll from, ll at){
        pre[at] = low[at] = cnt++;
        for(auto& i : _g[at]) if(i != from){
            if(pre[i] == LINF) dfs(at, i);
            low[at] = min(low[at], low[i]);
            if(low[i] != pre[i]) uf.unite(at, i);
        }
    }
    ll size() const { return cnt; }
    ll size(ll x) const { return siz[x]; }
    vector<ll>& operator[](ll x){ return g[x]; }
    auto begin(){ return g.begin(); }
    auto end(){ return g.end(); }
};

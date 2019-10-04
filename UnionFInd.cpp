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
    //* extra
    ll size(ll a){return -data[root(a)];}
    ll operator[](ll a){return root(a);}
    //*/
};


struct PartiallyPersistentUnionFind{
    vector<ll> data, last;
    vector<vector<pll>> hst;
    PartiallyPersistentUnionFind(){}
    PartiallyPersistentUnionFind(ll sz): data(sz, -1), last(sz, LINF), hst(sz){
        for(auto &i : hst) i.emplace_back(-1, -1);
    }
    bool unite(ll t, ll x, ll y){
        x = root(t, x);
        y = root(t, y);
        if(x == y)return 1;
        if(data[x] > data[y]) swap(x, y);
        data[x] += data[y];
        hst[x].emplace_back(t, data[x]);
        data[y] = x;
        last[y] = t;
        return 0;
    }
    ll root(ll t, ll x) const {
        if(t < last[x]) return x;
        return root(t, data[x]);
    }
    ll size(ll t, ll x) const {
        x = root(t, x);
        return -(--lower_bound(hst[x].begin(), hst[x].end(), make_pair(t, 0)))->second;
    }
    // extra
    bool find(ll t, ll x, ll y) const {
        return root(t, x) == root(t, y);
    }
    ll operator()(ll t, ll x) const {
        return -(--lower_bound(hst[x].begin(), hst[x].end(), make_pair(t, 0)))->second;
    }
};

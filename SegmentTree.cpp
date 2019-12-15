using ll = long long; 


template<class T>
struct SegmentTree{
    virtual T f(const T&, const T&) const = 0;
    ll size = 1;
    vector<T> data;
    const T def_value;
    SegmentTree(ll n, const T& def_value): def_value(def_value){
        while(size < n) size *= 2;
        data.assign(size * 2, def_value);
    }
    SegmentTree(const vector<T>& v, const T& def_value): def_value(def_value){
        while(size < v.size()) size *= 2;
        data.assign(size * 2, def_value);
        for(ll i = 0; i < v.size(); i++) data[size + i] = v[i];
        // for(ll i = Base::size; --i;) Base::data[i] = f(Base::data[i * 2], Base::data[i * 2 + 1]);
    }
    T operator[](ll at) const {
        return data[size + at];
    }
    void update(ll at){
        while(at /= 2) data[at] = f(data[at * 2], data[at * 2 + 1]);
    }
    void set(ll at, const T& val){
        at += size;
        data[at] = val;
        update(at);
    }
    void add(ll at, const T& val){
        at += size;
        data[at] += val;
        update(at);
    }
    T get(ll l, ll r) const {
        T L = def_value, R = def_value;
        l += size; r += size;
        for(; l < r; l /= 2, r /= 2){
            if(l & 1) L = f(L, data[l++]);
            if(r & 1) R = f(data[--r], R);
        }
        return f(L, R);
    }
    void clear(){
        for(auto& i : data) i = def_value;
    }
};
template<class T>
struct RmQ : SegmentTree<T>{
    using Base = SegmentTree<T>;
    T f(const T& a, const T& b) const {return min(a, b);}
    RmQ(ll n, const T& def_value) : SegmentTree<T>(n, def_value){}
    RmQ(const vector<T>& v, const T& def_value) : SegmentTree<T>(v, def_value){
        for(ll i = Base::size; --i;) Base::data[i] = f(Base::data[i * 2], Base::data[i * 2 + 1]);
    }
};
template<class T>
struct RMQ : SegmentTree<T>{
    using Base = SegmentTree<T>;
    T f(const T& a, const T& b) const {return max(a, b);}
    RMQ(ll n, const T& def_value) : SegmentTree<T>(n, def_value, [](T a, T b){return max(a, b);}){}
    RMQ(const vector<T>& v, const T& def_value) : SegmentTree<T>(v, def_value, [](T a, T b){return max(a, b);}){
        for(ll i = Base::size; --i;) Base::data[i] = f(Base::data[i * 2], Base::data[i * 2 + 1]);
    }
};
template<class T>
struct RSQ : SegmentTree<T>{
    using Base = SegmentTree<T>;
    T f(const T& a, const T& b) const {return a + b;}
    RSQ(ll n, const T& def_value = T()) : SegmentTree<T>(n, def_value){}
    RSQ(const vector<T>& v, const T& def_value = T()) : SegmentTree<T>(v, def_value){
        for(ll i = Base::size; --i;) Base::data[i] = f(Base::data[i * 2], Base::data[i * 2 + 1]);
    }
};


// easy version

struct SegmentTree{
    ll size = 1;
    vector<ll> data;
    SegmentTree(ll n){
        while(size < n) size *= 2;
        data.assign(size * 2, -LINF);
    }
    void update(ll at){
        while(at /= 2) data[at] = max(data[at * 2], data[at * 2 + 1]);
    }
    void set(ll at, ll val){
        at += size;
        data[at] = val;
        update(at);
    }
    ll get(ll l, ll r){
        ll ans = -LINF;
        l += size; r += size;
        for(; l < r; l /= 2, r /= 2){
            if(l & 1) chmax(ans, data[l++]);
            if(r & 1) chmax(ans, data[--r]);
        }
        return ans;
    }
};

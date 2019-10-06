using ll = long long;


template<class T, class U>
struct LazySegmentTree{
    using F = function<T(const T&, const T&)>;
    using M = function<void(ll, T&, const U&)>;
    using C = function<void(U&, const U&)>;
    ll size = 1, rank = 0;
    vector<T> data;
    vector<U> lazy;
    const F f;
    const M m;
    const C c;
    const T def_value;
    const U def_lazy;
    LazySegmentTree(ll n, const T& def_value, const U& def_lazy, const F& f, const M& m, const C& c): f(f), m(m), c(c), def_value(def_value), def_lazy(def_lazy){
        while(size < n){
            size *= 2;
            rank++;
        }
        data.assign(size * 2, def_value);
        lazy.assign(size * 2, def_lazy);
    }
    LazySegmentTree(const vector<T>& v, const T& def_value, const U& def_lazy, const F& f, const M& m, const C& c): f(f), m(m), c(c), def_value(def_value), def_lazy(def_lazy){
        while(size < v.size()){
            size *= 2;
            rank++;
        }
        data.assign(size * 2, def_value);
        lazy.assign(size * 2, def_lazy);
        for(ll i = 0; i < v.size(); i++) data[size + i] = v[i];
        for(ll i = size; --i;) data[i] = f(data[i * 2], data[i * 2 + 1]);
    }
    void update_down(ll at){
        ll r = 31 - __builtin_clz(at);
        for(ll i = r; i > 0; i--){
            ll a = at >> i;
            if(lazy[a] != def_lazy){
                m(rank + i - r - 1, data[a * 2], lazy[a]);
                m(rank + i - r - 1, data[a * 2 + 1], lazy[a]);
                c(lazy[a * 2], lazy[a]);
                c(lazy[a * 2 + 1], lazy[a]);
                lazy[a] = def_lazy;
            }
        }
    }
    void update_up(ll at){
        while(at /= 2){
            data[at] = f(data[at * 2], data[at * 2 + 1]);
        }
    }
    T operator[](ll at){
        at += size;
        update_down(at);
        return data[at];
    }
    void set(ll at, const T& val){
        at += size;
        update_down(at);
        data[at] = val;
        update_up(at);
    }
    void range_query(ll l, ll r, const U& val){
        if(l >= r) return;
        l += size;
        r += size;
        ll L = 0, R = 0;
        [&L, &R, l, r]() mutable {
            for(; l < r; l /= 2, r /= 2){
                if(l & 1){
                    if(!L) L = l;
                    l++;
                }
                if(r & 1 && !R) R = r - 1;
            }
        }();
        update_down(L);
        update_down(R);
        for(ll rank = 0; l < r; l /= 2, r /= 2, rank++){
            if(l & 1){
                m(rank, data[l], val);
                c(lazy[l++], val);
            }
            if(r & 1){
                m(rank, data[--r], val);
                c(lazy[r], val);
            }
        }
        update_up(L);
        update_up(R);
    }
    T get(ll l, ll r){
        if(l >= r) return def_value;
        T L = def_value, R = def_value;
        l += size; r += size;
        update_down(l);
        update_down(r - 1);
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
struct RAQRSQ : LazySegmentTree<T, T>{
    RAQRSQ(ll n, const T& def_value = T()) : LazySegmentTree<T, T>(n, def_value, def_value, [](T a, T b){return a + b;}, [](ll rank, T& x, const T& val){x += val << rank;}, [](T& a, const T& b){a += b;}){}
    RAQRSQ(const vector<T>& v, const T& def_value = T()) : LazySegmentTree<T, T>(v, def_value, def_value, [](T a, T b){return a + b;}, [](ll rank, T& x, const T& val){x += val << rank;}, [](T& a, const T& b){a += b;}){}
};
template<class T>
struct RUQRmQ : LazySegmentTree<T, T>{
    RUQRmQ(ll n, const T& def_value, const T& def_lazy = numeric_limits<T>::max()) : LazySegmentTree<T, T>(n, def_value, def_lazy, [](T a, T b){return min(a, b);}, [](ll rank, T& x, const T& val){x = val;}, [](T& a, const T& b){a = b;}){}
    RUQRmQ(const vector<T>& v, const T& def_value, const T& def_lazy = numeric_limits<T>::max()) : LazySegmentTree<T, T>(v, def_lazy, def_value, [](T a, T b){return min(a, b);}, [](ll rank, T& x, const T& val){x = val;}, [](T& a, const T& b){a = b;}){}
};
template<class T>
struct RAQRmQ : LazySegmentTree<T, T>{
    RAQRmQ(ll n, const T& def_value, const T& def_lazy = T()) : LazySegmentTree<T, T>(n, def_value, def_lazy, [](T a, T b){return min(a, b);}, [](ll rank, T& x, const T& val){x += val;}, [](T& a, const T& b){a += b;}){}
    RAQRmQ(const vector<T>& v, const T& def_value, const T& def_lazy = T()) : LazySegmentTree<T, T>(v, def_value, def_lazy, [](T a, T b){return min(a, b);}, [](ll rank, T& x, const T& val){x += val;}, [](T& a, const T& b){a += b;}){}
};
template<class T>
struct RUQRSQ : LazySegmentTree<T, T>{
    RUQRSQ(ll n, const T& def_value = T(), const T& def_lazy = numeric_limits<T>::max()) : LazySegmentTree<T, T>(n, def_value, def_lazy, [](T a, T b){return a + b;}, [](ll rank, T& x, const T& val){x = val << rank;}, [](T& a, const T& b){a = b;}){}
    RUQRSQ(const vector<T>& v, const T& def_value = T(), const T& def_lazy = numeric_limits<T>::max()) : LazySegmentTree<T, T>(v, def_value, def_lazy, [](T a, T b){return a + b;}, [](ll rank, T& x, const T& val){x = val << rank;}, [](T& a, const T& b){a = b;}){}
};

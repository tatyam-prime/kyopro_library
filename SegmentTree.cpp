using ll = long long; 


template<class T>
struct SegmentTree{
    using F = function<T(T, T)>;
    ll size = 1;
    vector<T> data;
    const F f;
    const T def_value;
    SegmentTree(ll n, const T& def_value, const F& f): f(f), def_value(def_value){
        while(size < n) size *= 2;
        data.assign(size * 2, def_value);
    }
    SegmentTree(const vector<T>& v, const T& def_value, const F& f): f(f), def_value(def_value){
        while(size < data.size()) size *= 2;
        data.assign(size * 2, def_value);
        for(ll i = 0; i < v.size(); i++) data[size + i] = v[i];
        for(ll i = size; --i;) data[i] = f(data[i * 2], data[i * 2 + 1]);
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
struct SegmentTree_Min : SegmentTree<T>{
    SegmentTree_Min(ll n, const T& def_value) : SegmentTree<T>(n, def_value, [](T a, T b){return min(a, b);}){}
    SegmentTree_Min(const vector<T>& v, const T& def_value) : SegmentTree<T>(v, def_value, [](T a, T b){return min(a, b);}){}
};
template<class T>
struct SegmentTree_Max : SegmentTree<T>{
    SegmentTree_Max(ll n, const T& def_value) : SegmentTree<T>(n, def_value, [](T a, T b){return max(a, b);}){}
    SegmentTree_Max(const vector<T>& v, const T& def_value) : SegmentTree<T>(v, def_value, [](T a, T b){return max(a, b);}){}
};
template<class T>
struct SegmentTree_Sum : SegmentTree<T>{
    SegmentTree_Sum(ll n, const T& def_value = T()) : SegmentTree<T>(n, def_value, [](T a, T b){return a + b;}){}
    SegmentTree_Sum(const vector<T>& v, const T& def_value = T()) : SegmentTree<T>(v, def_value, [](T a, T b){return a + b;}){}
};

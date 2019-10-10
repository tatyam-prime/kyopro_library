template<class T>
struct SegmentTreeBeats{
    struct Beats{
        T first, second;
        ll cnt;
        Beats(const T& a, const T& b = def_beats, ll c = 1): first(a), second(b), cnt(c){}
        Beats operator+(const Beats& a) const {
            if(first == a.first) return {first, max(second, a.second, [](const T& a, const T& b){ return c(a, b); }), cnt + a.cnt};
            if(first > a.first) return {first, max(second, a.first, [](const T& a, const T& b){ return c(a, b); }), cnt};
            return {a.first, max(first, a.second, [](const T& a, const T& b){ return c(a, b); }), a.cnt};
        }
    };
    T f(const T& a, const T& b) const { return a + b; }
    void m(T& data, Beats& beats, const T& val){
        data += beats.cnt * (val - beats.first);
        beats.first = val;
    }
    static bool c(const T& a, const T& b);
    static const T def_value, def_lazy, def_beats;
    ll size = 1, rank = 0;
    vector<T> data, lazy;
    vector<Beats> beats;
    SegmentTreeBeats(ll n){
        while(size < n){
            size *= 2;
            rank++;
        }
        data.assign(size * 2, def_value);
        lazy.assign(size * 2, def_lazy);
        beats.assign(size * 2, def_value);
        for(ll i = size; --i; ) beats[i] = beats[i * 2] + beats[i * 2 + 1];
    }
    SegmentTreeBeats(const vector<T>& v){
        while(size < v.size()){
            size *= 2;
            rank++;
        }
        data.assign(size * 2, def_value);
        lazy.assign(size * 2, def_lazy);
        beats.assign(size * 2, def_beats);
        for(ll i = 0; i < v.size(); i++){
            data[size + i] = v[i];
            beats[size + i] = v[i];
        }
        for(ll i = size; --i; ){
            data[i] = f(data[i * 2] , data[i * 2 + 1]);
            beats[i] = beats[i * 2] + beats[i * 2 + 1];
        }
    }
    void _push(ll at){
        if(lazy[at] != def_lazy){
            if(c(lazy[at], beats[at * 2].first)){
                m(data[at * 2], beats[at * 2], lazy[at]);
                lazy[at * 2] = min(lazy[at], lazy[at * 2], [](const T& a, const T& b){ return c(a, b); });
            }
            if(c(lazy[at], beats[at * 2 + 1].first)){
                m(data[at * 2 + 1], beats[at * 2 + 1], lazy[at]);
                lazy[at * 2 + 1] = min(lazy[at], lazy[at * 2 + 1], [](const T& a, const T& b){ return c(a, b); });
            }
            lazy[at] = def_lazy;
        }
    }
    void push(ll at){
        if(!at) return;
        for(ll i = 32 - __builtin_clz(at); --i; ) _push(at >> i);
    }
    void _update(ll at){
        data[at] = f(data[at * 2], data[at * 2 + 1]);
        beats[at] = beats[at * 2] + beats[at * 2 + 1];
    }
    void update(ll at){
        while(at /= 2) _update(at);
    }
    T operator[](ll at){
        at += size;
        push(at);
        return data[at];
    }
    void set(ll at, const T& val){
        at += size;
        push(at);
        data[at] = val;
        beats[at] = val;
        update(at);
    }
    void range_query(ll l, ll r, const T& val){
        l += size;
        r += size;
        ll at = 1, rnk = rank;
        while(at){
            if(l < (at + 1) << rnk && at << rnk < r && c(val, beats[at].first)){
                if(at < size) _push(at);
                if(l <= at << rnk && (at + 1) << rnk <= r && c(beats[at].second, val) && c(val, beats[at].first)){
                    m(data[at], beats[at], val);
                    lazy[at] = val;
                }
                else{
                    at *= 2;
                    rnk--;
                    continue;
                }
            }
            while(at & 1){
                at /= 2;
                rnk++;
                _update(at);
            }
            if(!at) return;
            at++;
        }
    }
    T get(ll l, ll r){
        if(l >= r) return def_value;
        T L = def_value, R = def_value;
        l += size; r += size;
        push(l);
        push(r - 1);
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
template<class T> const T SegmentTreeBeats<T>::def_value = T();
template<class T> const T SegmentTreeBeats<T>::def_lazy = numeric_limits<T>::max();
template<class T> const T SegmentTreeBeats<T>::def_beats = numeric_limits<T>::min();
template<class T> bool SegmentTreeBeats<T>::c(const T& a, const T& b){ return a < b; }

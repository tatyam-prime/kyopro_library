template<class T>
struct SegmentTreeBeats{
    struct Beats{
        T first_max, second_max;
        ll cnt;
        Beats(const T& a, const T& b = def_beats, ll c = 1): first_max(a), second_max(b), cnt(c){}
        Beats operator+(const Beats& a) const {
            if(first_max == a.first_max) return {first_max, max(second_max, a.second_max, c), cnt + a.cnt};
            if(first_max > a.first_max) return {first_max, max(second_max, a.first_max, c), cnt};
            return {a.first_max, max(first_max, a.second_max, c), a.cnt};
        }
    };
    inline T f(const T& a, const T& b) const { return a + b; }
    inline void m(T& data, Beats& beats, const T& val){
        data += beats.cnt * (val - beats.first_max);
        beats.first_max = val;
    }
    static inline bool c(const T& a, const T& b);
    static const T def_value, def_beats, identity;
    ll size = 1, rank = 0;
    vector<T> data, lazy;
    vector<Beats> beats;
    SegmentTreeBeats(ll n){
        while(size < n){
            size *= 2;
            rank++;
        }
        data.assign(size * 2, def_value);
        lazy.assign(size * 2, def_value);
        beats.assign(size * 2, def_value);
        for(ll i = size; --i; ) beats[i] = beats[i * 2] + beats[i * 2 + 1];
    }
    SegmentTreeBeats(const vector<T>& v){
        while(size < v.size()){
            size *= 2;
            rank++;
        }
        data.assign(size * 2, def_value);
        lazy.assign(size * 2, def_value);
        beats.assign(size * 2, def_beats);
        for(ll i = 0; i < v.size(); i++){
            data[size + i] = v[i];
            beats[size + i] = v[i];
        }
        for(ll i = size; --i; ){
            data[i] = f(data[i * 2], data[i * 2 + 1]);
            beats[i] = beats[i * 2] + beats[i * 2 + 1];
        }
    }
    inline void _push(ll at){
        if(lazy[at] != def_value){
            if(c(lazy[at], beats[at * 2].first_max)){
                m(data[at * 2], beats[at * 2], lazy[at]);
                lazy[at * 2] = min(lazy[at], lazy[at * 2], c);
            }
            if(c(lazy[at], beats[at * 2 + 1].first_max)){
                m(data[at * 2 + 1], beats[at * 2 + 1], lazy[at]);
                lazy[at * 2 + 1] = min(lazy[at], lazy[at * 2 + 1], c);
            }
            lazy[at] = def_value;
        }
    }
    inline void push(ll at){
        if(!at) return;
        for(ll i = 32 - __builtin_clz(at); --i; ) _push(at >> i);
    }
    inline void _update(ll at){
        data[at] = f(data[at * 2], data[at * 2 + 1]);
        beats[at] = beats[at * 2] + beats[at * 2 + 1];
    }
    inline void update(ll at){
        while(at /= 2) _update(at);
    }
    inline T operator[](ll at){
        at += size;
        push(at);
        return data[at];
    }
    inline void set(ll at, const T& val){
        at += size;
        push(at);
        data[at] = val;
        beats[at] = val;
        update(at);
    }
    inline void chmin(ll l, ll r, const T& val){
        l += size;
        r += size;
        ll at = 1, rnk = rank;
        while(at){
            if(l < (at + 1) << rnk && at << rnk < r && c(val, beats[at].first_max)){
                if(at < size) _push(at);
                if(l <= at << rnk && (at + 1) << rnk <= r && c(beats[at].second_max, val) && c(val, beats[at].first_max)){
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
    T get_f(ll l, ll r){
        if(l >= r) return identity;
        T L = identity, R = identity;
        l += size; r += size;
        push(l);
        push(r - 1);
        for(; l < r; l /= 2, r /= 2){
            if(l & 1) L = f(L, data[l++]);
            if(r & 1) R = f(data[--r], R);
        }
        return f(L, R);
    }
    T get_max(ll l, ll r){
        if(l >= r) return def_beats;
        T L = def_beats, R = def_beats;
        l += size; r += size;
        push(l);
        push(r - 1);
        for(; l < r; l /= 2, r /= 2){
            if(l & 1) L = max(L, beats[l++].first_max, c);
            if(r & 1) R = max(beats[--r].first_max, R, c);
        }
        return max(L, R, c);
    }
};
template<class T> const T SegmentTreeBeats<T>::def_value = numeric_limits<T>::max();
template<class T> const T SegmentTreeBeats<T>::def_beats = numeric_limits<T>::min();
template<class T> const T SegmentTreeBeats<T>::identity = T();
template<class T> bool SegmentTreeBeats<T>::c(const T& a, const T& b){ return a < b; }

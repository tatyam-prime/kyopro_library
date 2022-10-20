template<class T, T (*op)(T, T), T (*e)()>
struct SegmentTree{
    int size;
    vector<T> seg;
    SegmentTree(vector<T> v): size((int)v.size()), seg(move(v)){
        seg.insert(seg.begin(), size, T{});
        for(int i = size; --i >= 1; ) seg[i] = op(seg[i << 1], seg[i << 1 | 1]);
    }
    void set(int i, int d, T x){
        assert(0 <= i && i < size);
        i += size;
        seg[i] = x;
        while(void(i >>= 1), d--) seg[i] = op(seg[i << 1], seg[i << 1 | 1]);
    }
    T get(int l, int r) const {
        assert(0 <= l && l <= r && r <= size);
        l += size;
        r += size;
        T ans = e();
        while(l < r){
            if(l & 1) ans = op(ans, seg[l++]);
            if(r & 1) ans = op(ans, seg[--r]);
            l >>= 1;
            r >>= 1;
        }
        return ans;
    }
};
template<class T, T (*op)(T, T), T (*e)()>
struct SegmentTree2D{
    using Key = int;
    using key_type = pair<Key, Key>;
    using value_type = pair<key_type, T>;
    using Seg = SegmentTree<T, op, e>;
    int size = 1, lg_size = 0;
    vector<vector<key_type>> keys;
    vector<Seg> seg;
    static constexpr auto comp_y = [](const key_type& a, const key_type& b){ return tie(a.second, a.first) < tie(b.second, b.first); };
    SegmentTree2D(vector<value_type> v){
        sort(v.begin(), v.end(), [](value_type& a, value_type& b){ return a.first < b.first; });
        for(int i = 1; i < (int)v.size(); i++) assert(v[i - 1].first < v[i].first);
        while(size < v.size()){
            size <<= 1;
            lg_size++;
        }
        v.resize(size, pair{pair{INT_MAX, INT_MAX}, e()});
        keys.resize(lg_size + 1);
        seg.reserve(lg_size + 1);
        for(int d = 0; ; d++){
            // copy
            auto& Y = keys[d];
            vector<T> V;
            Y.reserve(size);
            V.reserve(size);
            for(auto& [key, val] : v){
                Y.push_back(key);
                V.push_back(val);
            }
            seg.emplace_back(move(V));
            
            if(d == lg_size) break;
            // merge
            const int w = 1 << d;
            for(int l = 0; l < size; l += w * 2){
                auto p = v.begin() + l;
                inplace_merge(p, p + w, p + w * 2, [](const value_type& a, const value_type& b){ return tie(a.first.second, a.first.first) < tie(b.first.second, b.first.first); });
            }
        }
    }
    void set_inner(int d, int i, key_type key, T val){
        auto& Y = keys[d];
        int l = i << d, r = (i + 1) << d;
        seg[d].set(lower_bound(Y.begin() + l, Y.begin() + r - 1, key, comp_y) - Y.begin(), d, val);
    }
    void set(key_type key, T val){
        auto& X = keys[0];
        int i = lower_bound(X.begin(), X.end(), key) - X.begin();
        assert(i < size && X[i] == key);
        for(int d = 0; d <= lg_size; d++, i >>= 1) set_inner(d, i, key, val);
    }
    void set(Key x, Key y, T val){
        set({x, y}, val);
    }
    T get(key_type key) const {
        auto& X = keys[0];
        int i = lower_bound(X.begin(), X.end(), key) - X.begin();
        assert(i < size && X[i] == key);
        return seg[0].seg[size + i];
    }
    T get(Key x, Key y) const {
        return get({x, y});
    }
    T get_inner(int d, int i, key_type y1, key_type y2) const {
        auto& Y = keys[d];
        int l = i << d, r = (i + 1) << d;
        l = lower_bound(Y.begin() + l, Y.begin() + r, y1, comp_y) - Y.begin();
        r = upper_bound(Y.begin() + l, Y.begin() + r, y2, comp_y) - Y.begin();
        return seg[d].get(l, r);
    }
    T get(key_type L, key_type R) const {
        assert(L.first <= R.first);
        assert(L.second <= R.second);
        auto& X = keys[0];
        int l = lower_bound(X.begin(), X.end(), L) - X.begin();
        int r = upper_bound(X.begin(), X.end(), R) - X.begin();
        T ans = e();
        for(int d = 0; l < r; d++, l >>= 1, r >>= 1){
            if(l & 1) ans = op(ans, get_inner(d, l++, L, R));
            if(r & 1) ans = op(ans, get_inner(d, --r, L, R));
        }
        return ans;
    }
    /// [x1, x2] * [y1, y2]
    T get(int x1, int x2, int y1, int y2) const {
        return get({x1, y1}, {x2, y2});
    }
};

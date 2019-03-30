using ll = long long; 


struct SegmentTree{ // range-sum
    ll size;
    vector<ll> data;
    SegmentTree(ll n){
        size = 1;
        while(size < n) size *= 2;
        data.resize(size * 2);
    }
    void add(ll at, ll val){
        at += size;
        data[at] += val;
        while(at /= 2) data[at] += val;
    }
    ll get(ll l, ll r){
        ll ans = 0;
        l += size;
        r += size;
        while(l < r){
            if(l & 1) ans += data[l++];
            if(r & 1) ans += data[--r];
            l /= 2;
            r /= 2;
        }
        return ans;
    }
};

/* options
SegmentTree(vector<ll> v){
    size = 1;
    while(size < v.size()) size *= 2;
    data.resize(size * 2);
    for(ll i = 0; i < v.size(); i++) data[size + i] = v[i];
    for(ll i = size - 1; i > 0; i--) data[i] = data[i * 2] + data[i * 2 + 1];
}
void set(ll at, ll val){
    add(at, val - data[size + at]);
}
*/

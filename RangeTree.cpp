using ll = int64_t;
using pll = pair<ll, ll>;
const LINF = 0x1fffffffffffffff;


struct RangeSet{
    vector<vector<ll>> data;
    vector<ll> x;
    ll siz_x = 1;
    RangeSet(vector<pll> a){
        sort(a.begin(), a.end());
        if(a.size()) x.push_back(a[0].first);
        for(auto& i : a) if(x.back() != i.first) x.push_back(i.first);
        while(x.size() > siz_x) siz_x *= 2;
        while(x.size() < siz_x) x.push_back(LINF);
        data.resize(siz_x * 2);
        ll at = 0;
        for(auto& i : a){
            if(x[at] != i.first) at++;
            data[siz_x + at].push_back(i.second);
        }
        for(ll i = siz_x; --i; ){
            data[i].resize(data[i * 2].size() + data[i * 2 + 1].size());
            merge(data[i * 2].begin(), data[i * 2].end(), data[i * 2 + 1].begin(), data[i * 2 + 1].end(), data[i].begin());
        }
    }
    ll count(ll x1, ll x2, ll y1, ll y2) const { // [x1, x2) × [y1, y2)
        ll l = lower_bound(x.begin(), x.end(), x1) - x.begin(), r = lower_bound(x.begin(), x.end(), x2) - x.begin();
        l += siz_x; r += siz_x;
        ll ans = 0;
        for(; l < r; l /= 2, r /= 2){
            if(l & 1){
                ans += lower_bound(data[l].begin(), data[l].end(), y2) - lower_bound(data[l].begin(), data[l].end(), y1);
                l++;
            }
            if(r & 1){
                r--;
                ans += lower_bound(data[r].begin(), data[r].end(), y2) - lower_bound(data[r].begin(), data[r].end(), y1);
            }
        }
        return ans;
    }
};

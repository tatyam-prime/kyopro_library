using namespace std;
using ll = long long;


struct PartialSum{
    vector<ll> data;
    template<class T> PartialSum(const T& v): data(v.size() + 1){
        for(ll i = 0; i < v.size(); i++) data[i + 1] = data[i] + v[i];
    }
    ll get(ll l, ll r) const {
        return data[r] - data[l];
    }
    ll operator[](ll at) const { return data[at]; }
};

struct PartialSum2D{
    vector<vector<ll>> data;
    template<class T> PartialSum2D(const T& v): data(v.size() + 1, vector<ll>(v[0].size() + 1)){
        for(ll i = 0; i < v.size(); i++) for(ll j = 0; j < v[i].size(); j++) data[i + 1][j + 1] = data[i][j + 1] + v[i][j];
        for(ll i = 0; i < v.size(); i++) for(ll j = 0; j < v[i].size(); j++) data[i + 1][j + 1] += data[i + 1][j];
    }
    ll get(ll x1, ll x2, ll y1, ll y2) const {
        return data[x2][y2] + data[x1][y1] - data[x1][y2] - data[x2][y1];
    }
    const vector<ll>& operator[](ll at) const { return data[at]; }
};

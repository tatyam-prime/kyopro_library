using ll = long long;

const ll mod = 1000000007; // 面倒なのでここで
inline constexpr ll extgcd(ll a, ll b, ll &x, ll &y){ ll g = a; x = 1; y = 0; if(b){ g = extgcd(b, a % b, y, x); y -= a / b * x; } return g; }
inline constexpr ll invmod(ll a, ll m = mod){ ll x = 0, y = 0; extgcd(a, m, x, y); return (x + m) % m; }
struct Modint{
    ll _num;
    constexpr Modint() : _num() { _num = 0; }
    constexpr Modint(ll x) : _num() { _num = x % mod; if(_num < 0) _num += mod; }
    inline constexpr Modint operator= (ll x){ _num = x % mod; if(_num<0) _num += mod; return *this; }
    inline constexpr Modint operator+ (ll x){ return Modint(_num + x); }
    inline constexpr Modint operator+ (Modint x){ ll a = _num + x._num; if(a >= mod) a -= mod; return Modint{a}; }
    inline constexpr Modint operator+=(ll x){ _num += x; _num %= mod; if(_num < 0) _num += mod; return *this; }
    inline constexpr Modint operator+=(Modint x){ _num += x._num; if(_num >= mod) _num -= mod; return *this; }
    inline constexpr Modint operator- (ll x){ return Modint(_num - x); }
    inline constexpr Modint operator- (Modint x){ ll a = _num - x._num; if(a < 0) a += mod; return Modint{a}; }
    inline constexpr Modint operator-=(ll x){ _num -= x; _num %= mod; if(_num < 0) _num += mod; return *this; }
    inline constexpr Modint operator-=(Modint x){ _num -= x._num; if(_num < 0) _num += mod; return *this; }
    inline constexpr Modint operator* (ll x){ return Modint(_num * (x % mod)); }
    inline constexpr Modint operator* (Modint x){ return Modint{_num * x._num % mod}; }
    inline constexpr Modint operator*=(ll x){ x %= mod; _num *= x; _num %= mod; if(_num < 0) _num += mod; return *this; }
    inline constexpr Modint operator*=(Modint x){ _num *= x._num; _num %= mod; return *this; }
    inline constexpr Modint operator/ (ll x){ return Modint(_num * invmod(x % mod, mod)); }
    inline constexpr Modint operator/ (Modint x){ return Modint{_num * invmod(x._num, mod) % mod}; }
    inline constexpr Modint operator/=(ll x){ _num *= invmod(x % mod, mod); _num %= mod; if(_num < 0) _num += mod; return *this; }
    inline constexpr Modint operator/=(Modint x){ _num *= invmod(x._num, mod); _num %= mod; return *this; }
    inline constexpr Modint pow(ll x){ ll i = 1; Modint ans = 1, cnt = *this; while(i <= x){ if(x & i){ ans *= cnt; x ^= i; } cnt *= cnt; i *= 2; } return ans; }
    operator ll() const { return _num; }
};
vector<Modint>fac(1, 1),inv(1, 1);
inline void reserve(ll a){
    if(fac.size() >= a) return;
    if(a < fac.size() * 2) a = fac.size() * 2;
    while(fac.size() < a) fac.push_back(fac.back() * ll(fac.size()));
    inv.resize(fac.size());
    inv.back() = Modint(1) / fac.back();
    for(ll i = inv.size() - 1; !inv[i - 1]; i--) inv[i - 1] = inv[i] * i;
}
inline Modint fact(ll n){ reserve(n + 1); return fac[n]; }
inline Modint perm(ll n, ll r){ reserve(n + 1); return fac[n] * inv[n - r]; }
inline Modint comb(ll n, ll r){ reserve(n + 1); return fac[n] * inv[r] * inv[n - r]; }
inline Modint Mcomb(ll n, ll r){ return comb(n + r - 1, n - 1); } // r個をn部屋に分ける

//　exawizards2019_e
signed main(){
    Modint ans = 1;
    ans /= 2;
    Modint cnt = ans;
    ll b, w;
    cin >> b >> w;
    for(int i = 0; i < b + w; i++){
        cout << ans << '\n';
        cnt /= 2;
        if(i >= w - 1) ans += comb(i, w - 1) * cnt;
        if(i >= b - 1) ans -= comb(i, b - 1) * cnt;
    }
}

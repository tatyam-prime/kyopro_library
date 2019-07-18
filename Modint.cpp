#include <bits/stdc++.h>
using ll = long long;

constexpr ll mod = 1000000007; // 面倒なのでここで
inline constexpr ll extgcd(ll a, ll b, ll &x, ll &y){ ll g = a; x = 1; y = 0; if(b){ g = extgcd(b, a % b, y, x); y -= a / b * x; } return g; }
inline constexpr ll invmod(ll a, ll m = mod){ ll x = 0, y = 0; extgcd(a, m, x, y); return (x + m) % m; }
struct Modint{
    ll num;
    constexpr Modint() : num() { num = 0; }
    constexpr Modint(ll x) : num() { num = x % mod; if(num < 0) num += mod; }
    inline constexpr Modint& operator= (int x){ num = x % mod; if(num < 0) num += mod; return *this; }
    inline constexpr Modint& operator= (ll x){ num = x % mod; if(num < 0) num += mod; return *this; }
    inline constexpr Modint& operator= (Modint x){ num = x.num; return *this; }
    inline constexpr Modint operator+ (int x) const { return Modint(num + x); }
    inline constexpr Modint operator+ (ll x) const { return Modint(num + x); }
    inline constexpr Modint operator+ (Modint x) const { ll a = num + x.num; if(a >= mod) a -= mod; return Modint{a}; }
    inline constexpr Modint& operator+=(int x){ num += x; num %= mod; if(num < 0) num += mod; return *this; }
    inline constexpr Modint& operator+=(ll x){ num += x; num %= mod; if(num < 0) num += mod; return *this; }
    inline constexpr Modint& operator+=(Modint x){ num += x.num; if(num >= mod) num -= mod; return *this; }
    inline constexpr Modint& operator++(){ if(num == mod - 1) num = 0; else num++; return *this; }
    inline constexpr Modint operator++(int){ Modint ans = *this; if(num == mod - 1) num = 0; else num++; return ans; }
    inline constexpr Modint operator- () const { if(num == 0) return 0; return Modint(mod - num); }
    inline constexpr Modint operator- (int x) const { return Modint(num - x); }
    inline constexpr Modint operator- (ll x) const { return Modint(num - x); }
    inline constexpr Modint operator- (Modint x) const { ll a = num - x.num; if(a < 0) a += mod; return Modint{a}; }
    inline constexpr Modint& operator-=(int x){ num -= x; num %= mod; if(num < 0) num += mod; return *this; }
    inline constexpr Modint& operator-=(ll x){ num -= x; num %= mod; if(num < 0) num += mod; return *this; }
    inline constexpr Modint& operator-=(Modint x){ num -= x.num; if(num < 0) num += mod; return *this; }
    inline constexpr Modint& operator--(){ if(num == 0) num = mod - 1; else num--; return *this; }
    inline constexpr Modint operator--(int){ Modint ans = *this; if(num == 0) num = mod - 1; else num--; return ans; }
    inline constexpr Modint operator* (int x) const { return Modint(num * (x % mod)); }
    inline constexpr Modint operator* (ll x) const { return Modint(num * (x % mod)); }
    inline constexpr Modint operator* (Modint x) const { return Modint{num * x.num % mod}; }
    inline constexpr Modint& operator*=(int x){ num *= Modint(x); num %= mod; return *this; }
    inline constexpr Modint& operator*=(ll x){ num *= Modint(x); num %= mod; return *this; }
    inline constexpr Modint& operator*=(Modint x){ num *= x.num; num %= mod; return *this; }
    inline constexpr Modint operator/ (int x) const { return Modint(num * invmod(Modint(x), mod)); }
    inline constexpr Modint operator/ (ll x) const { return Modint(num * invmod(Modint(x), mod)); }
    inline constexpr Modint operator/ (Modint x) const { return Modint{num * invmod(x.num, mod) % mod}; }
    inline constexpr Modint& operator/=(int x){ num *= invmod(Modint(x), mod); num %= mod; return *this; }
    inline constexpr Modint& operator/=(ll x){ num *= invmod(Modint(x), mod); num %= mod; return *this; }
    inline constexpr Modint& operator/=(Modint x){ num *= invmod(x.num, mod); num %= mod; return *this; }
    inline constexpr Modint pow(ll x) const { ll i = 1; Modint ans = 1, cnt = *this; while(i <= x){ if(x & i){ ans *= cnt; x ^= i; } cnt *= cnt; i *= 2; } return ans; }
    inline constexpr operator ll() const { return num; }
};
std::istream& operator>>(std::istream& is, Modint& x) { is >> x.num; x = Modint(x.num); return is; }
inline constexpr Modint operator""_M(ull x) { return Modint(x); }
std::vector<Modint> fac(1, 1), inv(1, 1);
inline void reserve(ll a){
    if(fac.size() >= a) return;
    if(a < fac.size() * 2) a = fac.size() * 2;
    if(a >= mod) a = mod;
    while(fac.size() < a) fac.push_back(fac.back() * ll(fac.size()));
    inv.resize(fac.size());
    inv.back() = Modint(1) / fac.back();
    for(ll i = inv.size() - 1; !inv[i - 1]; i--) inv[i - 1] = inv[i] * i;
}
inline Modint fact(ll n){ if(n < 0) return 0; reserve(n + 1); return fac[n]; }
inline Modint perm(ll n, ll r){ if(r < 0 || n < r) return 0; reserve(n + 1); return fac[n] * inv[n - r]; }
inline Modint comb(ll n, ll r){ if(r < 0 || n < r) return 0; reserve(n + 1); return fac[n] * inv[r] * inv[n - r]; }
inline Modint Mcomb(ll n, ll r){ return comb(n + r - 1, n - 1); } // r個をn部屋に分ける


//　exawizards2019_e
int main(){
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

using ll = long long;
using ull = unsigned long long;


constexpr unsigned mod = 1000000007;
struct Modint{
    unsigned num = 0;
    constexpr Modint(){}
    constexpr Modint(const Modint &x) : num(x.num){}
    inline constexpr operator ll() const { return num; }
    inline constexpr Modint& operator+=(Modint x){ num += x.num; if(num >= mod) num -= mod; return *this; }
    inline constexpr Modint& operator++(){ if(num == mod - 1) num = 0; else num++; return *this; }
    inline constexpr Modint operator++(int){ Modint ans(*this); operator++(); return ans; }
    inline constexpr Modint operator- () const { return Modint(0) -= *this; }
    inline constexpr Modint operator- (Modint x) const { return Modint(*this) -= x; }
    inline constexpr Modint& operator-=(Modint x){ if(num < x.num) num += mod; num -= x.num; return *this; }
    inline constexpr Modint& operator--(){ if(num == 0) num = mod - 1; else num--; return *this; }
    inline constexpr Modint operator--(int){ Modint ans(*this); operator--(); return ans; }
    inline constexpr Modint& operator*=(Modint x){ num = ull(num) * x.num % mod; return *this; }
    inline constexpr Modint& operator/=(Modint x){ return operator*=(x.inv()); }
    template<class T> constexpr Modint(T x){
        using U = typename conditional<sizeof(T) >= 4, T, int>::type;
        U y = x; y %= U(mod); if(y < 0) y += mod; num = unsigned(y);
    }
    template<class T> inline constexpr Modint operator+(T x) const { return Modint(*this) += x; }
    template<class T> inline constexpr Modint& operator+=(T x){ x %= mod; if(x < 0) x += mod; num += x; if(num >= mod) num -= mod; return *this; }
    template<class T> inline constexpr Modint operator- (T x) const { return Modint(*this) -= x; }
    template<class T> inline constexpr Modint& operator-=(T x){ return operator-=(Modint(x)); }
    template<class T> inline constexpr Modint operator* (T x) const { return Modint(*this) *= x; }
    template<class T> inline constexpr Modint& operator*=(T x){ return operator*=(Modint(x)); }
    template<class T> inline constexpr Modint operator/ (T x) const { return Modint(*this) /= x; }
    template<class T> inline constexpr Modint& operator/=(T x){ return operator/=(Modint(x)); }
    inline constexpr Modint inv() const { ll x = 0, y = 0; extgcd(num, mod, x, y); return x; }
    inline constexpr ll extgcd(ll a, ll b, ll &x, ll &y) const { ll g = a; x = 1; y = 0; if(b){ g = extgcd(b, a % b, y, x); y -= a / b * x; } return g; }
    inline constexpr Modint pow(ull x) const { Modint ans = 1, cnt = *this; while(x){ if(x & 1) ans *= cnt; cnt *= cnt; x /= 2; } return ans; }
};
std::istream& operator>>(std::istream& is, Modint& x) { ll a; is >> a; x = a; return is; }
inline constexpr Modint operator""_M(ull x) { return Modint(x); }
std::vector<Modint> fac(1, 1), inv(1, 1);
inline void reserve(ll a){
    if(fac.size() >= a) return;
    if(a < fac.size() * 2) a = fac.size() * 2;
    if(a >= mod) a = mod;
    while(fac.size() < a) fac.push_back(fac.back() * Modint(fac.size()));
    inv.resize(fac.size());
    inv.back() = fac.back().inv();
    for(ll i = inv.size() - 1; !inv[i - 1]; i--) inv[i - 1] = inv[i] * i;
}
inline Modint fact(ll n){ if(n < 0) return 0; reserve(n + 1); return fac[n]; }
inline Modint perm(ll n, ll r){ if(r < 0 || n < r) return 0; reserve(n + 1); return fac[n] * inv[n - r]; }
inline Modint comb(ll n, ll r){ if(r < 0 || n < r) return 0; reserve(n + 1); return fac[n] * inv[r] * inv[n - r]; }
inline Modint Mcomb(ll n, ll r){ return comb(n + r - 1, n - 1); } // r個をn部屋に分ける
inline Modint catalan(ll n){ if(n < 0) return 0; reserve(n * 2 + 1); return fac[n * 2] * inv[n] * inv[n + 1]; }


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

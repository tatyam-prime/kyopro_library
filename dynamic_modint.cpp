template<int id> class dynamic_modint {
    using mint = dynamic_modint;
    struct barrett {
        uint32_t _m;
        uint64_t im;
        barrett(uint32_t mod) : _m(mod), im(uint64_t(-1) / mod + 1) {}
        operator uint32_t() const { return _m; }
        uint32_t mul(uint32_t a, uint32_t b) const {
            uint64_t z = a;
            z *= b;
            uint64_t x = uint64_t(__uint128_t(z) * im >> 64);
            uint64_t y = x * _m;
            return uint32_t(z - y + (z < y ? _m : 0));
        }
    };
    static inline barrett m = 1;
    uint32_t _v = 0;
    static pair<int32_t, int32_t> inv_gcd(int32_t a, int32_t b) {
        if (a == 0) return {b, 0};
        int32_t s = b, t = a, m0 = 0, m1 = 1;
        while (t) {
            const int32_t u = s / t;
            s -= t * u; m0 -= m1 * u;
            swap(s, t); swap(m0, m1);
        }
        if (m0 < 0) m0 += b / s;
        return {s, m0};
    }
public:
    static mint raw(uint32_t v) { mint a; a._v = v; return a; }
    dynamic_modint() {}
    template <class T>
    dynamic_modint(T v) {
        static_assert(is_integral_v<T>, "T is not integral type.");
        if constexpr (is_signed_v<T>) {
            int64_t x = int64_t(v % int64_t(m));
            if (x < 0) x += m; _v = uint32_t(x);
        }
        else _v = uint32_t(v % m);
    }
    static uint32_t mod() { return m; }
    static void set_mod(int mod) {
        assert(1 <= mod);
        m = mod;
    }
    uint32_t val() const { return _v; }
    mint& operator++() { return *this += 1; }
    mint& operator--() { return *this -= 1; }
    mint operator++(int) { mint res = *this; ++*this; return res; }
    mint operator--(int) { mint res = *this; --*this; return res; }
    mint& operator+=(mint rhs) {
        if (_v >= m - rhs._v) _v -= m;
        _v += rhs._v; return *this;
    }
    mint& operator-=(mint rhs) {
        if (_v < rhs._v) _v += m;
        _v -= rhs._v; return *this;
    }
    mint& operator*=(mint rhs) { return *this = *this * rhs; }
    mint& operator/=(mint rhs) { return *this *= rhs.inv(); }
    mint operator+() const { return *this; }
    mint operator-() const { return mint{} - *this; }
    mint pow(long long n) const {
        assert(0 <= n);
        if(n == 0) return 1;
        mint x = *this, r = 1;
        while (1) {
            if (n & 1) r *= x; n >>= 1;
            if (n == 0) return r;
            x *= x;
        }
    }
    mint inv() const {
        auto eg = inv_gcd(_v, m);
        assert(eg.first == 1);
        return eg.second;
    }
    friend mint operator+(mint lhs, mint rhs) { return lhs += rhs; }
    friend mint operator-(mint lhs, mint rhs) { return lhs -= rhs; }
    friend mint operator*(mint lhs, mint rhs) { return raw(m.mul(lhs._v, rhs._v)); }
    friend mint operator/(mint lhs, mint rhs) { return lhs /= rhs; }
    friend bool operator==(mint lhs, mint rhs) { return lhs._v == rhs._v; }
    friend bool operator!=(mint lhs, mint rhs) { return lhs._v != rhs._v; }
};
using mint = dynamic_modint<-1>;
istream& operator>>(istream& in, mint& x) { long long a; in >> a; x = a; return in; }
ostream& operator<<(ostream& out, mint x) { return out << x.val(); }
mint operator""_M(unsigned long long x) { return x; }

constexpr uint32_t fact_mx = 2e7;
mint fac[fact_mx + 1], inv[fact_mx + 1];
void prepare_factorial() {
    fac[0] = 1;
    for(uint32_t i = 1; i <= fact_mx; i++) fac[i] = fac[i - 1] * mint::raw(i);
    inv[fact_mx] = fac[fact_mx].inv();
    for(uint32_t i = fact_mx; i; i--) inv[i - 1] = inv[i] * mint::raw(i);
}
mint inverse(long long n) { return inv[n] * fac[n - 1]; }
mint perm(long long n, long long r) {
    if(n < r || r < 0) return 0;
    if(n > fact_mx) [[unlikely]] {
        mint ans = 1, x = n;
        while(r--) ans *= x--;
        return ans;
    }
    return fac[n] * inv[n - r];
}
mint comb(long long n, long long r) {
    if(n < r || r < 0) return 0;
    r = min(r, n - r);
    const mint ans = perm(n, r);
    return ans * inv[r];
}
template<class... T> mint comb(long long n, T... rs) {
    if(n < 0) return 0;
    mint ans = fac[n];
    long long rn = n;
    for(long long r : {rs...}) {
        if(r < 0) return 0;
        ans *= inv[r];
        rn -= r;
    }
    if(rn < 0) return 0;
    return ans * inv[rn];
}
mint Mcomb(long long n, long long r){ return comb(n + r - 1, r); } // r balls into n boxes

// https://ei1333.github.io/luzhiled/snippets/string/rolling-hash.html を改造


using ll = long long;
using ull = unsigned long long;


const unsigned bases[64] = {257,262,266,275,276,281,285,290,296,302,306,310,311,313,323,333,344,345,350,357,367,370,373,402,423,425,431,440,442,443,454,457,458,462,471,478,481,487,489,492,499,501,502,503,506,514,524,532,535,541,550,552,557,559,562,563,567,570,571,580,592,597,604,612};
const ull mod = 0x1fffffffffffffff, base = bases[chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now().time_since_epoch()).count() & 63];
struct RollingHash {
    vector<ull> hashed, power;
    
    static constexpr ull mask(ll a){ return (1ull << a) - 1; }
    
    inline ull mul(ull a, ull b) const {
        //*
        __uint128_t ans = __uint128_t(a) * b;
        /*/
         // without __uint128_t
         ull a31 = a >> 31, b31 = b >> 31;
         a &= mask(31);
         b &= mask(31);
         ull x = a * b31 + b * a31;
         ull ans = (a31 * b31 << 1) + (x >> 30) + ((x & mask(30)) << 31) + a * b;
         //*/
        ans = (ans >> 61) + (ans & mod);
        if(ans >= mod) ans -= mod;
        return ans;
    }
    
    
    RollingHash(const string &s) {
        ll n = s.size();
        hashed.assign(n + 1, 0);
        power.assign(n + 1, 0);
        power[0] = 1;
        for(ll i = 0; i < n; i++) {
            power[i + 1] = mul(power[i], base);
            hashed[i + 1] = mul(hashed[i], base) + s[i];
            if(hashed[i + 1] >= mod) hashed[i + 1] -= mod;
        }
    }
    
    ull get(ll l, ll r) const {
        ull ret = hashed[r] + mod - mul(hashed[l], power[r - l]);
        if(ret >= mod) ret -= mod;
        return ret;
    }
    
    ull connect(ull h1, ull h2, ll h2len) const {
        ull ret = mul(h1, power[h2len]) + h2;
        if(ret >= mod) ret -= mod;
        return ret;
    }
    
    void connect(const string &s){
        ll n = hashed.size() - 1, m = s.size();
        hashed.resize(n + m + 1);
        power.resize(n + m + 1);
        for(ll i = n; i < n + m; i++) {
            power[i + 1] = mul(power[i], base);
            hashed[i + 1] = mul(hashed[i], base) + s[i - n];
            if(hashed[i + 1] >= mod) hashed[i + 1] -= mod;
        }
    }
    
    ll LCP(const RollingHash &b, ll l1, ll r1, ll l2, ll r2) {
        ll len = min(r1 - l1, r2 - l2);
        ll low = -1, high = len + 1;
        while(high - low > 1) {
            ll mid = (low + high) / 2;
            if(get(l1, l1 + mid) == b.get(l2, l2 + mid)) low = mid;
            else high = mid;
        }
        return low;
    }
};



/*

mod 2^61 - 1 の RollingHash です  結構強いです
名前の衝突がある場合は namespace で囲うと良いです
bases : 256 以上の原始根を並べています  特に意味はないです
hashed[i] : s[0, i) のハッシュ結果  hashed[i + 1] = s[i] + s[i-1] * power[1] + s[i-2] * power[2] + ...
power[i] : base ** i
get(l, r) : s[l, r) のハッシュ結果  s[r-1] + s[r-2] * power[1] + ... + s[l] * power[r - l + 1]
connect(h1, h2, h2len) : 2つのハッシュ結果からそれらを繋げたときのハッシュ結果を得ます
connect(s) : コンストラクタで渡した文字列に s を繋げたことにします  過去必要になったやつ
LCP(...) : Longest Common Prefix の長さ

*/

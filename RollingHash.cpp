// https://ei1333.github.io/luzhiled/snippets/string/rolling-hash.html を改造


const int bases[64] = {258, 259, 263, 264, 265, 268, 269, 270, 271, 272, 273, 275, 276, 278, 279, 280, 281, 282, 283, 284, 286, 287, 288, 292, 295, 296, 298, 299, 300, 302, 305, 307, 310, 311, 312, 313, 314, 316, 317, 319, 320, 323, 325, 326, 327, 328, 331, 332, 337, 338, 342, 343, 347, 348, 349, 353, 356, 357, 359, 363, 371, 373, 374, 377};
const ull mod = 281474976710597, base = bases[random_device()() & 63];
struct RollingHash {
    vector<ull> hashed, power;
    inline ull mul(ull a, ull b) const {
        return __uint128_t(a) * b % mod;
    }
    
    RollingHash(const string &s) {
        ll n = s.size();
        hashed.assign(n + 1, 0);
        power.assign(n + 1, 0);
        power[0] = 1;
        rep(n) {
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
    
    ull connect(ull h1, ll h2, ll h2len) const {
        ull ret = mul(h1, power[h2len]) + h2;
        if(ret >= mod) ret -= mod;
        return ret;
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

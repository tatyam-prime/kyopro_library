// https://ei1333.github.io/luzhiled/snippets/string/rolling-hash.html を改造


const int bases[64] = {261, 263, 264, 266, 267, 268, 269, 270, 272, 275, 276, 281, 285, 292, 293, 295, 297, 301, 302, 305, 310, 311, 325, 327, 329, 333, 341, 347, 348, 350, 351, 355, 358, 360, 367, 371, 373, 375, 378, 380, 383, 385, 386, 389, 396, 397, 399, 401, 402, 403, 405, 408, 414, 416, 418, 422, 430, 433, 434, 437, 438, 448, 453, 454};
const ull mod = 36028797018963913, base = bases[random_device()() & 63];
struct RollingHash {
    vector<ull> hashed, power;
    
    inline ull mul(ull a, ull b) const {
        return __uint128_t(a) * b % mod;
    }
    
    inline ull mul2(ull a, ull b) const {
        return a * b % mod;
    }
    
    RollingHash(const string &s) {
        ll n = s.size();
        hashed.assign(n + 1, 0);
        power.assign(n + 1, 0);
        power[0] = 1;
        rep(n) {
            power[i + 1] = mul2(power[i], base);
            hashed[i + 1] = mul2(hashed[i], base) + s[i];
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
    
    void connect(const string &s){
        ll n = hashed.size() - 1, m = s.size();
        hashed.resize(n + m + 1);
        power.resize(n + m + 1);
        rep(i, n, n + m) {
            power[i + 1] = mul2(power[i], base);
            hashed[i + 1] = mul2(hashed[i], base) + s[i - n];
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



// for Codeforces


const int bases[64] = {259, 261, 264, 269, 270, 280, 281, 284, 285, 286, 288, 295, 298, 304, 307, 309, 310, 311, 312, 316, 318, 319, 322, 323, 328, 330, 348, 350, 352, 353, 355, 359, 360, 367, 378, 380, 381, 382, 384, 390, 393, 394, 395, 397, 398, 399, 402, 410, 412, 413, 414, 416, 417, 424, 434, 435, 438, 440, 442, 449, 450, 462, 463, 464};
const ull mod = 4294967291, base = bases[chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now().time_since_epoch()).count() & 63];
struct RollingHash {
    vector<ull> hashed, power;
    
    inline ull mul(ull a, ull b) const {
        return a * b % mod;
    }
    
    inline ull mul2(ull a, ull b) const {
        ull x = a * b;
        unsigned xh = unsigned(x >> 32), xl = unsigned(x), d, m;
        asm("divl %4; \n\t" : "=a" (d), "=d" (m) : "d" (xh), "a" (xl), "r" (mod));
        return m;
    }
    
    RollingHash(const string &s) {
        ll n = s.size();
        hashed.assign(n + 1, 0);
        power.assign(n + 1, 0);
        power[0] = 1;
        rep(n) {
            power[i + 1] = mul2(power[i], base);
            hashed[i + 1] = mul2(hashed[i], base) + s[i];
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
    
    void connect(const string &s){
        ll n = hashed.size() - 1, m = s.size();
        hashed.resize(n + m + 1);
        power.resize(n + m + 1);
        rep(i, n, n + m) {
            power[i + 1] = mul2(power[i], base);
            hashed[i + 1] = mul2(hashed[i], base) + s[i - n];
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

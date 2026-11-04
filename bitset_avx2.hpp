#include <immintrin.h>
#pragma GCC target("avx2")
#pragma GCC optimize("Ofast,unroll-loops")
template<size_t N> struct alignas(32) Bitset {
    using V = __m256i;
    using u64 = uint64_t;
    using u64a = uint64_t __attribute__((may_alias));
    static_assert(N && N % 256 == 0);
    static constexpr size_t K = N / 256, W = N / 64;
    V a[K];
    static inline const V zero{};
    void reset() { for (auto& x : a) x = zero; }
    bool get(size_t i) const { return ((const u64a*)a)[i >> 6] >> (i & 63) & 1; }
    bool operator[](size_t i) const { return get(i); }
    void set(size_t i, bool x = 1) {
        u64a& w = ((u64a*)a)[i >> 6];
        u64 m = 1ull << (i & 63);
        w = (w & ~m) | (-u64(x) & m);
    }
    static Bitset op(auto f, const same_as<Bitset> auto&... x) {
        Bitset r;
        for (size_t i = 0; i < K; ++i) r.a[i] = f(x.a[i]...);
        return r;
    }
#define BOP(c) \
    friend Bitset operator c(const Bitset& x, const Bitset& y) { return op([](V x, V y) { return x c y; }, x, y); } \
    Bitset& operator c##=(const Bitset& y) { \
        for (size_t i = 0; i < K; ++i) a[i] c##= y.a[i]; \
        return *this; \
    }
    BOP(&) BOP(|) BOP(^)
#undef BOP
    friend Bitset operator~(const Bitset& x) { return op([](V x) { return ~x; }, x); }
    static unsigned mask(V x, V y = zero) { return 15 ^ _mm256_movemask_pd((__m256d)(x == y)); }
    bool operator==(const Bitset& y) const {
        for (size_t i = 0; i < K; ++i) if (mask(a[i], y.a[i])) return 0;
        return 1;
    }
    // Lexicographic order of bits [0], [1], ... (0 < 1).
    strong_ordering operator<=>(const Bitset& y) const {
        for (size_t i = 0; i < K; ++i) if (unsigned m = mask(a[i], y.a[i])) {
            size_t j = countr_zero(m);
            u64 x = ((const u64a*)&a[i])[j], z = ((const u64a*)&y.a[i])[j];
            return (x & (x ^ z) & -(x ^ z)) ? strong_ordering::greater : strong_ordering::less;
        }
        return strong_ordering::equal;
    }
    template<bool R>
    static void shift(const Bitset& x, Bitset& z, size_t k) {
        if (k >= N) { z.reset(); return; }
        size_t q = k >> 6, b = k & 63, m = W - q, t = 0;
        const u64a* s = (const u64a*)x.a + (R ? q : 0);
        u64a* d = (u64a*)z.a + (R ? 0 : q);
        if (!b) memmove(d, s, m * 8);
        else {
            __m128i B = _mm_cvtsi32_si128(b), C = _mm_cvtsi32_si128(64 - b);
            for (; t + 4 < m; t += 4) {
                size_t i = R ? t : m - 4 - t;
                V u = _mm256_loadu_si256((const V*)(s + i));
                V v = _mm256_loadu_si256((const V*)(s + i + (R ? 1 : -1)));
                _mm256_store_si256((V*)(d + i), R ?
                    _mm256_srl_epi64(u, B) | _mm256_sll_epi64(v, C) :
                    _mm256_sll_epi64(u, B) | _mm256_srl_epi64(v, C));
            }
            for (; t < m; ++t) {
                size_t i = R ? t : m - 1 - t;
                u64 v = (R ? i + 1 < m : i != 0) ? s[i + (R ? 1 : -1)] : 0;
                d[i] = R ? (s[i] >> b) | (v << (64 - b)) : (s[i] << b) | (v >> (64 - b));
            }
        }
        memset((u64a*)z.a + (R ? m : 0), 0, q * 8);
    }
    template<bool R>
    Bitset sh(size_t k) const { Bitset z; shift<R>(*this, z, k); return z; }
    Bitset operator<<(size_t k) const { return sh<0>(k); }
    Bitset operator>>(size_t k) const { return sh<1>(k); }
    Bitset& operator<<=(size_t k) { shift<0>(*this, *this, k); return *this; }
    Bitset& operator>>=(size_t k) { shift<1>(*this, *this, k); return *this; }
    u64 count() const {
        V lut = _mm256_broadcastsi128_si256(_mm_setr_epi8(0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4));
        V m = _mm256_set1_epi8(15), s = zero;
        for (auto x : a) s += _mm256_sad_epu8(_mm256_add_epi8(
            _mm256_shuffle_epi8(lut, x & m), _mm256_shuffle_epi8(lut, _mm256_srli_epi16(x, 4) & m)), zero);
        return u64(s[0]) + s[1] + s[2] + s[3];
    }
    bool none() const {
        for (auto x : a) if (!_mm256_testz_si256(x, x)) return 0;
        return 1;
    }
    bool all() const {
        for (auto x : a) if (!_mm256_testc_si256(x, ~zero)) return 0;
        return 1;
    }
    template<bool R>
    size_t find(size_t p) const {
        if constexpr (R) { if (++p >= N) return N; }
        else { p = min(p, N); if (!p--) return -1; }
        size_t i = p >> 8, j = p >> 6 & 3;
        u64 x = ((const u64a*)a)[p >> 6] & (R ? ~0ull << (p & 63) : ~0ull >> (~p & 63));
        if (!x) {
            unsigned m = mask(a[i]) & (R ? 0xEu << j : (1u << j) - 1);
            while (!m) {
                if constexpr (R) { if (++i == K) return N; }
                else if (!i--) return -1;
                m = mask(a[i]);
            }
            j = R ? countr_zero(m) : 31 - countl_zero(m);
            x = ((const u64a*)(a + i))[j];
        }
        return i * 256 + j * 64 + (R ? countr_zero(x) : 63 - countl_zero(x));
    }
    size_t _Find_next(size_t p) const { return find<1>(p); }
    size_t _Find_first() const { return find<1>(-1); }
    ptrdiff_t _Find_prev(size_t p) const { return find<0>(p); }
    ptrdiff_t _Find_last() const { return find<0>(N); }
};

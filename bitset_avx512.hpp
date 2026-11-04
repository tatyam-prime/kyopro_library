#pragma GCC optimize("Ofast,unroll-loops")
#include <immintrin.h>
using u64 = uint64_t;
using u64a = u64 __attribute__((may_alias));

// https://gist.github.com/tatyam-prime/00267996616494dc14feb9136c7570d7
template<size_t N> struct alignas(64) Bitset {
    static_assert(N && N % 512 == 0);
    static constexpr size_t K = N / 512;
    alignas(64) __m512i a[K];
    static inline const __m512i zero = _mm512_setzero_si512();
    void reset() {
        for (auto& x : a) x = zero;
    }
    bool get(size_t i) const { return ((const u64a*)a)[i >> 6] >> (i & 63) & 1; }
    bool operator[](size_t i) const { return get(i); }
    void set(size_t i, bool x = 1) {
        u64a& w = ((u64a*)a)[i >> 6];
        u64 m = 1ull << (i & 63);
        w = (w & ~m) | (-u64(x) & m);
    }
    template<unsigned F>  // F[x*2+y]
    static Bitset op(const Bitset& x, const Bitset& y) {
        return op<F | F << 4>(x, x, y);
    }
    template<unsigned F>  // F[x*4+y*2+z]
    static Bitset op(const Bitset& x, const Bitset& y, const Bitset& z) {
        static_assert(F < 256);
        Bitset r;
        for (size_t i = 0; i < K; ++i) r.a[i] = _mm512_ternarylogic_epi64(x.a[i], y.a[i], z.a[i], F);
        return r;
    }
#define BOP(c, f, intr) \
    friend Bitset operator c(const Bitset& x, const Bitset& y) { return op<f>(x, y); } \
    Bitset& operator c##=(const Bitset& y) { \
        for (size_t i = 0; i < K; ++i) a[i] = intr(a[i], y.a[i]); \
        return *this; \
    }
    BOP(&, 8, _mm512_and_si512)
    BOP(|, 14, _mm512_or_si512)
    BOP(^, 6, _mm512_xor_si512)
#undef BOP
    friend Bitset operator~(const Bitset& x) {
        return op<1>(x, x, x);
    }
    bool operator==(const Bitset& y) const {
        for (size_t i = 0; i < K; ++i)
            if (_mm512_cmpeq_epi64_mask(a[i], y.a[i]) != 0xFF) return false;
        return true;
    }
    // a[0], a[1], ... の辞書順 (0 < 1)
    strong_ordering operator<=>(const Bitset& y) const {
        for (size_t i = 0; i < K; ++i) {
            __mmask8 m = ~_mm512_cmpeq_epi64_mask(a[i], y.a[i]);
            if (!m) continue;
            size_t j = countr_zero((unsigned)m);
            u64 x = ((const u64a*)&a[i])[j], z = ((const u64a*)&y.a[i])[j];
            return (x & (x ^ z) & -(x ^ z)) ? strong_ordering::greater : strong_ordering::less;
        }
        return strong_ordering::equal;
    }
    static __m512i pm(__m512i x, __m512i i, __m512i y) {
        return _mm512_permutex2var_epi64(x, i, y);
    }
    template<bool R>
    static void shift(const Bitset& x, Bitset& z, size_t k) {
        if (k >= N) { z.reset(); return; }
        if (k == 0) { if (&x != &z) z = x; return; }
        size_t q = k >> 9, s = k >> 6 & 7, b = k & 63, m = K - q;
        if (!(s | b)) {
            if constexpr (R)
                for (size_t i = 0; i < m; ++i) z.a[i] = x.a[i + q];
            else
                for (size_t i = K; i-- > q;) z.a[i] = x.a[i - q];
        } else {
            __m512i id = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
            __m512i C = _mm512_set1_epi64(b);
            __m512i ia = _mm512_add_epi64(id, _mm512_set1_epi64(R ? s : 8 - s));
            __m512i ib = _mm512_add_epi64(ia, _mm512_set1_epi64(R ? 1 : -1));
            for (size_t t = 0; t < m; ++t) {
                size_t i = R ? t : K - 1 - t, j = R ? i + q : i - q;
                __m512i u = R ? x.a[j] : (j ? x.a[j - 1] : zero);
                __m512i v = R ? (j + 1 < K ? x.a[j + 1] : zero) : x.a[j];
                __m512i A = s ? pm(u, ia, v) : (R ? u : v);
                z.a[i] = !b ? A : R ? _mm512_shrdv_epi64(A, pm(u, ib, v), C)
                                   : _mm512_shldv_epi64(A, pm(u, ib, v), C);
            }
        }
        if constexpr (R)
            for (size_t i = m; i < K; ++i) z.a[i] = zero;
        else
            for (size_t i = 0; i < q; ++i) z.a[i] = zero;
    }
    template<bool R>
    Bitset sh(size_t k) const { Bitset z; shift<R>(*this, z, k); return z; }
    Bitset operator<<(size_t k) const { return sh<0>(k); }
    Bitset operator>>(size_t k) const { return sh<1>(k); }
    Bitset& operator<<=(size_t k) { shift<0>(*this, *this, k); return *this; }
    Bitset& operator>>=(size_t k) { shift<1>(*this, *this, k); return *this; }
    u64 count() const {
        __m512i s = zero;
        for (auto x : a) s = _mm512_add_epi64(s, _mm512_popcnt_epi64(x));
        return _mm512_reduce_add_epi64(s);
    }
    bool none() const {
        for (auto x : a)
            if (_mm512_test_epi64_mask(x, x)) return 0;
        return 1;
    }
    bool all() const {
        __m512i o = _mm512_set1_epi64(-1);
        for (auto x : a)
            if (_mm512_cmpeq_epi64_mask(x, o) != 0xFF) return 0;
        return 1;
    }
    size_t _Find_next(size_t p) const {
        if (++p >= N) return N;
        size_t i = p >> 9, j = p >> 6 & 7;
        auto q = (const u64a*)(a + i);
        if (uint64_t x = q[j] & (-1ULL << (p & 63))) return i * 512 + j * 64 + countr_zero(x);
        __mmask8 m = _mm512_test_epi64_mask(a[i], a[i]) & (__mmask8)(0xFE << j);
        while (!m && ++i < K) m = _mm512_test_epi64_mask(a[i], a[i]);
        if (!m) return N;
        j = countr_zero<uint8_t>(m);
        q = (const u64a*)(a + i);
        return i * 512 + j * 64 + countr_zero(q[j]);
    }
    size_t _Find_first() const { return _Find_next(-1); }
    ssize_t _Find_prev(size_t p) const {
        if (!p--) return -1;
        size_t i = p >> 9, j = p >> 6 & 7;
        auto q = (const u64a*)(a + i);
        if (uint64_t x = q[j] & (-1ULL >> (~p & 63))) return i * 512 + j * 64 + 63 - countl_zero(x);
        __mmask8 m = _mm512_test_epi64_mask(a[i], a[i]) & (__mmask8)((1 << j) - 1);
        while (!m && i--) m = _mm512_test_epi64_mask(a[i], a[i]);
        if (!m) return -1;
        j = 7 - countl_zero<uint8_t>(m);
        q = (const u64a*)(a + i);
        return i * 512 + j * 64 + 63 - countl_zero(q[j]);
    }
    ssize_t _Find_last() const { return _Find_prev(N); }
};

// using ll = long long; using ull = unsigned long long; using ld = long double;
namespace cpin {
struct eval_tag {};
template<class T> T read() { T x{}; cin >> x; return x; }
template<class F> struct GEN {
    F f;
    auto operator()(ll i = 0) const { return f(i); }
    auto operator=(eval_tag) const { return (*this)(); }
    template<class A> auto to(auto n) const {
        return cpin::GEN{[p = *this, n](ll) {
            ll size;
            if constexpr (requires { n(); }) size = n();
            else size = n;
            A a;
            if constexpr (requires { a.reserve(size); }) a.reserve(size);
            for (ll i = 0; i < size; ++i) a.insert(a.end(), p(i));
            return a;
        }};
    }
    template<template<class...> class A> auto to(auto n) const {
        return to<decltype(ranges::to<A>(declval<vector<decltype((*this)())>>() | views::as_rvalue))>(n);
    }
    auto operator[](auto n) const { return to<vector>(n); }
    auto operator-(auto d) const {
        return cpin::GEN{[p = *this, d](ll i) { return p(i) - d; }};
    }
};
#define CPI_TYPES(X) X(INT,int) X(LL,ll) X(STR,string) X(CHR,char) X(ULL,ull) X(DBL,double) X(LD,ld)
#define CPI_READER(x,...) [[maybe_unused]] const auto x = RD<__VA_ARGS__>;
template<class T> const auto RD = GEN{[](ll) { return read<T>(); }};
CPI_TYPES(CPI_READER)
[[maybe_unused]] const auto ID = GEN{[](ll i) { return i; }};
auto K(auto x) { return GEN{[x = move(x)](ll) { return x; }}; }
auto TP(auto... p) { return GEN{[=](ll i) { return tuple{p(i)...}; }}; }
auto ARR(auto... p) { return GEN{[=](ll i) {
    using T = common_type_t<decltype(p(i))...>;
    return array<T, sizeof...(p)>{T(p(i))...};
}}; }
#define FN(i,...) cpin::GEN{[&](ll i) { return (__VA_ARGS__); }}
#define CPI_E0(...) __VA_ARGS__
#define CPI_E1(...) CPI_E0(CPI_E0(CPI_E0(__VA_ARGS__)))
#define CPI_E2(...) CPI_E1(CPI_E1(CPI_E1(__VA_ARGS__)))
#define CPI_E3(...) CPI_E2(CPI_E2(CPI_E2(__VA_ARGS__)))
#define CPI_LP ()
#define CPI_NEXT() CPI_EACH
#define CPI_EACH(x,...) CPI_ITEM(x) __VA_OPT__(CPI_NEXT CPI_LP (__VA_ARGS__))
#define CPI_ITEM(x) auto x = [] { \
    if constexpr (string_view{#x}.contains('=')) return cpin::eval_tag{}; \
    else return cpin::LL(); \
}();
#define IN(...) __VA_OPT__(CPI_E3(CPI_EACH(__VA_ARGS__)))
}

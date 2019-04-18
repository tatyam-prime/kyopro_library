#include <bits/stdc++.h>
using namespace std;


inline int scan(){ return getchar(); }
inline void scan(int &a){ scanf("%d", &a); }
inline void scan(unsigned &a){ scanf("%u", &a); }
inline void scan(long &a){ scanf("%ld", &a); }
inline void scan(long long &a){ scanf("%lld", &a); }
inline void scan(unsigned long long &a){ scanf("%llu", &a); }
inline void scan(char &a){ scanf("%c", &a); }
inline void scan(float &a){ scanf("%f", &a); }
inline void scan(double &a){ scanf("%lf", &a); }
inline void scan(long double &a){ scanf("%Lf", &a); }
inline void scan(vector<bool> &vec){ for(unsigned i = 0; i < vec.size(); i++) { int a; scan(a); vec[i] = a; } }
template<class T> inline void scan(vector<T> &vec);
template<class T, size_t size> inline void scan(array<T, size> &vec);
template<class T, class L> inline void scan(pair<T, L> &p);
template<class T, size_t size> inline void scan(T (&vec)[size]);
template<class T> inline void scan(vector<T> &vec){ for(auto &i : vec) scan(i); }
template<class T, size_t size> inline void scan(array<T, size> &vec){ for(auto &i : vec) scan(i); }
template<class T, class L> inline void scan(pair<T, L> &p){ scan(p.first); scan(p.second); }
template<class T, size_t size> inline void scan(T (&vec)[size]){ for(auto &i : vec) scan(i); }
template<class T> inline void scan(T &a){ cin>>a; }
inline void in(){}
template <class Head, class... Tail> inline void in(Head &&head, Tail&&... tail){ scan(head); in(move(tail)...); }
inline void print(){ putchar('\n'); }
inline void print(const bool &a){ printf("%d", a); }
inline void print(const int &a){ printf("%d", a); }
inline void print(const unsigned &a){ printf("%u", a); }
inline void print(const long &a){ printf("%ld", a); }
inline void print(const long long &a){ printf("%lld", a); }
inline void print(const unsigned long long &a){ printf("%llu", a); }
inline void print(const char &a){ printf("%c", a); }
inline void print(const char a[]){ printf("%s", a); }
inline void print(const float &a){ printf("%.10f", a); }
inline void print(const double &a){ printf("%.10lf", a); }
inline void print(const long double &a){ printf("%.10Lf", a); }
template<class T> void print(const vector<T> &vec);
template<class T, size_t size> void print(const array<T, size> &vec);
template<class T, class L> void print(const pair<T, L> &p);
template<class T, size_t size> inline void print(const T (&vec)[size]);
template<class T> void print(const vector<T> &vec){ print(vec[0]); for(auto i = vec.begin(); ++i != vec.end(); ){ putchar(' '); print(*i); } }
template<class T, size_t size> void print(const array<T, size> &vec){ print(vec[0]); for(auto i = vec.begin(); ++i != vec.end(); ){ putchar(' '); print(*i); } }
template<class T, class L> void print(const pair<T, L> &p){ print(p.first); putchar(' '); print(p.second); }
template<class T, size_t size> inline void print(const T (&vec)[size]){ print(vec[0]); for(auto i = vec; ++i != end(vec); ){ putchar(' '); print(*i); } }
template<class T> inline void print(const T &a){ cout << a; }
inline bool out(){ putchar('\n'); return 0; }
template<class T> inline bool out(const T &t){ print(t); putchar('\n'); return 0; }
template<class Head, class... Tail> inline bool out(const Head &head, const Tail&... tail){ print(head); putchar(' '); out(tail...); return 0; }


int main(){ // 使用例
    int a[5];
    vector<vector<pair<int, int>>> b(2, vector<pair<int,int>>(2));
    double c;
    long long d;
    in(a, b, c, d);
    out(a, b, c, d);
}

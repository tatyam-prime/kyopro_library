#include <bits/stdc++.h>
using namespace std;


inline int scan(){ return getchar(); }
inline void scan(bool &a){ int b; scanf("%d", &b); a=b; }
inline void scan(int &a){ scanf("%d", &a); }
inline void scan(unsigned &a){ scanf("%u", &a); }
inline void scan(long &a){ scanf("%ld", &a); }
inline void scan(long long &a){ scanf("%lld", &a); }
inline void scan(unsigned long long &a){ scanf("%llu", &a); }
inline void scan(char &a){ scanf("%c", &a); }
inline void scan(float &a){ scanf("%f", &a); }
inline void scan(double &a){ scanf("%lf", &a); }
inline void scan(long double &a){ scanf("%Lf", &a); }
template<class T> inline istream &operator >> (istream &is, vector<T> &vec);
template<class T, size_t size> inline istream &operator >> (istream &is, array<T, size> &vec);
template<class T, class L> inline istream &operator >> (istream &is, pair<T, L> &p);
template<class T, size_t size> inline istream &operator >> (istream &is, T (&vec)[size]);
template<class T> inline istream &operator >> (istream &is, vector<T> &vec){ for(auto &i : vec) scan(i); return is; }
template<class T, size_t size> inline istream &operator >> (istream &is, array<T, size> &vec){ for(auto &i : vec) scan(i); return is; }
template<class T, class L> inline istream &operator >> (istream &is, pair<T, L> &p){ scan(p.first); scan(p.second); return is; }
template<class T, size_t size> inline istream &operator >> (istream &is, T (&vec)[size]){ for(auto &i : vec) scan(i); return is; }
template<class T> inline void scan(T &a){ cin>>a; }
inline void in(){ }
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
inline void print(const float &a){ printf("%f", a); }
inline void print(const double &a){ printf("%lf", a); }
inline void print(const long double &a){ printf("%Lf", a); }
template<class T> inline ostream &operator << (ostream &os, const vector<T> &vec);
template<class T, size_t size> inline ostream &operator << (ostream &os, const array<T, size> &vec);
template<class T, class L> inline ostream &operator << (ostream &os, const pair<T, L> &p);
template<class T, size_t size> inline typename enable_if<is_same<T, const char>::value == 0, ostream>::type &operator << (ostream &os, const T (&vec)[size]);
template<class T> inline ostream &operator << (ostream &os, const vector<T> &vec){ print(vec[0]); for(auto i=vec.begin(); ++i != vec.end(); ){ putchar(' '); print(*i); }return os; }
template<class T, size_t size> inline ostream &operator << (ostream &os, const array<T, size> &vec){ print(vec[0]); for(auto i=vec.begin(); ++i!=vec.end(); ){ putchar(' '); print(*i); }return os; }
template<class T, class L> inline ostream &operator << (ostream &os, const pair<T, L> &p){ print(p.first); putchar(' '); print(p.second); return os; }
template<class T, size_t size> inline typename enable_if<is_same<T, const char>::value == 0, ostream>::type &operator << (ostream &os, const T (&vec)[size]){ print(vec[0]); for(auto i=vec; ++i != end(vec); ){ putchar(' '); print(*i); }return os; }
template<class T> inline void print(const T &a){ cout<<a; }
template<class T> inline bool out(const T &t){ print(t); putchar('\n'); return 0; }
inline bool out(){ putchar('\n'); return 0; }
template<class Head, class... Tail> inline bool out(const Head &head, const Tail&... tail){ print(head); print(' '); out(tail...); return 0; }


int main(){ // 使用例
    int a[5];
    vector<vector<pair<int, int>>> b(2, vector<pair<int,int>>(2));
    double c;
    long long d;
    in(a,b,c,d);
    out(a,b,c,d);
}

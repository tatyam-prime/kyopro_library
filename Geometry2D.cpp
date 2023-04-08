template<class T> using P = pair<T, T>;
#define x first
#define y second
template<class T> P<T> operator+(const P<T>& a, const P<T>& b) { return {a.x + b.x, a.y + b.y}; }
template<class T> P<T> operator-(const P<T>& a, const P<T>& b) { return {a.x - b.x, a.y - b.y}; }
template<class T> P<T> operator-(const P<T>& a) { return {-a.x, -a.y}; }
template<class T, class U> P<T> operator*(const P<T>& a, const U& b) { return {a.x * b, a.y * b}; }
template<class T, class U> P<T> operator/(const P<T>& a, const U& b) { return {a.x / b, a.y / b}; }
template<class T> P<T>& operator+=(P<T>& a, const P<T>& b) { return a = a + b; }
template<class T> P<T>& operator-=(P<T>& a, const P<T>& b) { return a = a - b; }
template<class T, class U> P<T>& operator*=(P<T>& a, const U& b) { return a = a * b; }
template<class T, class U> P<T>& operator/=(P<T>& a, const U& b) { return a = a / b; }
template<class T> P<T> rotate(const P<T>& a) { return {-a.y, a.x}; } // 90 degree ccw
template<class T> T dot(const P<T>& a, const P<T>& b) { return a.x * b.x + a.y * b.y; }
template<class T> T cross(const P<T>& a, const P<T>& b) { return dot(rotate(a), b); }
template<class T> T square(const P<T>& a) { return dot(a, a); }
template<class T> ld abs(const P<T>& a) { return hypotl(a.x, a.y); }
template<class T> T gcd(const P<T>& a) { return gcd(a.x, a.y); }
template<class T> P<T> normalize(P<T> a) {
    if(a == P<T>{}) return a;
    a /= gcd(a);
    if(a < P<T>{}) a = -a;
    return a;
}

using Real = ld;
using R = ll;
struct Point {
    using P = Point;
    using Pair = pair<R, R>;
    R x = 0, y = 0;
    Point(){}
    Point(R x, R y): x(x), y(y){}
    Point(Pair a): x(a.first), y(a.second){}
    operator Pair() const { return {x, y}; }
    friend R dot(P a, P b) { return a.x * b.x + a.y * b.y; }
    friend R square(P a) { return dot(a, a); }
    friend R cross(P a, P b) { return a.x * b.y - a.y * b.x; }
    friend Real abs(P a) { return hypot((Real)a.x, (Real)a.y); }
    friend Real distance(P a, P b) { return abs(a - b); }
    R gcd() { return std::gcd(x, y); }
    P& operator+=(P b) { x += b.x; y += b.y; return *this; }
    P& operator-=(P b) { x -= b.x; y -= b.y; return *this; }
    P& operator*=(R b) { x *= b; y *= b; return *this; }
    P& operator/=(R b) { x /= b; y /= b; return *this; }
    friend P operator+(P a, P b) { return a += b; }
    friend P operator-(P a, P b) { return a -= b; }
    friend P operator*(P a, R b) { return a *= b; }
    friend P operator/(P a, R b) { return a /= b; }
    P operator-() const { return {-x, -y}; }
    P rotate() const { return {-y, x}; } // 90 度回転
    friend bool operator==(P a, P b) { return Pair(a) == Pair(b); }
    friend bool operator!=(P a, P b) { return Pair(a) != Pair(b); }
    friend bool operator<(P a, P b) { return Pair(a) < Pair(b); }
    friend bool operator>(P a, P b) { return Pair(a) > Pair(b); }
    friend istream& operator>>(istream& in, P& a) { return in >> a.x >> a.y; }
    friend ostream& operator>>(ostream& out, P& a) { return out << a.x << ' ' << a.y; }
    friend P normalize(P a) {
        if(a == P{}) return a;
        a /= a.gcd();
        if(a < P{}) a = -a;
        return a;
    }
    friend int ccw(P a, P b, P c) {
        b -= a; c -= a;
        if(cross(b, c) > 0) return 1;
        if(cross(b, c) < 0) return -1;
        if(dot(b, c) < 0) return 2;
        if(square(b) < square(c)) return -2;
        return 0;
    }
};
struct Line { Point a, b; };
struct Segment : Line {};
bool parallel(const Line &a, const Line &b) { return cross(a.b - a.a, b.b - b.a) == 0; }
bool orthogonal(const Line &a, const Line &b) { return dot(a.a - a.b, b.a - b.b) == 0; }
bool intersect(const Line &l, Point p) { return abs(ccw(l.a, l.b, p)) != 1; }
bool intersect(const Line &l, const Line &m) { return abs(cross(l.b - l.a, m.b - m.a)) > 0 || abs(cross(l.b - l.a, m.b - l.a)) == 0; }
bool intersect(const Segment &s, Point p) {return ccw(s.a, s.b, p) == 0; }
bool intersect(const Line &l, const Segment &s) { return cross(l.b - l.a, s.a - l.a) * cross(l.b - l.a, s.b - l.a) == 0; }
bool intersect(const Segment &s, const Segment &t) { return ccw(s.a, s.b, t.a) * ccw(s.a, s.b, t.b) <= 0 && ccw(t.a, t.b, s.a) * ccw(t.a, t.b, s.b) <= 0; }

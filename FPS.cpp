#include <atcoder/convolution>
using Modint = atcoder::modint998244353;
struct Poly{
    vector<Modint> a;
    Poly(){}
    Poly(ll n): a(n){}
    Poly(const vector<Modint>& a): a(a){}
    Poly(vector<Modint>&& a): a(a){}
    Modint& operator[](ll x){ return a[x]; }
    Modint operator[](ll x) const { return a[x]; }
    auto begin(){ return a.begin(); }
    auto end(){ return a.end(); }
    auto begin() const { return a.cbegin(); }
    auto end() const { return a.cend(); }
    ll size() const { return a.size(); }
    Poly& resize(ll n){ a.resize(n); return *this; }
    Poly& resize(const Poly& f){ if(size() < f.size()) resize(f.size()); return *this; }
    Poly& operator+=(const Poly& f){
        resize(f);
        for(ll i = 0; i < f.size(); i++) a[i] += f[i];
        return *this;
    }
    Poly operator+(const Poly& f) const {
        if(size() < f.size()) return Poly(f) += *this;
        return Poly(*this) += f;
    }
    Poly& operator-=(const Poly& f){
        resize(f);
        for(ll i = 0; i < f.size(); i++) a[i] -= f[i];
        return *this;
    }
    Poly operator-(const Poly& f) const { return Poly(*this) -= f; }
    Poly operator-(int) const {
        Poly ans(size());
        for(ll i = 0; i < size(); i++) ans[i] = -a[i];
        return ans;
    }
    Poly operator*(const Poly& f) const { return atcoder::convolution(a, f.a); }
    Poly& operator*=(const Poly& f){ a = atcoder::convolution(move(a), vector(f.a)); return *this; }
    Poly inv(ll n = -1) const {
        if(n == -1) n = size();
        if(n == 0) return {};
        assert(size() && a[0] != 0);
        vector<Modint> ans = {a[0].inv()};
        for(ll m = 1; m < n; m *= 2) {
            const Modint M = Modint(m * m * 4).inv();
            vector<Modint> x(a.begin(), a.begin() + min(size(), m * 2)), y = ans;
            x.resize(m * 2); atcoder::internal::butterfly(x);
            y.resize(m * 2); atcoder::internal::butterfly(y);
            for(ll i = 0; i < m * 2; ++i) x[i] *= y[i];
            atcoder::internal::butterfly_inv(x);
            for(ll i = 0; i < m; i++) x[i] = 0;
            for(ll i = m; i < m * 2; i++) x[i] *= M;
            atcoder::internal::butterfly(x);
            for(ll i = 0; i < m * 2; ++i) x[i] *= -y[i];
            atcoder::internal::butterfly_inv(x);
            ans.insert(ans.end(), x.begin() + m, x.end());
        }
        ans.resize(n);
        return ans;
    }
    Poly operator/=(const Poly& f){
        const ll n = size();
        *this *= f.inv(n);
        return resize(n);
    }
    Poly operator/(const Poly& f) const { return Poly(*this) /= f; }
    Poly div(const Poly& f) const {
        assert(f.size() && f.a.back() != 0);
        Poly x = *this, y = f;
        while(x.size() && !x.a.back().val()) x.a.pop_back();
        while(y.size() && !y.a.back().val()) y.a.pop_back();
        if(x.size() < f.size()) return {};
        const ll n = x.size() - y.size() + 1;
        reverse(x.begin(), x.end()); x.resize(n);
        reverse(y.begin(), y.end()); y.resize(n);
        x /= y;
        reverse(x.begin(), x.end());
        return x;
    }
    Poly operator%(const Poly& f) const {
        assert(f.size() && f.a.back() != 0);
        return (*this - f * div(f)).resize(f.size() - 1);
    }
    pair<Poly, Poly> divmod(const Poly& f) const {
        assert(f.size() && f.a.back() != 0);
        const Poly D = div(f);
        return {D, (*this - f * D).resize(f.size() - 1)};
    }
    Poly D(ll n = -1) const {
        if(n == -1) n = size();
        if(n == 0) return {};
        Poly ans(size() - 1);
        for(ll i = 1; i < size(); i++) ans[i - 1] = a[i] * Modint::raw(i);
        return ans;
    }
    Poly integral() const {
        const ll n = size();
        Poly ans(n + 1);
        ans[0] = 1;
        for(ll i = 0; i < n; i++) ans[i + 1] = ans[i] * Modint::raw(i + 1);
        ans[n] = ans[n].inv();
        for(ll i = n; i--; ){
            swap(ans[i], ans[i + 1]);
            ans[i + 1] *= ans[i] * a[i];
            ans[i] *= Modint::raw(i + 1);
        }
        ans[0] = 0;
        return ans;
    }
    Poly log(ll n = -1) const {
        if(n == -1) n = size();
        if(n == 0) return {};
        assert(size() && a[0] == 1);
        return (D(n) * inv(n)).resize(n - 1).integral();
    }
    Poly exp(ll n = -1) const {
        if(n == -1) n = size();
        if(n == 0) return {};
        assert(a[0] == 0);
        if(size() == 1) return vector<Modint>{1};
        vector<Modint> b = {1, a[1]}, c = {1}, z1, z2 = {1, 1};
        for(ll m = 2; m < n; m *= 2) {
            const Modint M = Modint(m).inv(), M2 = Modint(m * 2).inv();
            auto y = b;
            y.resize(m * 2); atcoder::internal::butterfly(y);
            z1 = move(z2);
            vector<Modint> z(m);
            for(ll i = 0; i < m; i++) z[i] = y[i] * z1[i];
            atcoder::internal::butterfly_inv(z);
            for(ll i = 0; i < m / 2; i++) z[i] = 0;
            for(ll i = m / 2; i < m; i++) z[i] *= M * M;
            atcoder::internal::butterfly(z);
            for(ll i = 0; i < m; i++) z[i] *= -z1[i];
            atcoder::internal::butterfly_inv(z);
            c.insert(c.end(), z.begin() + m / 2, z.end());
            z2 = c; z2.resize(m * 2);
            atcoder::internal::butterfly(z2);
            Poly x = vector<Modint>(begin(), begin() + min(size(), m));
            x = x.D(); x.a.push_back(0);
            atcoder::internal::butterfly(x.a);
            for(ll i = 0; i < m; i++) x[i] *= y[i] * M;
            atcoder::internal::butterfly_inv(x.a);
            x -= Poly(b).D();
            x.resize(m * 2);
            for(ll i = 0; i < m - 1; i++){
                x[m + i] = x[i]; x[i] = 0;
            }
            atcoder::internal::butterfly(x.a);
            for (ll i = 0; i < m * 2; i++) x[i] *= z2[i] * M2;
            atcoder::internal::butterfly_inv(x.a);
            x = x.integral(); x.a.pop_back();
            for(ll i = m; i < min(size(), m * 2); i++) x[i] += a[i];
            for(ll i = 0; i < m; i++) x[i] = 0;
            for(ll i = m; i < m * 2; i++) x[i] *= M2;
            atcoder::internal::butterfly(x.a);
            for(ll i = 0; i < m * 2; ++i) x[i] *= y[i];
            atcoder::internal::butterfly_inv(x.a);
            b.insert(b.end(), x.begin() + m, x.end());
        }
        return Poly(move(b)).resize(n);
    }
    Poly pow(ll k, ll n = -1){
        if(n == -1) n = size();
        if(n == 0) return {};
        assert(k >= 0);
        if(k == 0){
            Poly ans(n);
            ans[0] = 1;
            return ans;
        }
        Poly ans = *this;
        ll cnt = 0;
        while(cnt < size() && ans[cnt] == 0) cnt++;
        if(cnt == size() || cnt * k >= n) return Poly(n);
        ans.a.erase(ans.a.begin(), ans.a.begin() + cnt);
        const Modint c = ans[0], C = c.pow(k), D = c.inv(), K = k;
        for(Modint& i : ans) i *= D;
        ans = ans.log(n - cnt * k);
        for(Modint& i : ans) i *= K;
        ans = ans.exp();
        for(Modint& i : ans) i *= C;
        ans.a.insert(ans.a.begin(), cnt * k, 0);
        return ans;
    }
};

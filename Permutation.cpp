// 順列の掛け算と累乗
using ll = long long;


struct Permutation { // 0-indexed
    vector<ll> perm;
    Permutation(initializer_list<ll>&& perm): perm(perm) {}
    Permutation(vector<ll>&& perm): perm(perm) {}
    template<class T> Permutation(const initializer_list<T>& perm): perm(perm.begin(), perm.end()) {}
    template<class T> Permutation(const vector<T>& perm): perm(perm.begin(), perm.end()) {}
    static Permutation identity(ll n) { vector<ll> ans(n); iota(ans.begin(), ans.end(), 0); return ans; }
    Permutation inv() const { vector<ll> ans(perm.size()); for(ll i = 0; i < perm.size(); i++) ans[perm[i]] = i; return ans; }
    Permutation operator* (const Permutation& x) const { vector<ll> ans(perm.size()); for(ll i = 0; i < perm.size(); i++) ans[i] = perm[x.perm[i]]; return ans; }
    Permutation& operator*=(const Permutation& x) { vector<ll> ans(perm.size()); for(ll i = 0; i < perm.size(); i++) ans[i] = perm[x.perm[i]]; perm.swap(ans); return *this; }
    Permutation pow(ll x) const {
        Permutation ans = identity(perm.size()), cnt = *this;
        while(x){
            if(x & 1) ans *= cnt;
            x /= 2;
            cnt *= cnt;
        }
        return ans;
    }
};

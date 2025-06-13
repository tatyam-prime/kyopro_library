template<class S, auto op, auto e>
class UnionFindMonoid {
    static_assert(std::is_convertible_v<decltype(op), std::function<S(S, S)>>,
                  "op must work as S(S, S)");
    static_assert(std::is_convertible_v<decltype(e), std::function<S()>>,
                  "e must work as S()");
    int _n;
    std::vector<int> parent_or_size;
    std::vector<S> data;
public:
    UnionFindMonoid(int n = 0) : _n(n), parent_or_size(n, -1), data(n, e()) {}
    UnionFindMonoid(const std::vector<S>& a) : _n((int)a.size()), parent_or_size(a.size(), -1), data(a) {}
    
    int leader(int a) {
        assert(0 <= a && a < _n);
        while (true) {
            const int b = parent_or_size[a];
            if (b < 0) return a;
            const int c = parent_or_size[b];
            if (c < 0) return b;
            a = parent_or_size[a] = c; // path halving
        }
    }
    bool is_leader(int a) const {
        assert(0 <= a && a < _n);
        return parent_or_size[a] < 0;
    }
    int merge(int a, int b) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        a = leader(a);
        b = leader(b);
        if (a == b) return a;
        if (-parent_or_size[a] < -parent_or_size[b]) swap(a, b);
        parent_or_size[a] += parent_or_size[b];
        parent_or_size[b] = a;
        data[a] = op(data[a], data[b]);
        return a;
    }
    bool same(int a, int b) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        return leader(a) == leader(b);
    }
    int size(int a) {
        assert(0 <= a && a < _n);
        return -parent_or_size[leader(a)];
    }
    S get(int a) {
        assert(0 <= a && a < _n);
        return data[leader(a)];
    }
    void set(int a, S x) {
        assert(0 <= a && a < _n);
        data[leader(a)] = x;
    }
};

// 動的に凸法を管理する
struct Polygon {
    map<ll, ll> lower{{0, 0}}, upper{{0, 0}};
    ll size() const {
        return lower.size() + upper.size() - (*lower.begin() == *upper.begin()) - (*lower.rbegin() == *upper.rbegin());
    }
    void add(ll x, ll y) {
        {
            auto can_erase = [&](map<ll, ll>::iterator p) -> bool {
                if(p == lower.begin()) return 0;
                if(next(p) == lower.end()) return 0;
                return cross(*next(p) - *p, *prev(p) - *p) <= 0;
            };
            auto [p, f] = lower.try_emplace(x, y);
            chmin(p->second, y);
            if(can_erase(p)) {
                lower.erase(p);
                return;
            }
            while(p != lower.begin() && can_erase(prev(p))) lower.erase(prev(p));
            while(next(p) != lower.end() && can_erase(next(p))) lower.erase(next(p));
        }
        {
            auto can_erase = [&](map<ll, ll>::iterator p) -> bool {
                if(p == upper.begin()) return 0;
                if(next(p) == upper.end()) return 0;
                return cross(*prev(p) - *p, *next(p) - *p) <= 0;
            };
            auto [p, f] = upper.try_emplace(x, y);
            chmax(p->second, y);
            if(can_erase(p)) {
                upper.erase(p);
                return;
            }
            while(p != upper.begin() && can_erase(prev(p))) upper.erase(prev(p));
            while(next(p) != upper.end() && can_erase(next(p))) upper.erase(next(p));
        }
    }
};

struct SWAG{
    using T = <#ll#>;
    inline T f(T a, T b) const {
        return <#gcd(a, b)#>;
    }
    const T def_value = <#-LINF#>;
    ll size = 0;
    vector<T> fold_l, r;
    T fold_r;
    T get() const {
        if(r.empty()){
            if(fold_l.empty()) return def_value;
            return fold_l.back();
        }
        if(fold_l.empty()) return fold_r;
        return f(fold_l.back(), fold_r);
    }
    void push(T a){
        if(r.empty()) fold_r = a;
        else fold_r = f(fold_r, a);
        r.push_back(a);
        size++;
    }
    void pop(){
        if(fold_l.empty()){
            for(ll i = size; i--; ) fold_l.push_back(r[i]);
            r.clear();
            for(ll i = 1; i < size; i++) fold_l[i] = f(fold_l[i], fold_l[i - 1]);
        }
        fold_l.pop_back();
        size--;
    }
};

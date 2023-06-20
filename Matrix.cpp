template<class T, size_t N, size_t M = N>
struct Matrix{
    array<array<T, M>, N> a;
    Matrix(){
        for(auto& i : a) i.fill(T{});
    }
    Matrix(const array<array<T, M>, N>& a): a(a){}
    Matrix(const vector<vector<T>>& b){
        for(size_t i = 0; i < b.size(); i++) for(size_t j = 0; j < b[i].size(); j++) a[i][j] = b[i][j];
    }
    static Matrix e(){
        static_assert(N == M, "this matrix is not square.");
        Matrix a;
        for(size_t i = 0; i < N; i++) a[i][i] = T{1};
        return a;
    }
    auto& operator[](size_t i){ return a[i]; }
    auto& operator[](size_t i) const { return a[i]; }
    auto begin(){ return a.begin(); }
    auto begin() const { return a.begin(); }
    auto end(){ return a.end(); }
    auto end() const { return a.end(); }
    bool operator==(const Matrix& b){ return a == b.a; }
    bool operator!=(const Matrix& b){ return a != b.a; }
    Matrix& operator+=(const Matrix& b){
        for(size_t i = 0; i < N; i++) for(size_t j = 0; j < M; j++) a[i][j] += b[i][j];
        return *this;
    }
    Matrix operator+(const Matrix& b) const {
        return Matrix(*this) += b;
    }
    Matrix& operator-=(const Matrix& b){
        for(size_t i = 0; i < N; i++) for(size_t j = 0; j < M; j++) a[i][j] -= b[i][j];
        return *this;
    }
    Matrix operator-(const Matrix& b) const {
        return Matrix(*this) -= b;
    }
    template<size_t K> Matrix<T, N, K> operator*(const Matrix<T, M, K>& b){
        Matrix<T, N, K> c;
        for(size_t j = 0; j < M; j++) for(size_t i = 0; i < N; i++) for(size_t k = 0; k < K; k++) c[i][k] += a[i][j] * b[j][k];
        return c;
    }
    Matrix& operator*=(const Matrix<T, M, M>& b){
        return *this = *this * b;
    }
    Matrix pow(long long r) const {
        if(r == 0) return e();
        if(r < 0) return inv().pow(-r);
        Matrix ans = e(), A = *this;
        while(1){
            if(r & 1) ans *= A;
            if(r == 1) return ans;
            A *= A;
            r >>= 1;
        }
    }
    Matrix inv() const {
        Matrix A = *this, B = e();
        for(size_t i = 0; i < N; i++){
            if(A[i][i] == 0) for(size_t j = i + 1; j < N; j++) if(A[j][i] != 0){
                swap(A[i], A[j]);
                swap(B[i], B[j]);
                break;
            }
            assert("this matrix is not regular." && A[i][i] != 0);
            const T x = T{1} / A[i][i];
            for(size_t k = i; k < N; k++) A[i][k] *= x;
            for(size_t k = 0; k < N; k++) B[i][k] *= x;
            for(size_t j = 0; j < N; j++) if(i != j){
                const T x = A[j][i];
                for(size_t k = i; k < N; k++) A[j][k] -= A[i][k] * x;
                for(size_t k = 0; k < N; k++) B[j][k] -= B[i][k] * x;
            }
        }
        return B;
    }
    T det() const {
        Matrix A = *this;
        T ans = 1;
        for(size_t i = 0; i < N; i++){
            if(A[i][i] == 0) for(size_t j = i + 1; j < N; j++) if(A[j][i] != 0){
                swap(A[i], A[j]);
                ans = -ans;
                break;
            }
            if(A[i][i] == 0) return 0;
            ans *= A[i][i];
            const T x = T{1} / A[i][i];
            for(size_t k = i; k < N; k++) A[i][k] *= x;
            for(size_t j = i + 1; j < N; j++){
                const T x = A[j][i];
                for(size_t k = i; k < N; k++) A[j][k] -= A[i][k] * x;
            }
        }
        return ans;
    }
};





template<class T>
struct Matrix{
    size_t N, M;
    vector<T> a;
    Matrix(size_t N, size_t M): N(N), M(M), a(N * M){}
    Matrix(const vector<vector<T>>& b): Matrix(b.size(), b[0].size()){
        for(size_t i = 0; i < N; i++) for(size_t j = 0; j < M; j++) *this[i][j] = b[i][j];
    }
    static Matrix e(size_t N){
        Matrix a(N, N);
        for(size_t i = 0; i < N; i++) a[i][i] = T{1};
        return a;
    }
    auto operator[](size_t i){ return a.begin() + i * M; }
    auto operator[](size_t i) const { return a.begin() + i * M; }
    bool operator==(const Matrix& b){ return a == b.a; }
    bool operator!=(const Matrix& b){ return a != b.a; }
    Matrix& operator+=(const Matrix& b){
        for(size_t i = 0; i < N * M; i++) a[i] += b[i];
        return *this;
    }
    Matrix operator+(const Matrix& b) const {
        return Matrix(*this) += b;
    }
    Matrix& operator-=(const Matrix& b){
        for(size_t i = 0; i < N * M; i++) a[i] -= b[i];
        return *this;
    }
    Matrix operator-(const Matrix& b) const {
        return Matrix(*this) -= b;
    }
    Matrix operator*(const Matrix& b){
        assert(M == b.N);
        const size_t K = b.M;
        Matrix c(N, K);
        for(size_t j = 0; j < M; j++) for(size_t i = 0; i < N; i++) for(size_t k = 0; k < K; k++) c[i][k] += *this[i][j] * b[j][k];
        return c;
    }
    Matrix& operator*=(const Matrix& b){
        return *this = *this * b;
    }
    Matrix pow(long long r) const {
        if(r == 0) return e(N);
        if(r < 0) return inv().pow(-r);
        Matrix ans = e(N), A = *this;
        while(1){
            if(r & 1) ans *= A;
            if(r == 1) return ans;
            A *= A;
            r >>= 1;
        }
    }
    Matrix inv() const {
        Matrix A = *this, B = e(N);
        for(size_t i = 0; i < N; i++){
            if(A[i][i] == 0) for(size_t j = i + 1; j < N; j++) if(A[j][i] != 0){
                for(size_t k = i; k < N; k++) swap(A[i][k], A[j][k]);
                for(size_t k = 0; k < N; k++) swap(B[i][k], B[j][k]);
                break;
            }
            assert("this matrix is not regular." && A[i][i] != 0);
            const T x = T{1} / A[i][i];
            for(size_t k = i; k < N; k++) A[i][k] *= x;
            for(size_t k = 0; k < N; k++) B[i][k] *= x;
            for(size_t j = 0; j < N; j++) if(i != j){
                const T x = A[j][i];
                for(size_t k = i; k < N; k++) A[j][k] -= A[i][k] * x;
                for(size_t k = 0; k < N; k++) B[j][k] -= B[i][k] * x;
            }
        }
        return B;
    }
    T det() const {
        Matrix A = *this;
        T ans = 1;
        for(size_t i = 0; i < N; i++){
            if(A[i][i] == 0) for(size_t j = i + 1; j < N; j++) if(A[j][i] != 0){
                for(size_t k = i; k < N; k++) swap(A[i][k], A[j][k]);
                ans = -ans;
                break;
            }
            if(A[i][i] == 0) return 0;
            ans *= A[i][i];
            const T x = T{1} / A[i][i];
            for(size_t k = i; k < N; k++) A[i][k] *= x;
            for(size_t j = i + 1; j < N; j++){
                const T x = A[j][i];
                for(size_t k = i; k < N; k++) A[j][k] -= A[i][k] * x;
            }
        }
        return ans;
    }
};

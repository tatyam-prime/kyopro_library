# ジェネレータと IN マクロ

[cpin.cpp](./cpin.cpp) は、ジェネレータから多次元 vector を簡単に作成するための class `cpin::GEN` と，入力を行うジェネレータ群を提供します．
`IN(...)` はジェネレータを呼び出し、その結果で変数を宣言するマクロです。入力を読む用途にも、定数や計算結果からデータを作る用途にも使えます。ジェネレータは `()` で直接呼び出すこともできます。

## 使用例

```cpp
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    IN(n, a = LL[n]);
    cout << accumulate(a.begin(), a.end(), 0LL) << '\n';
}
```

- `IN(n)`：`long long` の変数 `n` を宣言して、1 個読み込む。
- `IN(a = LL[n])`：`long long` を `n` 個読み込み、`vector<long long>` の変数 `a` を宣言する。
- `IN(n, a = LL[n])`：左から順に処理するので、先に読んだ `n` を配列の長さに使える。

実装は C++23 の `string_view::contains`、`ranges::to`、`views::as_rvalue` を使っています。これらに対応した標準ライブラリと、`-std=c++23` が必要です。

## 1. 値を 1 個ずつ読む

`IN` の各項目は、次のどちらかの形で書きます。

```cpp
IN(n, m);                   // n, m はどちらも long long
IN(k = INT, s = STR);        // k は int、s は string
IN(x = DBL, y = LD, c = CHR);
```

`名前` だけなら `long long`、`名前 = ジェネレータ` ならジェネレータの返す型になります。通常の変数宣言と同じく、末尾に `;` を付けます。

| ジェネレータ | 読み込む型 |
| --- | --- |
| `INT` | `int` |
| `LL` | `long long`（`ll`） |
| `ULL` | `unsigned long long`（`ull`） |
| `DBL` | `double` |
| `LD` | `long double`（`ld`） |
| `STR` | `string` |
| `CHR` | `char` |
| `RD<T>` | 指定した型 `T` |

`RD<T>` は `cin >> x` で入力できる型に使えます。例えば `IN(x = RD<short>);` と書けます。

読み込みには `cin >>` を使うため、改行も空白も区切りとして扱います。`STR` は空白を含まない 1 語を読み、`CHR` は通常、空白を飛ばして 1 文字読みます。

## 2. 配列・行列を作る

### 一次元配列

ジェネレータの後ろに `[個数]` を付けると、その結果を並べた `vector` を作ります。

```cpp
IN(n, a = INT[n], words = STR[n]);
// a: vector<int>
// words: vector<string>
```

入力:

```text
3
10 20 30
apple banana cherry
```

結果は `a = {10, 20, 30}`、`words = {"apple", "banana", "cherry"}` です。`a` の全要素を読み終わってから `words` を読みます。

### 二次元配列

```cpp
IN(h, w, a = LL[w][h]);
// a: vector<vector<long long>>、h 行 × w 列
```

入力:

```text
2 3
1 2 3
4 5 6
```

結果は `a = {{1, 2, 3}, {4, 5, 6}}` です。

**`LL[w][h]` は「長さ `w` の配列を `h` 個作る」という順番です。** 右側の `[h]` が外側の配列になり、`a[i][j]` で i 行 j 列を参照します。三次元なら `LL[w][h][d]` が `d × h × w` になります。

文字のグリッドが空白なしの行として与えられる場合は、次の形も便利です。

```cpp
IN(h, w, grid = STR[h]);
// 例: 入力が 2 3 / abc / def なら grid = {"abc", "def"}
```

この場合、各文字列の長さが `w` かどうかは検査しません。

### 長さ自体を入力から読む

`[個数]` には、数値の代わりにジェネレータも指定できます。その場合、配列を作るたびにジェネレータを呼び出して個数を決めます。`INT` を指定すれば、まず個数を入力から読みます。

```cpp
IN(a = LL[INT]);
// まず int の長さを 1 個読み、その個数だけ long long を読む
```

行ごとに長さが違う配列も、そのまま表現できます。

```cpp
IN(n, rows = LL[INT][n]);
```

入力:

```text
3
2 10 20
1 30
3 40 50 60
```

結果は `rows = {{10, 20}, {30}, {40, 50, 60}}` です。各行の先頭の整数を、その行の長さとして使います。

## 3. 生成した値から引く

`ジェネレータ - 値` は、生成した値から指定した値を引いて返すジェネレータです。入力では、1-indexed の頂点番号や添字を 0-indexed に直すときに便利です。

```cpp
IN(v = INT - 1);
IN(n, a = (INT - 1)[n]);
```

2 行目に対する入力が `3 1 3 2` なら、`a = {0, 2, 1}` になります。

配列では **`(INT - 1)[n]` のように、各要素を変換してから配列にします。** `INT[n] - 1` は `vector` 自体から `1` を引こうとするため使えません。

## 4. 複数の値を 1 組にする

### `TP(...)`：異なる型も使える tuple

```cpp
IN(record = TP(INT, STR, DBL));
// record: tuple<int, string, double>
auto [id, name, score] = record;
```

`7 alice 9.5` を入力すると、`id = 7`、`name = "alice"`、`score = 9.5` になります。組の中も、指定した左から順に読みます。

`[個数]` と組み合わせると、「1 組ずつ読む配列」になります。例えば重み付き辺なら次のように書けます。

```cpp
IN(m, edges = TP(INT - 1, INT - 1, LL)[m]);
// edges: vector<tuple<int, int, long long>>
for (auto [u, v, cost] : edges) {
    // u, v は 0-indexed
}
```

入力:

```text
2
1 3 10
2 3 20
```

結果は `edges = {(0, 2, 10), (1, 2, 20)}` です。

### `ARR(...)`：固定長の array

```cpp
IN(n, points = ARR(LL, LL)[n]);
// points: vector<array<long long, 2>>
// 各点を x, y の順に読み、points[i][0], points[i][1] で参照する
```

`ARR` は `std::array` を返します。要素の型が異なる場合は `common_type_t` で共通の型にそろえるので、例えば `ARR(INT, LL)` は `array<long long, 2>` になります。

## 5. 添字・定数・独自の変換を組み合わせる

| ジェネレータ | 返す値 | 標準入力を読むか |
| --- | --- | --- |
| `ID` | 繰り返し中の添字（`long long`、0 始まり） | 読まない |
| `K(x)` | 指定した値 `x` | 読まない |
| `FN(i, 式)` | 添字 `i` を使って計算した式の結果 | 式による |

```cpp
IN(n, a = TP(LL, ID)[n]);
// 入力が 3 50 20 50 なら a = {(50, 0), (20, 1), (50, 2)}
```

```cpp
IN(zeros = K(0)[5]);
// zeros: vector<int> = {0, 0, 0, 0, 0}。入力は消費しない
```

```cpp
IN(n, b = FN(i, LL() + i)[n]);
// 入力が 3 10 10 10 なら b = {10, 11, 12}
```

`FN` の式で値を読むときは `LL()` のように `()` を付けます。`FN` は周囲の変数を参照でキャプチャするので、その場の変数を使った処理も書けます。

## 6. vector 以外のコンテナに入れる

`ジェネレータ.to<コンテナ>(個数)` で、格納先を指定できます。

```cpp
IN(n, values = INT.to<set>(n));
// values: set<int>。n 個読み込み、重複を除いて格納する
```

```cpp
IN(n, chars = CHR.to<string>(n));
// chars: string。空白を飛ばしながら n 文字読む
```

```cpp
IN(n, q = LL.to<deque<ll>>(n));
// q: deque<long long>
```

`set` のようにコンテナのテンプレートだけを指定する形と、`deque<ll>` のように型まで指定する形が使えます。`[n]` は `.to<vector>(n)` の省略形です。

対象のコンテナには、デフォルト構築と `insert(end(), 要素)` が必要です。`std::array`、`queue`、`stack` などには、この方法は使えません。

型の指定にコンマが含まれるときは、右辺全体を `(...)` で囲みます。例えば、キーと文字列の組を `map` に入れるなら次のように書けます。

```cpp
IN(n, names = (FN(i, pair{INT(), STR()}).to<map<int, string>>(n)));
// names: map<int, string>。同じキーが複数ある場合は最初の値を残す
```

## IN を使わずに変数を作る

ジェネレータを `()` で呼び出せば、通常の変数宣言と組み合わせて使えます。
実は，`IN(a = LL[n]);` は `auto a = LL[n]();` 相当のコードに展開されています。

ジェネレータを作る段階では、値を生成する処理は実行されません。`LL` や `LL[n]` も、`IN` で評価するか `()` で呼び出す時点で実際に入力を読みます。呼び出すたびに処理を実行するので、同じジェネレータを繰り返し使えます。

```cpp
auto reader = LL[3];       // まだ読まない
auto a = reader();        // ここで 3 個読む
auto b = reader();        // さらに次の 3 個を読む
auto x = INT();           // int を 1 個読む
```

入力を使わずに、定数や添字から配列を作ることもできます。

```cpp
auto zeros = K(0)[5]();             // vector<int> = {0, 0, 0, 0, 0}
auto indices = ID[5]();             // vector<long long> = {0, 1, 2, 3, 4}
auto squares = FN(i, i * i)[5]();   // vector<long long> = {0, 1, 4, 9, 16}
```

`K(0)[5]` や `FN(i, i * i)[5]` がジェネレータで、末尾の `()` を付けた結果が実際の `vector` です。これらは `IN(zeros = K(0)[5]);` のように `IN` の右辺にも指定できます。

## 注意点

- `IN` は新しい変数を宣言します。既存の変数に代入したい場合は `x = INT();` のように書きます。
- `IN(x = INT)` の右辺にはジェネレータを指定します。`IN(x = INT())` や `IN(x = 42)` という書き方はできません。定数なら `IN(x = K(42));` と書けます。
- `IN` の引数を区切るコンマと、テンプレート引数や `{...}` 内のコンマが混ざる場合は、右辺を `(...)` で囲みます。プリプロセッサは `()` の中のコンマを区切りとして扱いませんが、`<>` や `{}` だけでは保護できません。
- `IN` は複数の宣言に展開されます。`if` やループの中で使うときは `{ ... }` の中に置きます。
- 配列の長さは 0 以上で、入力が指定した型・個数に合うことを前提としています。入力の失敗を検査する機能はありません。

## 独自の型を読む

`using mint = atcoder::modint998244353;` に対して、`cin` から `mint` を入力して返すジェネレータ `MINT` を定義してみましょう。  
独自のジェネレータは、`cpin::GEN` に関数オブジェクト `f: ll -> mint` を渡すと作ることができます。  
`MINT()` は `f(0)` を返し、`MINT[n]()` は `vector{f(0), f(1), ..., f(n-1)}` を返します。(実際には、`MINT[n]` は `vector{f(0), f(1), ..., f(n-1)}` を返すジェネレータを作成します。)

```cpp
#include <atcoder/modint>
using namespace cpin;
using mint = atcoder::modint998244353;

const auto MINT = GEN{[](ll) { return mint(LL()); }};

int main() {
    IN(a = MINT);           // a: mint
    IN(n, b = MINT[n]);     // b: vector<mint>

    cout << a.val() << '\n';
    for (const auto& x : b) cout << x.val() << '\n';
}
```

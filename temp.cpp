// Template codes are in https://github.com/nettou03/AtCoder-Template

#include <bits/stdc++.h>
using namespace std;

// ===== typedef =====
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int,int>;
using pll = pair<ll,ll>;
template<class T> using vec = vector<T>;
template<class T> using vec2 = vector<vector<T>>;

// ===== macros =====
#define rep(i,n) for (ll i = 0; i < (ll)(n); ++i)
#define rep2(i,a,b) for (ll i = (ll)(a); i < (ll)(b); ++i)
#define repr(i,n) for (ll i = (ll)(n) - 1; i >= 0; --i)
#define repr2(i,a,b) for (ll i = (ll)(b) - 1; i >= (ll)(a); --i)  // [a, b) を b-1 から a へ
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define eb emplace_back
#define fi first
#define se second
#define sz(x) ((ll)(x).size())

// ===== constants =====
constexpr ll INF = 4e18;
constexpr int IINF = 1 << 30;
constexpr ll MOD  = 1000000007;
constexpr ll MOD9 = 998244353;
constexpr double EPS = 1e-9;
constexpr int dx[4] = {1, -1, 0, 0};
constexpr int dy[4] = {0, 0, 1, -1};

// ===== utility functions =====
template<class T> bool chmax(T &a, const T &b) { if (a < b) { a = b; return true; } return false; }
template<class T> bool chmin(T &a, const T &b) { if (b < a) { a = b; return true; } return false; }

template<class T> istream& operator>>(istream &is, vector<T> &v) {
    for (auto &x : v) is >> x;
    return is;
}
template<class T> ostream& operator<<(ostream &os, const vector<T> &v) {
    for (size_t i = 0; i < v.size(); ++i) os << v[i] << (i + 1 < v.size() ? ' ' : '\n');
    return os;
}
template<class T1, class T2> ostream& operator<<(ostream &os, const pair<T1,T2> &p) {
    os << p.first << ' ' << p.second;
    return os;
}

ll gcd(ll a, ll b) { while (b) { a %= b; swap(a, b); } return a; }
ll lcm(ll a, ll b) { return a / gcd(a, b) * b; }

// ===== Yes / No output =====
#define Yes cout << "Yes" << endl
#define No cout << "No" << endl
#define YES cout << "YES" << endl
#define NO cout << "NO" << endl

ll power(ll a, ll n, ll mod) {
    ll res = 1;
    a %= mod;
    while (n > 0) {
        if (n & 1) res = res * a % mod;
        a = a * a % mod;
        n >>= 1;
    }
    return res;
}

// ===== graph =====
using Graph = vec2<int>;                    // 重みなし隣接リスト
using WGraph = vector<vector<pair<int,ll>>>; // 重み付き隣接リスト {to, cost}

// 読み込み: n頂点 m辺、1-indexed 入力を 0-indexed に変換
Graph read_graph(int n, int m, bool directed = false) {
    Graph g(n);
    rep(i, m) {
        int a, b; cin >> a >> b; --a; --b;
        g[a].pb(b);
        if (!directed) g[b].pb(a);
    }
    return g;
}

// BFS: s からの最短距離（到達不能は -1）
vec<ll> bfs(const Graph &g, int s) {
    vec<ll> dist(g.size(), -1);
    queue<int> q;
    dist[s] = 0; q.push(s);
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int to : g[v]) {
            if (dist[to] != -1) continue;
            dist[to] = dist[v] + 1;
            q.push(to);
        }
    }
    return dist;
}

// DFS（非再帰・スタック溢れ対策）: s から到達可能な頂点に true
vec<bool> dfs(const Graph &g, int s) {
    vec<bool> seen(g.size(), false);
    stack<int> st;
    st.push(s);
    while (!st.empty()) {
        int v = st.top(); st.pop();
        if (seen[v]) continue;
        seen[v] = true;
        for (int to : g[v]) if (!seen[to]) st.push(to);
    }
    return seen;
}

// 連結成分: comp[v] = 成分番号, 戻り値 = 成分数
int connected_components(const Graph &g, vec<int> &comp) {
    int n = g.size(), k = 0;
    comp.assign(n, -1);
    rep(s, n) {
        if (comp[s] != -1) continue;
        stack<int> st; st.push(s); comp[s] = k;
        while (!st.empty()) {
            int v = st.top(); st.pop();
            for (int to : g[v]) if (comp[to] == -1) { comp[to] = k; st.push(to); }
        }
        ++k;
    }
    return k;
}

// グリッドBFS: (sx, sy) からの最短距離（壁・到達不能は -1）
vec2<ll> grid_bfs(const vec<string> &grid, int sx, int sy, char wall = '#') {
    int h = grid.size(), w = grid[0].size();
    vec2<ll> dist(h, vec<ll>(w, -1));
    queue<pii> q;
    dist[sx][sy] = 0; q.push({sx, sy});
    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        rep(d, 4) {
            int nx = x + dx[d], ny = y + dy[d];
            if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
            if (grid[nx][ny] == wall || dist[nx][ny] != -1) continue;
            dist[nx][ny] = dist[x][y] + 1;
            q.push({nx, ny});
        }
    }
    return dist;
}

// Dijkstra: s からの最短距離（到達不能は INF）
vec<ll> dijkstra(const WGraph &g, int s) {
    vec<ll> dist(g.size(), INF);
    priority_queue<pll, vec<pll>, greater<pll>> pq;
    dist[s] = 0; pq.push({0, s});
    while (!pq.empty()) {
        auto [d, v] = pq.top(); pq.pop();
        if (d > dist[v]) continue;
        for (auto [to, c] : g[v]) {
            if (chmin(dist[to], d + c)) pq.push({dist[to], to});
        }
    }
    return dist;
}

// ===== debug =====
#ifdef LOCAL
#define dbg(x) cerr << #x << " = " << (x) << " (line " << __LINE__ << ")" << endl
#else
#define dbg(x)
#endif

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(15);

    return 0;
}

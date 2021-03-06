#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cassert>
#include <set>
#include <string>

using namespace std;

typedef long long ll;

const ll LLINF = 9e18;

struct FlowNetwork {
	struct edge {
		int v, rev; ll cap, cost, flo;
		edge (int _v, ll _cap, ll _cost) : v(_v),
			cap(_cap), cost(_cost), flo(0) { }
	};
	int V;
	vector< vector<edge> > e;
	FlowNetwork(int V) : V(V) {
		e.assign(V, vector<edge>());
	}
	void add_edge(int u, int v, ll uv_cap, ll uv_cost) {
		e[u].push_back(edge(v, uv_cap,  uv_cost));
		e[v].push_back(edge(u,      0, -uv_cost));
		e[u].back().rev = e[v].size() - 1;
		e[v].back().rev = e[u].size() - 1;
	}
	pair<ll, ll> mcmf(int s, int t);
};

vector<ll> d;
struct comp {
	bool operator()(int i, int j) {
		return (d[i] != d[j] ? d[i] < d[j] : i != j);
	}
};

typedef pair<ll, int> li;
pair<ll, ll> FlowNetwork::mcmf(int s, int t) {
	ll flow = 0, cost = 0;
	vector<pair<int, int> > p(V, {-1, -1});
	vector<ll> pot(V, 0);
	d.assign(V, LLINF);
	d[s] = 0;
	
	// Set potentials
	bool ch = true;
	for (int it = 0; it < V - 1 && ch; ++it) {
		ch = false;
		for (int u = 0; u < V; ++u) {
			if (d[u] == LLINF) continue;
			for (int i = 0; i < int(e[u].size()); ++i) {
				edge &uv = e[u][i];
				if (uv.cap == uv.flo) continue;
				int v = uv.v;
				if (d[v] == LLINF || d[v] > d[u] + uv.cost) {
					d[v] = d[u] + uv.cost;
					ch = true;
				}
			}
		}
	}
	for (int i = 0; i < V; ++i)
		if (d[i] != LLINF) pot[i] = d[i];
	
	
	while (true) {
		d.assign(V, -1);
		d[s] = 0;
		set<int, comp> st;
		st.insert(s);
		while (!st.empty()) {
			int u = *st.begin();
			st.erase(st.begin());
			for (int i = 0; i < int(e[u].size()); ++i) {
				edge &uv = e[u][i];
				if (uv.cap == uv.flo) continue;
				int v = uv.v;
				ll w = d[u] + uv.cost + pot[u] - pot[v];
				if (d[v] == -1 || w < d[v]) {
					if (st.find(v) != st.end())
						st.erase(st.find(v));
					d[v] = w;
					p[v] = {u, i};
					st.insert(v);
				}
			}
		}
		if (d[t] == -1) break;
		int u = t;
		ll cap = LLINF;
		while (u != s) {
			edge &uv = e[p[u].first][p[u].second];
			cap = min(cap, uv.cap - uv.flo);
			u = p[u].first;
		}
		if (cap == 0) break;
		u = t;
		flow += cap;
		while (u != s) {
			edge &uv = e[p[u].first][p[u].second];
			edge &vu = e[uv.v][uv.rev];
			uv.flo += cap;
			vu.flo -= cap;
			cost += cap * uv.cost;
			u = p[u].first;
		}
		for (int i = 0; i < V; ++i)
			if (d[i] != -1) pot[i] += d[i];
	}
	
	return {flow, cost};
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	
	int T;
	cin >> T;
	for (int t = 1; t <= T; ++t) {
		int W, H;
		cin >> W >> H;
		FlowNetwork fn(2 * W * H);
		for (int y = 0; y < H; ++y) {
			string s;
			cin >> s;
			for (int x = 0; x < W; ++x) {
				int type = (s[x]=='.'?0:(s[x]=='*'?1:2));
				
				if (y < H - 1)
				fn.add_edge(2*(y*W+x)+1, 2*(y*W+x+W), 2, 0);
				if (x < W - 1)
				fn.add_edge(2*(y*W+x)+1, 2*(y*W+x+1), 2, 0);

				if (type == 2) continue;
				if (type == 1) {
					fn.add_edge(2*(y*W+x), 2*(y*W+x)+1, 1, -1);
					fn.add_edge(2*(y*W+x), 2*(y*W+x)+1, 1, 0);
				}
				if (type == 0)
				fn.add_edge(2*(y*W+x), 2*(y*W+x)+1, 2, 0);
			}
		}
		auto res = fn.mcmf(0, 2 * W * H - 1);
		assert(res.first == 2);
		cout << (-res.second) << endl;
	}

	return 0;
}

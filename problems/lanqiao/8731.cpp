#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <map>

using namespace std;

const int INF = 0x3f3f3f3f;
const int MAXN = 10005;
const int MAXM = 1000000; // 边数限制在百万内存约 40MB

struct CostFlow {
	int head[MAXN], to[MAXM], cap[MAXM], flow[MAXM], cost[MAXM], nxt[MAXM];
	int dis[MAXN], pre[MAXN], last[MAXN];
	bool vis[MAXN];
	int idx, S, T;

	void init(int s, int t) {
		S = s; T = t; idx = 0;
		fill(head, head + MAXN, -1);
	}

	void add(int u, int v, int c, int w) {
		to[idx] = v; cap[idx] = c; flow[idx] = 0; cost[idx] = w; nxt[idx] = head[u]; head[u] = idx++;
		to[idx] = u; cap[idx] = 0; flow[idx] = 0; cost[idx] = -w; nxt[idx] = head[v]; head[v] = idx++;
	}

	bool spfa(int& max_cost) {
		fill(dis, dis + MAXN, -INF);
		fill(vis, vis + MAXN, false);
		queue<int> q;
		dis[S] = 0; q.push(S); vis[S] = true;

		while (!q.empty()) {
			int u = q.front(); q.pop(); vis[u] = false;
			for (int i = head[u]; i != -1; i = nxt[i]) {
				if (cap[i] > flow[i] && dis[to[i]] < dis[u] + cost[i]) {
					dis[to[i]] = dis[u] + cost[i];
					pre[to[i]] = u;
					last[to[i]] = i;
					if (!vis[to[i]]) { vis[to[i]] = true; q.push(to[i]); }
				}
			}
		}
		if (dis[T] == -INF) return false;
		max_cost += dis[T];
		int cur = T;
		while (cur != S) {
			flow[last[cur]]++;
			flow[last[cur] ^ 1]--;
			cur = pre[cur];
		}
		return true;
	}
} mcf;

long long g[5005];
int f_val[5005];
int last_f[11]; // 记每数根上一次出点
map<long long, int> last_val; // 记每个具值上一次出点

int main() {
	ios::sync_with_stdio(false); cin.tie(0);
	int n; cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> g[i];
		f_val[i] = (g[i] - 1) % 9 + 1;
	}

	// 1~n: 入点, n+1~2n: 出点
	mcf.init(0, 2 * n + 1);

	for (int i = 1; i <= n; i++) {
		mcf.add(i, i + n, 1, 1);      // 节点内部边
		mcf.add(0, i, 1, 0);          // 源点到入点
		mcf.add(i + n, 2 * n + 1, 1, 0); // 出点到汇点

		// 数根相同 f(gi) == f(gj)
		// 建立链式连接：上一个同数根点的入点 -> 当前点的入点 P=0
		// 出点 下同根点的入点
		if (last_f[f_val[i]]) {
			int prev = last_f[f_val[i]];
			mcf.add(prev, i, INF, 0);      // 入点 链跳点
			mcf.add(prev + n, i, INF, 0);  // 实跳
		}
		last_f[f_val[i]] = i;

		// |gi - gj| == 11
		// 链式BUT数值散直连出现的最近符条点
		long long target1 = g[i] - 11;
		long long target2 = g[i] + 11;
		if (last_val.count(target1)) mcf.add(last_val[target1] + n, i, INF, 0);
		if (last_val.count(target2)) mcf.add(last_val[target2] + n, i, INF, 0);

		// 更新值最后位
		last_val[g[i]] = i;
	}

	int current_cost = 0;
	for (int i = 1; i <= n; i++) {
		//SPFA 返f不能法增加流量输当前最大 cost
		mcf.spfa(current_cost);
		cout << current_cost << (i == n ? "" : " ");
	}
	cout << endl;

	return 0;
}
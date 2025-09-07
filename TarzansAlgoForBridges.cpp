vector<int> disc, low, parent;
vector<pair<int, int> > bridges;
int timer;

void find_bridges_dfs(int u, const vector<vector<ll> >& adj) {
    disc[u] = low[u] = timer++;
    
    for (ll v_ll : adj[u]) {
        int v = static_cast<int>(v_ll);
        
        if (v == parent[u]) {
            continue;
        }

        if (disc[v] != -1) {
            low[u] = min(low[u], disc[v]);
        } 
        else {
            parent[v] = u;
            find_bridges_dfs(v, adj);
            
            low[u] = min(low[u], low[v]);
            
          
            if (low[v] > disc[u]) {
                bridges.push_back(make_pair(u, v));
            }
        }
    }
}


vector<pair<int, int> > findBridges(int n, const vector<vector<ll> >& adj) {
    disc.assign(n + 1, -1);
    low.assign(n + 1, -1);
    parent.assign(n + 1, -1);
    bridges.clear();
    timer = 0;

    for (int i = 1; i <= n; ++i) {
        if (disc[i] == -1) {
            find_bridges_dfs(i, adj);
        }
    }
    return bridges;
}
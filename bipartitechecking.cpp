//checks if a graph is bipartite or not.
// also finds the connected components of the graph.

bool checkBipartite(int start,ll color,vector<ll>&nodes){
    queue<pii>q;
    q.push(make_pair(start,color));
    visited[start]=color;
    // vallist.push_back(color);
    bool flag=true;
    while(!q.empty()){
        ll node=q.front().first;
        ll col=q.front().second;
        nodes.push_back(node);

        q.pop();

        for(auto x:graph[node]){
            if(visited[x]==-1){
                visited[x]=1-col;
                q.push(make_pair(x,1-col));
            }
            else if(visited[x]==col){
                flag= false;
            }
        }
    }

    return flag;
}
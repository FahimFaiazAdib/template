vector<vector<pair<ll,ll> > >graph;

void djkst(int startnode,vector<ll>&kept){

        priority_queue<pair<ll,ll>, std::vector<pair<ll,ll> >, std::greater<pair<ll,ll> > > pq;
        pq.push(make_pair(0,startnode));
        kept[startnode]=0;
        while(!pq.empty()){
            ll dist= pq.top().first;
            ll node=pq.top().second;
            pq.pop();
            if(kept[node]<dist)continue;
            for(auto it : graph[node]){
                if(kept[it.first]>max(it.second,dist)){
                    kept[it.first]=max(it.second,dist);
                    pq.push(make_pair(kept[it.first],it.first));
                }
            }
        }


}
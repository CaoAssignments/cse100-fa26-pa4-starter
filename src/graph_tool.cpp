// Provided input/output and scenario driver. Do not modify.
#include "Graph.hpp"
#include "DisjointSet.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
struct Road { std::size_t u, v; long long weight; };
std::size_t number(const std::string& s) {
    if (s.empty() || s.find_first_not_of("0123456789") != std::string::npos)
        throw std::invalid_argument("expected a nonnegative integer");
    std::size_t used=0; auto value=std::stoull(s,&used);
    if (used != s.size() || value > std::numeric_limits<std::size_t>::max())
        throw std::invalid_argument("integer out of range");
    return static_cast<std::size_t>(value);
}
std::vector<std::size_t> readEvents(const std::string& file, std::size_t m, bool restoration) {
    std::ifstream in(file); std::string line;
    if (!std::getline(in,line)) throw std::runtime_error("cannot read scenario file: "+file);
    if (!line.empty() && line.back()=='\r') line.pop_back();
    if (line != (restoration ? "step\tedge_id" : "edge_id")) throw std::runtime_error("invalid scenario header");
    std::vector<std::size_t> result; std::vector<bool> seen(m,false);
    while (std::getline(in,line)) {
        if (!line.empty() && line.back()=='\r') line.pop_back();
        if (restoration) {
            auto tab=line.find('\t');
            if (tab==std::string::npos || number(line.substr(0,tab))!=result.size()+1)
                throw std::runtime_error("restoration steps must start at 1 and be consecutive");
            line=line.substr(tab+1);
        }
        auto edge=number(line);
        if (edge>=m || seen[edge]) throw std::runtime_error("invalid or duplicate scenario edge ID");
        seen[edge]=true; result.push_back(edge);
    }
    return result;
}
int main(int argc,char** argv) {
    try {
        if (argc<3) throw std::invalid_argument("usage: graph_tool GRAPH dfs SOURCE [CLOSURES] | bfs SOURCE TARGET [CLOSURES] | dijkstra SOURCE TARGET [CLOSURES] | components [CLOSURES] | restore CLOSURES RESTORATION SOURCE TARGET");
        std::string mode=argv[2]; int basic=mode=="dfs"?4:((mode=="bfs"||mode=="dijkstra")?5:3);
        bool restore=mode=="restore";
        if (restore ? argc!=7 : ((mode!="dfs"&&mode!="bfs"&&mode!="dijkstra"&&mode!="components") || (argc!=basic&&argc!=basic+1)))
            throw std::invalid_argument("invalid command/arguments");
        std::ifstream in(argv[1]); long long n,m;
        if (!(in>>n>>m)||n<0||n>100000||m<0||m>200000) throw std::runtime_error("invalid graph header");
        Graph graph(static_cast<std::size_t>(n)); std::vector<Road> roads;
        for(long long i=0;i<m;++i) {
            long long u,v,w;
            if(!(in>>u>>v>>w)||u<0||v<0||u>=n||v>=n||w<0||w>1000000000LL) throw std::runtime_error("invalid edge");
            roads.push_back({static_cast<std::size_t>(u),static_cast<std::size_t>(v),w});
        }
        std::string extra; if(in>>extra) throw std::runtime_error("unexpected extra graph input");
        std::vector<bool> closed(roads.size(),false);
        if(restore||argc==basic+1) for(auto e:readEvents(argv[restore?3:basic],roads.size(),false)) closed[e]=true;
        std::vector<std::size_t> events;
        if(restore) {
            events=readEvents(argv[4],roads.size(),true);
            for(auto e:events) if(!closed[e]) throw std::runtime_error("restored edge was not initially closed");
        }
        std::size_t source=0,target=0;
        if(mode!="components") {
            source=number(argv[restore?5:3]);
            target=(mode=="dfs")?source:number(argv[restore?6:4]);
            if(source>=static_cast<std::size_t>(n)||target>=static_cast<std::size_t>(n)) throw std::out_of_range("vertex ID out of range");
        }
        if(restore||mode=="components") {
            DisjointSet sets(static_cast<std::size_t>(n));
            for(std::size_t e=0;e<roads.size();++e) if(!closed[e]) sets.unite(roads[e].u,roads[e].v);
            if(restore) {
                std::cout<<"step\tedge_id\tmerged\tcomponents\tsource_component_size\tconnected\n";
                std::cout<<"0\t-\t-\t"<<sets.componentCount()<<'\t'<<sets.componentSize(source)<<'\t'<<sets.connected(source,target)<<'\n';
                for(std::size_t i=0;i<events.size();++i) {
                    auto e=events[i]; bool merged=sets.unite(roads[e].u,roads[e].v);
                    std::cout<<i+1<<'\t'<<e<<'\t'<<merged<<'\t'<<sets.componentCount()<<'\t'<<sets.componentSize(source)<<'\t'<<sets.connected(source,target)<<'\n';
                }
            } else {
                std::cout<<sets.componentCount()<<'\n';
                for(std::size_t v=0;v<static_cast<std::size_t>(n);++v) std::cout<<(v?" ":"")<<sets.componentSize(v);
                std::cout<<'\n';
            }
        } else {
            for(std::size_t e=0;e<roads.size();++e) if(!closed[e]) graph.addEdge(roads[e].u,roads[e].v,roads[e].weight);
            if(mode=="dfs") {
                auto order=graph.dfs(source);
                for(std::size_t i=0;i<order.size();++i) std::cout<<(i?" ":"")<<order[i];
                std::cout<<'\n';
            } else {
                auto r=mode=="bfs"?graph.bfs(source,target):graph.dijkstra(source,target);
                if(r.distance==Graph::INF) std::cout<<"UNREACHABLE\n";
                else {
                    std::cout<<r.distance<<'\n';
                    for(std::size_t i=0;i<r.path.size();++i) std::cout<<(i?" ":"")<<r.path[i];
                    std::cout<<'\n';
                }
            }
        }
    } catch(const std::exception& e) { std::cerr<<e.what()<<'\n'; return 1; }
}

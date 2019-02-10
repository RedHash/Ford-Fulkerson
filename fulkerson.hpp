#pragma once

#include <map>
#include <string>
#include <unordered_map>

class falkerson_transport_net
{
    struct edge
    {
        long flow;
        long capacity;
        bool real;
    };
public:
    falkerson_transport_net()
    {
        build_transport_net();
    }

    void build_transport_net();

    void run_max_flow();

    long get_max_flow() const;

    void print_flows_lexicographically() const;

private:
    //First node, second node, arc
    std::map<std::string, std::map<std::string, edge>> network;
    std::map<std::string, std::map<std::string, long>> residual_graph;
    std::string source;
    std::string sink;

private:
    void insert_auxiliary_edges();

    void fill_residual_graph();

    std::unordered_map<std::string, std::string> get_path_in_residual_graph(); //BFS

    long get_min_flow_way(const std::unordered_map<std::string, std::string>& parents) const;

    void increase_flow(long lowest_throughput, const std::unordered_map<std::string, std::string>& parents);

};
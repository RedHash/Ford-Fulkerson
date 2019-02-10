#include "fulkerson.hpp"

#include <iostream>
#include <algorithm>
#include <set>
#include <queue>
#include <climits>

void falkerson_transport_net::build_transport_net()
{
    long edge_quantity;
    std::cout << "Input quantity of edges: ";
    std::cin >> edge_quantity;
    std::cout << "Input source and sink of the net: ";
    std::cin >> source >> sink;

    std::cout << R"(Input edges as: "First node" "Second node" "Capacity")" << std::endl;
    std::string first_node = "-1", second_node = "-1";
    for (auto i = 0; i < edge_quantity; i++)
    {
        long capacity = -1;

        std::cin >> first_node >> second_node >> capacity;

        network[first_node].insert({second_node, {0, capacity, true}});
    }
}

long falkerson_transport_net::get_max_flow() const
{
    long max_flow = 0;
    // TODO : add exception catcher
    for (const auto& source_node_connections : network.at(source))
    {
        if (source_node_connections.second.real && source_node_connections.second.flow > 0)
        {
            max_flow += source_node_connections.second.flow;
        }
    }

    return max_flow;
}

void falkerson_transport_net::print_flows_lexicographically() const
{
    for (const auto& connections : network)
    {
        for (const auto& edge : connections.second)
        {
            if (edge.second.real)
            {
                std::cout << connections.first << ' ' << edge.first;

                if (edge.second.flow > 0)
                {
                    std::cout << ' ' << edge.second.flow << std::endl;
                }
                else
                {
                    std::cout << ' ' << 0 << std::endl;
                }
            }
        }
    }
}

void falkerson_transport_net::insert_auxiliary_edges()
{
    //Insert fake backward arcs (if they don't already exist)
    for (const auto& connections : network)
    {
        for (const auto& edge : connections.second)
        {
            const std::string& first_node_name = connections.first;
            const std::string& second_node_name = edge.first;

            //if (network[second_node_name].find(first_node_name) == network[second_node_name].end())
            if (!network[second_node_name].count(first_node_name))
            {
                network[second_node_name].insert({first_node_name, {0, 0, false}});
            }
        }
    }
}

void falkerson_transport_net::run_max_flow()
{
    insert_auxiliary_edges();

    fill_residual_graph();

    auto parents = get_path_in_residual_graph();

    while (parents.find(sink) != parents.end())
    {
        long lowest_throughput = get_min_flow_way(parents);

        increase_flow(lowest_throughput, parents);

        fill_residual_graph();

        parents = get_path_in_residual_graph();
    }
}

void falkerson_transport_net::fill_residual_graph()
{
    for (const auto& connections : network)
    {
        for (const auto& edge : connections.second)
        {
            const std::string& first_node = connections.first;
            const std::string& second_node = edge.first;

            // Capacity of edge - flow on it
            residual_graph[first_node][second_node] = network[first_node][second_node].capacity -
                                                      network[first_node][second_node].flow;
        }
    }
}

std::unordered_map<std::string, std::string> falkerson_transport_net::get_path_in_residual_graph()
{
    std::set<std::string> visited;
    visited.insert(source);

    std::unordered_map<std::string, std::string> parents;
    parents[source] = sink;

    std::queue<std::string> queue_of_vertices;
    queue_of_vertices.push(source);

    while (!queue_of_vertices.empty())
    {
        std::string prev_node = queue_of_vertices.front();
        queue_of_vertices.pop();

        for (const auto& connections : residual_graph[prev_node])
        {

            const std::string& next_node = connections.first;

            if (!visited.count(next_node) && residual_graph[prev_node][next_node] > 0)
            {
                queue_of_vertices.push(next_node);
                parents[next_node] = prev_node;
                visited.insert(next_node);
            }
        }
    }

    return parents;
}

long falkerson_transport_net::get_min_flow_way(const std::unordered_map<std::string, std::string>& parents) const
{
    std::string next_node = sink;

    std::string prev_node = parents.at(sink);

    long min_capacity = LONG_MAX;

    while (next_node != source)
    {
        min_capacity = std::min(min_capacity, residual_graph.at(prev_node).at(next_node));

        next_node = prev_node;

        prev_node = parents.at(next_node);
    }

    return min_capacity;
}

void falkerson_transport_net::increase_flow(long lowest_throughput, const std::unordered_map<std::string, std::string>& parents)
{
    std::string next_node = sink;

    std::string prev_node = parents.at(sink);

    while (next_node != source)
    {
        network[prev_node][next_node].flow += lowest_throughput;
        network[next_node][prev_node].flow -= lowest_throughput;

        next_node = prev_node;
        prev_node = parents.at(next_node);
    }
}
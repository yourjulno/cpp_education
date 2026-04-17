#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>

int main()
{
    constexpr int vertex_count = 10;
    using Graph = boost::adjacency_matrix<
        boost::undirectedS,
        boost::no_property,
        boost::property<boost::edge_weight_t, int>
    >;

    Graph graph(vertex_count);

    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> distribution(1, 10);

    auto weights = boost::get(boost::edge_weight, graph);

    for (int i = 0; i < vertex_count; ++i)
    {
        for (int j = i + 1; j < vertex_count; ++j)
        {
            auto [edge, inserted] = boost::add_edge(i, j, graph);
            if (inserted)
            {
                weights[edge] = distribution(engine);
            }
        }
    }

    std::cout << "Adjacency matrix:\n";
    for (int i = 0; i < vertex_count; ++i)
    {
        for (int j = 0; j < vertex_count; ++j)
        {
            if (i == j)
            {
                std::cout << std::setw(3) << 0;
            }
            else
            {
                auto [edge, exists] = boost::edge(i, j, graph);
                std::cout << std::setw(3) << weights[edge];
            }
        }
        std::cout << '\n';
    }

    std::vector<int> permutation;
    for (int i = 1; i < vertex_count; ++i)
    {
        permutation.push_back(i);
    }

    int best_cost = std::numeric_limits<int>::max();
    std::vector<int> best_path;

    do
    {
        int current_cost = 0;
        int current_vertex = 0;

        for (int next_vertex : permutation)
        {
            auto [edge, exists] = boost::edge(current_vertex, next_vertex, graph);
            current_cost += weights[edge];
            current_vertex = next_vertex;
        }

        auto [last_edge, exists] = boost::edge(current_vertex, 0, graph);
        current_cost += weights[last_edge];

        if (current_cost < best_cost)
        {
            best_cost = current_cost;
            best_path.clear();
            best_path.push_back(0);
            for (int vertex : permutation)
            {
                best_path.push_back(vertex);
            }
            best_path.push_back(0);
        }
    }
    while (std::next_permutation(permutation.begin(), permutation.end()));

    std::cout << "\nOptimal path:\n";
    for (std::size_t i = 0; i < best_path.size(); ++i)
    {
        std::cout << best_path[i];
        if (i + 1 < best_path.size())
        {
            std::cout << " -> ";
        }
    }

    std::cout << "\nTotal cost:\n" << best_cost << '\n';

    return 0;
}
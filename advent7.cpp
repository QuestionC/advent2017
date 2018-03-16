#include "advent.hpp"

// Input7.txt describes as
// root (weight) -> leaf, leaf...

// Build the tree

#include <map>
#include <stdlib.h>

class Node {
    public:
    std::vector<std::string> children;
    std::vector<std::string> parents;
    std::string name;
    int weight;

    Node() {}
    Node(std::string const & N, int W) : name(N), weight(W) {}
};

std::ostream & operator<< (std::ostream & out, Node const & print_me) {
    out << print_me.name << " (" << print_me.weight << ")\n";
    out << "children: " << print_me.children;
    out << "parents: " << print_me.parents;

    return out;
}

void calc_tot_weights(std::map<std::string, int> & tot_weights, std::map<std::string, Node> & graph, std::string node)
{
    Node & N = graph[node];

    if (node == "gmcrj" || node == "piyyydu" || node == "aawuak") {
        DPRINT(node);
        DPRINT(N);
    }

    int tot_weight = N.weight;
    for (auto S = N.children.begin(); S != N.children.end(); ++S) {
        calc_tot_weights(tot_weights, graph, *S);
        tot_weight += tot_weights[*S];
    }

    // std::cout << node << " = " << tot_weight << "\n";
    tot_weights[node] = tot_weight;

    return;
}

std::string find_unbalanced_child(std::map<std::string, Node> graph, std::map<std::string, int> tot_weights, int delta, std::string node_name) {
    Node & N = graph[node_name];

    for (int i = 0; i < N.children.size() - 1; ++i) {
        int W1 = tot_weights[N.children[i]];
        int W2 = tot_weights[N.children[i + 1]];
    
        if (W1 != W2) {
            if (W1 - W2 == delta)
                return N.children[i];
            else if (W2 - W1 == delta)
                return N.children[i + 1];
            else {
                std::cerr << "Impossible case in find_unbalanced_child";
                exit(-1);
            }
        }
    }

    // No unbalanced children
    return std::string();
}

int main(void) {
    std::map<std::string, Node> graph;

    std::ifstream input("input7.txt");

    std::string line;

    while (std::getline(input, line)) {
        char buff[100];
        int num_chars;
        int tot_num_chars;
        int n;

        std::string name;
        int weight;
        std::vector<std::string> children;
        char const * c_str = line.c_str();

        n = sscanf(c_str, " %s (%d)%n", buff, &weight, &num_chars);
        if (n != 2) {
            exit(-1);
        }
        name = std::string(buff);

        if (name == "gmcrj" || name == "aawuak") {
            DPRINT(name);
        }

        graph[name].name = name;
        graph[name].weight = weight;
        
        tot_num_chars = num_chars;

        n = sscanf(c_str + tot_num_chars, " -> %[^,]%n", buff, &num_chars);
        if (n == -1) {
            continue;
        }
        graph[name].children.push_back(buff);
        graph[buff].parents.push_back(name);

        tot_num_chars += num_chars;

        while (n != -1) {
            n = sscanf(c_str + tot_num_chars, ", %[^,]%n", buff, &num_chars);
            if (n == -1) {
                // This should fall through inner and outer loop.
                continue;
            }
            graph[name].children.push_back(buff);
            graph[buff].parents.push_back(name);
            
            tot_num_chars += num_chars;
        }
        if (n == -1) {
            continue;
        }
    }

    auto i = graph.begin();

    Node * curr = &i->second;
    while (!curr->parents.empty()) {
        curr = &graph[curr->parents.front()];
    }
    Node const & N = *curr;

    std::cout << N;

    // Part 2
    // Find the unbalanced disc.
    std::map<std::string, int> tot_weights;
    calc_tot_weights(tot_weights, graph, N.name);

    // Find the wrong one
    // The weight difference we can know from looking just at the root as long as it has 3 children.
    if (N.children.size() < 3) {
        std::cout << "I dunno how to handle root node with less than 3 kids.";
        return -1;
    }
    int W1 = tot_weights[N.children[0]];
    int W2 = tot_weights[N.children[1]];
    int W3 = tot_weights[N.children[2]];

    int correct_weight;
    if (W1 == W2 || W1 == W3) {
        correct_weight = W1;
    } else {
        correct_weight = W2;
    }

    std::vector<std::string>::const_iterator S;
    for (S = N.children.begin(); S != N.children.end(); ++S) {
        if (tot_weights[*S] != correct_weight)
            break;
    }

    int weight_delta = tot_weights[*S] - correct_weight;
  
    // Now that we have the weight delts, just run down the tree until we find a balanced node.  That's the one whose weight needs changed.

    std::string unbalanced_node = N.name;
    std::string next_unbalanced_node;
    for (;;) {
        next_unbalanced_node = find_unbalanced_child(graph, tot_weights, weight_delta, unbalanced_node);
        if (next_unbalanced_node == std::string())
            break;
        unbalanced_node = next_unbalanced_node;
    }

    Node & unbalance = graph[unbalanced_node];
    std::cout << tot_weights[unbalanced_node] << "\n";
    std::cout << unbalance;
    for (auto a = unbalance.children.begin(); a != unbalance.children.end(); ++a) {
        std::cout << tot_weights[*a] << "\n";
        std::cout << graph[*a];
    }

    std::cout << "===========\n";

    std::string parent_node = unbalance.parents.front();
    Node & parent = graph[parent_node];
    std::cout << tot_weights[parent_node] << "\n";
    std::cout << parent;
    for (auto a = parent.children.begin(); a != parent.children.end(); ++a) {
        std::cout << tot_weights[*a] << "\n";
        std::cout << graph[*a];
    }

    std::cout << graph[unbalanced_node].weight - weight_delta << "\n";

    /*
    std::string node_name;
    while (std::getline(std::cin, node_name)) {
        std::cout << tot_weights[node_name] << "\n";
        std::cout << graph[node_name];
    }
    */

    return 0;
}

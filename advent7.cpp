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
    out << print_me.children;
    out << print_me.parents;

    return out;
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

    Node & N = i->second;
    while (!N.parents.empty()) {
        N = graph[N.parents.front()];
    }

    std::cout << N;



    return 0;
}

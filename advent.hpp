#ifndef ADVENT_HPP
#define ADVENT_HPP

// Stuff that makes life easy

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#define DPRINT(X) std::cerr << #X ": " << X << '\n';

std::ostream & operator<< (std::ostream & out, std::pair<int, int> P) {
    out << "(" << P.first << ", " << P.second << ")\n";
    return out;
}

std::ostream & operator<< (std::ostream & out, std::vector<int> V) {
    out << "[";

    if (V.empty()) {
        return out << "]\n";
    }
    
    std::vector<int>::const_iterator ci = V.begin();
    out << *ci++;

    for (; ci != V.end(); ++ci) {
        out << ", " << *ci;
    }

    out << "]\n";

    return out;
}

std::string ToString(std::vector<int> const & V) {
    std::stringstream S;
    S << V;

    return S.str();
}

#endif // ADVENT_HPP

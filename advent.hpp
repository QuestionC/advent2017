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

template <typename T>
std::ostream & operator<< (std::ostream & out, std::vector<T> V) {
    out << "[";

    if (V.empty()) {
        return out << "]\n";
    }
    
    typename std::vector<T>::const_iterator ci = V.begin();
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

// Take a string, split it up into substrings.
// Not as efficient as C++ might expect but who gives a shit, it's parsing.
std::vector<std::string> str_split(std::string split_me, std::string const & token) {
    std::vector<std::string> result;

    size_t begin = 0;
    size_t end = split_me.find(token);

    result.push_back(split_me.substr(begin, end));

    while (end != std::string::npos) {
        begin = end + token.length();
        end = split_me.find(token, begin);
        result.push_back(split_me.substr(begin, end));
    }

    return result;
}

std::string str_trim(std::string const & trim_me) {
    size_t L = trim_me.length();

    if (L == 0)
        return "";

    char const * data = trim_me.data();

    size_t begin;
    for (begin = 0; begin < L; ++begin) {
        if (!isspace(trim_me[begin])) {
            break;
        }
    }

    if (begin == L) {
        return "";
    }

    size_t end;
    for (end = L - 1; end > 0; --end) {
        if (!isspace(trim_me[end])) {
            break;
        }
    }

    // range is [begin, end)
    end += 1;

    return trim_me.substr(begin, end);
}

#endif // ADVENT_HPP

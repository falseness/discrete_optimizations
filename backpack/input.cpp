#include "input.hpp"

#include <fstream>
#include <cstring>

Input ReadFromFile(const char* file_path) {
    Input result;
    std::ifstream fin(file_path);
    fin >> result.n >> result.backpack_weight;
    for (size_t i = 0; i < result.n; ++i) {
        uint64_t cost, weight;
        fin >> cost >> weight;
        result.items.push_back(Item{cost, weight});
    }
    return result;
}

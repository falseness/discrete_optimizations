#include <iostream>

#include <unordered_set>

#include <cassert>

#include <input.hpp>


int main(int argc, char *argv[]) {
    assert(argc == 2);
    
    uint64_t indices_count;
    std::cin >> indices_count;
    std::unordered_set<uint64_t> indices;
    for (int i = 0; i < indices_count; ++i) {
        uint64_t index;
        std::cin >> index;
        auto [_, inserted] = indices.insert(index);
        if (inserted) {
            continue;
        }
        return 1;
    }

    auto input = ReadFromFile(argv[1]);
    uint64_t result_weight = 0;
    uint64_t result_cost = 0;
    for (auto index : indices) {
        const auto& item = input.items.at(index);
        result_weight += item.weight;
        result_cost += item.cost;
    }
    if (result_weight > input.backpack_weight) {
        return 1;
    }
    std::cout << "ok backpack with cost " << result_cost << std::endl;
}
#include <bits/stdint-uintn.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>


struct Item {
    uint64_t cost;
    uint64_t weight;
};

bool comp(const Item& lhs, const Item& rhs) {
    return lhs.cost * rhs.weight > rhs.cost * lhs.weight;
}

uint64_t get_greedy(uint64_t backpack_weight, const std::deque<Item>& input) {
    uint64_t current_weight = 0;
    uint64_t current_cost = 0;
    for (const auto& item : input) {
        if (current_weight + item.weight <= backpack_weight) {
            current_weight += item.weight;
            current_cost += item.cost;
        }
    }
    return current_cost;
}

void run() {
    std::ifstream fin("./data/ks_10000_0");
    int n, backpack_weight;
    fin >> n >> backpack_weight;
    std::deque<Item> input;
    for (size_t i = 0; i < n; ++i) {
        uint64_t cost, weight;
        fin >> cost >> weight;
        input.push_back(Item{cost, weight});
    }
    std::sort(input.begin(), input.end(), comp);
    uint64_t best_result = 0;
    while (!input.empty()) {
        best_result = std::max(best_result, get_greedy(backpack_weight, input));
        input.pop_front();
    }
    
    std::cout << best_result << std::endl;
}


int main() {
    run();
}

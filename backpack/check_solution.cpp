#include <bits/stdint-uintn.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <vector>
#include <cassert>
#include <cmath>


struct Item {
    uint64_t cost;
    uint64_t weight;
};

bool comp(const Item& lhs, const Item& rhs) {
    return lhs.cost * rhs.weight > rhs.cost * lhs.weight;
}

struct Result {
    std::unordered_set<size_t> indices = {};
    uint64_t cost = 0;
    uint64_t weight = 0;

    void Insert(const size_t index, const std::vector<Item>& input) {
        bool inserted = indices.insert(index).second;
        assert(inserted);
        cost += input.at(index).cost;
        weight += input[index].weight;
    }
};

bool IsCorrect(const Result& result, uint64_t backpack_weight, const std::vector<Item>& input) {
    uint64_t current_weight = 0;
    uint64_t current_cost = 0;
    for (auto index : result.indices) {
        const auto& item = input.at(index); 
        current_weight += item.weight;
        current_cost += item.cost;
    }
    bool is_correct = current_weight <= backpack_weight && current_cost == result.cost && current_weight == result.weight;
    if (is_correct) {
        return true;
    }  
    std::cout << "INCORRECT " << backpack_weight << ' ' << current_weight << ' ' << current_cost << ' ' << result.weight << ' ' << result.cost << std::endl;
    return false;
}

Result GetGreedy(const uint64_t backpack_weight, const size_t index_to_start,
                 const std::vector<Item> &input) {
  Result result;
  for (size_t i = index_to_start; i < input.size(); ++i) {
    if (result.weight + input[i].weight <= backpack_weight) {
      result.Insert(i, input);
    }
  }
  return result;
}

bool get_bit(uint64_t mask, size_t i) {
    return (mask >> i) & 1;
}


double RandDouble(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

uint64_t RandomGreedyTmp(uint64_t backpack_weight, const std::vector<Item>& input) {
    uint64_t current_weight = 0;
    uint64_t current_cost = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        const auto& item = input[i];
        double chance = 1 / (double)(input.size() - i + 1);
        if (RandDouble(0.0, 1.0) < chance) {
            continue;
        }
        if (current_weight + item.weight <= backpack_weight) {
            current_weight += item.weight;
            current_cost += item.cost;
        }
    }
    return current_cost;
}


uint64_t ExponentialGreedy(uint64_t n, uint64_t backpack_weight, const std::vector<Item>& input) {
    size_t first_elements = std::round(std::log2(1e8 / n)) + 1;
    first_elements = std::min(first_elements, n);

    uint64_t best_result = 0;

    for (size_t mask = 0; mask < (1 << first_elements); ++mask) {
        Result result;
        for (size_t i = 0; i < first_elements; ++i) {
            if (!get_bit(mask, i)) {
                continue;
            }
            if (result.weight + input[i].weight > backpack_weight) {
                break;
            }
            result.Insert(i, input);
        }
        auto another_result = GetGreedy(backpack_weight - result.weight, first_elements, input);
        for (auto index : result.indices) {
            another_result.Insert(index, input);
        }
        assert(IsCorrect(another_result, backpack_weight, input));

        best_result = std::max(best_result, another_result.cost);
    }
    return best_result;
}

uint64_t RandomGreedy(uint64_t n, uint64_t backpack_weight, const std::vector<Item>& input) {
    uint64_t best_result = 0;
    for (size_t i = 0; i < 1e8 / n; ++i) {
        best_result = std::max(RandomGreedyTmp(backpack_weight, input), best_result);
    }
    return best_result;
}


void run() {


    std::ifstream fin("./data/ks_400_0");
    uint64_t n, backpack_weight;
    fin >> n >> backpack_weight;
    std::vector<Item> input;
    for (size_t i = 0; i < n; ++i) {
        uint64_t cost, weight;
        fin >> cost >> weight;
        input.push_back(Item{cost, weight});
    }
    
}


int main() {
    run();
}

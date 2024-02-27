#include <bits/stdint-uintn.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <vector>
#include <cassert>
#include <numeric>
#include <cmath>

#include <input.hpp>

struct Result {
    std::unordered_set<size_t> indices = {};
    uint64_t cost = 0;
    uint64_t weight = 0;

    auto operator<(const Result& other) const {
        return cost < other.cost;
    }

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

Result RandomGreedyTmp(uint64_t backpack_weight, const std::vector<Item>& input) {
    Result backpack;
    for (size_t i = 0; i < input.size(); ++i) {
        const auto& item = input[i];
        double chance = 1 / (double)(input.size() + 1);
        if (RandDouble(0.0, 1.0) < chance) {
            continue;
        }
        if (backpack.weight + item.weight <= backpack_weight) {
            backpack.Insert(i, input);
        }
    }
    return backpack;
}


Result ExponentialGreedy(uint64_t n, uint64_t backpack_weight, const std::vector<Item>& input) {
    size_t first_elements = std::round(std::log2(1e8 / n)) + 1;
    first_elements = std::min(first_elements, n);

    Result best_backpack;

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

        best_backpack = std::max(best_backpack, another_result);
    }
    return best_backpack;
}

Result RandomGreedy(uint64_t n, uint64_t backpack_weight, const std::vector<Item>& input) {
    Result best_backpack;
    for (size_t i = 0; i < 1e8 / n; ++i) {
        best_backpack = std::max(RandomGreedyTmp(backpack_weight, input), best_backpack);
    }
    return best_backpack;
}


int main(int argc, char *argv[]) {
    assert(argc == 2);
    auto input = ReadFromFile(argv[1]);
    // хочу посортить входные данные,
    // при этом далее по индексам в посорченных данных хочу далее восстановить старые 
    std::vector<size_t> sorted_indices(input.n);
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(), [&](size_t lhs, size_t rhs) {
        return input.items[lhs].cost * input.items[lhs].weight > input.items[lhs].cost * input.items[lhs].weight;
    });
    std::vector<Item> sorted_items;
    sorted_items.reserve(input.n);
    for (auto index : sorted_indices) {
        sorted_items.push_back(input.items[index]);
    }
    input.items = std::move(sorted_items);

    std::vector<size_t> inverse_permutation(input.n);
    for (size_t i = 0; i < input.n; ++i) {
        inverse_permutation[sorted_indices[i]] = i;
    }


    auto result = std::max(RandomGreedy(input.n, input.backpack_weight, input.items), ExponentialGreedy(input.n, input.backpack_weight, input.items));
    std::cout << result.indices.size() << '\n';
    for (auto index : result.indices) {
        std::cout << inverse_permutation[index] << ' ';
    }
}

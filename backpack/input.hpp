#include <cstdint>

#include <vector>
#include <string>


struct Item {
    uint64_t cost;
    uint64_t weight;
};

struct Input {
    uint64_t backpack_weight;
    uint64_t n;
    std::vector<Item> items;
};

Input ReadFromFile(const char* file_path);

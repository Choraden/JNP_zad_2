#include "encstrset.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <utility>
#include <string>

using set_t = std::unordered_set<std::string>;
using map_t = std::unordered_map<unsigned long, set_t>;

// Rzeczy ukrywane przed światem zewnętrznym
namespace {
    map_t& get_global_map() {
        static auto *global_map = new map_t();
        return *global_map;
    }

    std::string encrypt(const char *value, const char *key) {
        // TODO
        return value;
    }

    std::string string_to_write_out(const char *str) {
        if (str == nullptr) {
            return "NULL";
        }
        return "\"" + std::string(str) + "\"";
    }
}

unsigned long jnp1::encstrset_new() {
    static unsigned long next_id = 0;
    map_t& global_map = get_global_map();
    global_map.insert(std::make_pair(next_id, set_t()));
    std::cout
        << "encstrset_new()" << std::endl
        << "encstrset_new: set #" << next_id << " created" << std::endl;
    return next_id++;
}

bool jnp1::encstrset_insert(unsigned long id,
                            const char *value, const char *key) {
    std::cout
            << "encstrset_insert(" << id << ", "
            << string_to_write_out(value) << ", "
            << string_to_write_out(key) << ")" << std::endl;
    if (value == nullptr) {
        std::cout << "encstrset_insert: invalid value (NULL)" << std::endl;
        return false;
    }

    // Wydobycie z mapy odpowiedniego zbioru
    map_t& global_map = get_global_map();
    auto set_it = global_map.find(id);
    if (set_it == global_map.end()) {
        std::cout
            << "encstrset_insert: set #" << id
            << " does not exist" << std::endl;
        return false;
    }

    // Wstawianie zaszyfrowanej wartości do zbioru
    std::string encrypted_value = encrypt(value, key);
    bool inserted_now = set_it->second.insert(encrypted_value).second;
    if (inserted_now) {
        std::cout
                << "encstrset_insert: set #" << id << ", cypher \""
                << encrypted_value << "\" inserted" << std::endl;
    }
    else {
        std::cout
                << "encstrset_insert: set #" << id << ", cypher \""
                << encrypted_value << "\" was already present" << std::endl;
    }
    return inserted_now;
}

void jnp1::encstrset_copy(unsigned long src_id, unsigned long dst_id) {
    std::cout
        << "encstrset_copy(" << src_id << ", "
        << dst_id << ")" << std::endl;

    // Wydobycie z mapy dwóch zbiorów
    map_t& global_map = get_global_map();
    auto src_it = global_map.find(src_id);
    if (src_it == global_map.end()) {
        std::cout
            << "encstrset_copy: set #" << src_id
            << " does not exist" << std::endl;
        return;
    }
    auto dst_it = global_map.find(dst_id);
    if (dst_it == global_map.end()) {
        std::cout
            << "encstrset_copy: set #" << dst_id
            << " does not exist" << std::endl;
        return;
    }

    // Przekopiowanie wszystkich elementów z jednego zbioru do drugiego
    for (const auto& to_copy : src_it->second) {
        bool inserted_now = dst_it->second.insert(to_copy).second;
        if (inserted_now) {
            std::cout
                << "encstrset_copy: cypher \"" << to_copy
                << "\" copied from set #" << src_id
                << " to set #" << dst_id << std::endl;
        }
        else {
            std::cout
                << "encstrset_copy: copied cypher \"" << to_copy
                << "\" was already present in set #" << dst_id << std::endl;
        }
    }
}
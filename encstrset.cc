#include "encstrset.h"
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <iomanip>

// Rzeczy ukrywane przed światem zewnętrznym
namespace {
#ifdef NDEBUG
    constexpr bool debug = false;
#else
    constexpr bool debug = true;
#endif

    using cypher_t = std::string;
    using set_t = std::unordered_set<cypher_t>;
    using set_id_t = unsigned long;
    using map_t = std::unordered_map<set_id_t, set_t>;
    using fun_args_t = std::vector<std::string>;

    namespace accessory {
        map_t& get_global_map() {
            static auto *global_map = new map_t();
            return *global_map;
        }

        std::ostream& get_stream() {
            static std::ios_base::Init init;
            static std::ostream *global_stream = debug ?
                                                 &std::cerr : new std::ostream (nullptr);
            return *global_stream;
        }

        cypher_t encrypt(const char *value, const char *key) {
            cypher_t res(value);
            if (key != nullptr) {
                std::string key_s(key);
                size_t key_len = key_s.length();
                if (key_len > 0) {
                    unsigned it = 0;
                    for (auto &i : res) {
                        i ^= key_s[it];
                        (++it) %= key_len;
                    }
                }
            }
            return res;
        }

        // Otacza tekst cudzysłowami
        // lub wypisuje "NULL", jeżeli str == nullptr
        std::string wrap_text(const char *str) {
            if (str == nullptr) {
                return "NULL";
            }

            std::ostringstream oss;
            oss << "\"" << str << "\"";
            return oss.str();
        }
    }

    namespace print_f {
        // Wypisuje opis wywołania funkcji
        // w postaci fun_name(args[0], args[1], ...) i koniec linii
        void desc_fun(const std::string &fun_name, fun_args_t args) {
            accessory::get_stream() << fun_name << "(";
            for (auto it = args.begin(); it != args.end(); it++) {
                accessory::get_stream() << *it;
                if (std::next(it) != args.end()) {
                    accessory::get_stream() << ", ";
                }
            }
            accessory::get_stream() << ")" << std::endl;
        }

        // Wypisuje szyfr w postaci szesnastkowej oddzielonej spacjami
        // i otoczonej cudzysłowami
        void print_cypher(const cypher_t &cypher) {
            static constexpr unsigned FILTER = 0xffu;
            static constexpr int NO_OF_DIGITS = 2;
            accessory::get_stream() << "\"" << std::hex << std::uppercase;
            char prev_fill = accessory::get_stream().fill('0');
            for (auto it = cypher.begin(); it != cypher.end(); it++) {
                accessory::get_stream() << std::setw(NO_OF_DIGITS)
                                        << (FILTER & static_cast<unsigned>(*it));
                if (std::next(it) != cypher.end()) {
                    accessory::get_stream() << " ";
                }
            }
            accessory::get_stream() << "\"" << std::dec << std::nouppercase;
            accessory::get_stream().fill(prev_fill);
        }

        // Wypisuje linię postaci
        // 'fun_name: set #id, cypher "..." (additional_text)'
        void desc_fun_res(const std::string &fun_name,
                          const set_id_t id,
                          const cypher_t &cypher,
                          const std::string &additional_text) {
            accessory::get_stream() << fun_name << ": set #"
                                    << id << ", cypher ";
            print_f::print_cypher(cypher);
            accessory::get_stream() << additional_text << std::endl;
        }

        // Wypisuje linię postaci 'fun_name: set #id (action)'
        void set_action(const std::string &fun_name,
                        const set_id_t id, const std::string &action) {
            accessory::get_stream() << fun_name << ": set #" << id << " "
                                    << action << std::endl;
        }

        void set_doesnt_exist(const std::string &fun_name, const set_id_t id) {
            print_f::set_action(fun_name, id, "does not exist");
        }

        void invalid_value(const std::string &fun_name) {
            accessory::get_stream() << fun_name << ": invalid value (NULL)"
                                    << std::endl;
        }

        void new_desc(const set_id_t id) {
            print_f::set_action("encstrset_new", id, "created");
        }

        void delete_desc(const set_id_t id) {
            print_f::set_action("encstrset_delete", id, "deleted");
        }

        void size_desc(const set_id_t id, const size_t size) {
            accessory::get_stream() << "encstrset_size: set #" << id
                                    << " contains " << size
                                    << " element(s)" << std::endl;
        }

        void insertion_desc(const set_id_t id,
                            const cypher_t &cypher, const bool inserted) {
            print_f::desc_fun_res("encstrset_insert", id, cypher,
                                  inserted ?
                                  " inserted" :
                                  " was already present");
        }

        void removing_desc(const set_id_t id,
                           const cypher_t &cypher, const bool removed) {
            print_f::desc_fun_res("encstrset_remove", id, cypher,
                                  removed ?
                                  " removed" :
                                  " was not present");
        }

        void testing_desc(const set_id_t id,
                          const cypher_t &cypher, const bool present) {
            print_f::desc_fun_res("encstrset_test", id, cypher,
                                  present ?
                                  " is present" :
                                  " is not present");
        }

        void clear_desc(const set_id_t id) {
            print_f::set_action("encstrset_clear", id, "cleared");
        }

        void copying_desc(const set_id_t src_id, const set_id_t dst_id,
                          const cypher_t &cypher, const bool copied) {
            accessory::get_stream() << "encstrset_copy:";
            if (!copied) {
                accessory::get_stream() << " copied";
            }
            accessory::get_stream() << " cypher ";
            print_f::print_cypher(cypher);
            if (copied) {
                accessory::get_stream() << " copied from set #" << src_id
                                        << " to set #" << dst_id << std::endl;
            }
            else {
                accessory::get_stream() << " was already present in set #"
                                        << dst_id << std::endl;
            }
        }

    }
}

unsigned long jnp1::encstrset_new() {
    print_f::desc_fun("encstrset_new", fun_args_t {});
    static set_id_t id = 0;
    map_t &global_map = accessory::get_global_map();
    global_map.insert(std::make_pair(id, set_t()));
    print_f::new_desc(id);
    return id++;
}

void jnp1::encstrset_delete(unsigned long id) {
    print_f::desc_fun("encstrset_delete", fun_args_t {std::to_string(id)});
    map_t &global_map = accessory::get_global_map();
    auto set_it = global_map.find(id);
    if (set_it == global_map.end()) {
        print_f::set_doesnt_exist("encstrset_delete", id);
        return;
    }
    global_map.erase(set_it);
    print_f::delete_desc(id);
}

size_t jnp1::encstrset_size(unsigned long id) {
    print_f::desc_fun("encstrset_size", fun_args_t {std::to_string(id)});
    map_t &global_map = accessory::get_global_map();
    auto set_it = global_map.find(id);
    if (set_it == global_map.end()) {
        print_f::set_doesnt_exist("encstrset_size", id);
        return 0;
    }
    size_t set_size = set_it->second.size();
    print_f::size_desc(id, set_size);
    return set_size;
}

bool jnp1::encstrset_insert(unsigned long id,
                            const char *value, const char *key) {
    print_f::desc_fun("encstrset_insert",
                      fun_args_t {std::to_string(id),
                                  accessory::wrap_text(value),
                                  accessory::wrap_text(key)});
    if (value == nullptr) {
        print_f::invalid_value("encstrset_insert");
        return false;
    }

    // Wydobycie z mapy odpowiedniego zbioru
    map_t &global_map = accessory::get_global_map();
    auto set_it = global_map.find(id);
    if (set_it == global_map.end()) {
        print_f::set_doesnt_exist("encstrset_insert", id);
        return false;
    }

    // Wstawianie zaszyfrowanej wartości do zbioru
    cypher_t cypher = accessory::encrypt(value, key);
    bool inserted_now = set_it->second.insert(cypher).second;
    print_f::insertion_desc(id, cypher, inserted_now);
    return inserted_now;
}

bool jnp1::encstrset_remove(unsigned long id,
                            const char *value, const char *key) {
    print_f::desc_fun("encstrset_remove",
                      fun_args_t {std::to_string(id),
                                  accessory::wrap_text(value),
                                  accessory::wrap_text(key)});
    if (value == nullptr) {
        print_f::invalid_value("encstrset_remove");
        return false;
    }

    // Wydobycie z mapy odpowiedniego zbioru
    map_t &global_map = accessory::get_global_map();
    auto set_it = global_map.find(id);
    if (set_it == global_map.end()) {
        print_f::set_doesnt_exist("encstrset_remove", id);
        return false;
    }

    // Usunięcie zaszyfrowanej wartości ze zbioru
    cypher_t cypher = accessory::encrypt(value, key);
    size_t removed_now = set_it->second.erase(cypher);
    print_f::removing_desc(id, cypher, removed_now > 0);
    return removed_now > 0;
}

bool jnp1::encstrset_test(unsigned long id,
                          const char *value, const char *key) {
    print_f::desc_fun("encstrset_test",
                      fun_args_t {std::to_string(id),
                                  accessory::wrap_text(value),
                                  accessory::wrap_text(key)});
    if (value == nullptr) {
        print_f::invalid_value("encstrset_test");
        return false;
    }

    // Wydobycie z mapy odpowiedniego zbioru
    map_t &global_map = accessory::get_global_map();
    auto set_it = global_map.find(id);
    if (set_it == global_map.end()) {
        print_f::set_doesnt_exist("encstrset_test", id);
        return false;
    }

    // Sprawdzenie czy zaszyfrowana wartość jest w zbiorze
    cypher_t cypher = accessory::encrypt(value, key);
    size_t exists = set_it->second.count(cypher);
    print_f::testing_desc(id, cypher, exists > 0);
    return exists > 0;
}

void jnp1::encstrset_clear(unsigned long id) {
    print_f::desc_fun("encstrset_clear", fun_args_t {std::to_string(id)});

    // Wydobycie z mapy odpowiedniego zbioru
    map_t &global_map = accessory::get_global_map();
    auto set_it = global_map.find(id);
    if (set_it == global_map.end()) {
        print_f::set_doesnt_exist("encstrset_clear", id);
        return;
    }

    set_it->second.clear();
    print_f::clear_desc(id);
}

void jnp1::encstrset_copy(unsigned long src_id, unsigned long dst_id) {
    print_f::desc_fun("encstrset_copy",
                      fun_args_t {std::to_string(src_id),
                                  std::to_string(dst_id)});

    // Wydobycie z mapy dwóch zbiorów
    map_t &global_map = accessory::get_global_map();
    auto src_it = global_map.find(src_id);
    if (src_it == global_map.end()) {
        print_f::set_doesnt_exist("encstrset_copy", src_id);
        return;
    }
    auto dst_it = global_map.find(dst_id);
    if (dst_it == global_map.end()) {
        print_f::set_doesnt_exist("encstrset_copy", dst_id);
        return;
    }

    // Przekopiowanie wszystkich elementów z jednego zbioru do drugiego
    for (const auto &to_copy : src_it->second) {
        bool inserted_now = dst_it->second.insert(to_copy).second;
        print_f::copying_desc(src_id, dst_id, to_copy, inserted_now);
    }
}
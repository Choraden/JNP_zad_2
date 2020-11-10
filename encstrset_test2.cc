#include "encstrset.h"

#ifdef NDEBUG
    #undef NDEBUG
#endif

#include <cassert>

namespace {
    unsigned long test() {
        unsigned long id = ::jnp1::encstrset_new();
        ::jnp1::encstrset_insert(id, "testowy", "1538221");
        return id;
    }

    unsigned long id = test();
}

int main() {
    ::jnp1::encstrset_insert(id, nullptr, nullptr);
    ::jnp1::encstrset_insert(id, nullptr, "ABC");
    assert(::jnp1::encstrset_test(id, "testowy", "1538221"));
    assert(!::jnp1::encstrset_test(id, "Testowy", "1538221"));
    assert(!::jnp1::encstrset_test(id, "testowy", ""));
    ::jnp1::encstrset_delete(id);
}

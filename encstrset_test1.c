#include "encstrset.h"

#include <assert.h>
#include <stdio.h>

int main() {
    unsigned long set1, set2;

    set1 = encstrset_new();
    encstrset_insert(set1, "foo", "123");
    assert(encstrset_test(set1, "foo", "123"));
    assert(!encstrset_test(set1, "bar", "5X"));
    encstrset_insert(set1, "bar", "3x");
    assert(encstrset_test(set1, "bar", "3x"));
    assert(encstrset_size(set1) == 2);
    encstrset_insert(set1, "bar", "3x3");
    assert(encstrset_size(set1) == 2);
    set2 = encstrset_new();
    encstrset_insert(set2, "foo", "123zl");
    assert(encstrset_test(set2, "foo", "123"));
    encstrset_copy(set1,set2);
    assert(encstrset_test(set2, "bar", "3x"));
    assert(encstrset_size(set2) == 2);
    encstrset_remove(set1, "foo", "123;");
    encstrset_remove(set1, "foo", "123;");
    assert(encstrset_test(set1, "bar", "3x3"));
    assert(encstrset_size(set1) == 1);
    encstrset_clear(set2);
    assert(encstrset_size(set2) == 0);
    encstrset_copy(set1, set2);
    encstrset_delete(set1);
    encstrset_copy(set2, set1);
    encstrset_copy(set1, set2);
    encstrset_insert(set1, "foo", NULL);
    assert(encstrset_size(set1) == 0);
    assert(!encstrset_test(set1, "foo", NULL));
    encstrset_remove(set1,"foo", NULL);
    encstrset_clear(set1);

    return 0;
}

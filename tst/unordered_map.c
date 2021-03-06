#include "test.h"
#include "../src/unordered_map.h"

static int compare_int(const void *const one, const void *const two)
{
    const int a = *(int *) one;
    const int b = *(int *) two;
    return a - b;
}

static int hash_count;

static unsigned long hash_int(const void *const key)
{
    hash_count++;
    unsigned long hash = 17;
    hash = 31 * hash + *(int *) key;
    return hash;
}

void test_unordered_map(void)
{
    unordered_map me = unordered_map_init(sizeof(int), sizeof(int),
                                         hash_int, compare_int);
    assert(unordered_map_size(me) == 0);
    assert(unordered_map_is_empty(me));
    int key = 4;
    int value = 9;
    unordered_map_put(me, &key, &value);
    assert(unordered_map_size(me) == 1);
    value = 5;
    unordered_map_put(me, &key, &value);
    assert(unordered_map_size(me) == 1);
    assert(!unordered_map_is_empty(me));
    assert(unordered_map_contains(me, &key));
    value = 0xdeadbeef;
    unordered_map_get(&value, me, &key);
    assert(value == 5);
    key = 7;
    assert(!unordered_map_contains(me, &key));
    unordered_map_put(me, &key, &value);
    assert(unordered_map_size(me) == 2);
    assert(unordered_map_contains(me, &key));
    int val_arr[10] = {5, 9, 4, -5, 0, 6, 1, 5, 7, 2};
    for (int i = 0; i < 10; i++) {
        unordered_map_put(me, &val_arr[i], &value);
        assert(unordered_map_contains(me, &val_arr[i]));
    }
    assert(unordered_map_size(me) == 9);
    for (int i = 0; i < 10; i++) {
        assert(unordered_map_contains(me, &val_arr[i]));
    }
    for (int i = -100; i < 100; i++) {
        bool contains = false;
        for (int j = 0; j < 10; j++) {
            if (val_arr[j] == i) {
                contains = true;
            }
        }
        assert(unordered_map_contains(me, &i) == contains);
    }
    int num = -3;
    assert(!unordered_map_remove(me, &num));
    assert(unordered_map_size(me) == 9);
    assert(!unordered_map_contains(me, &num));
    num = 6;
    assert(unordered_map_remove(me, &num));
    assert(unordered_map_size(me) == 8);
    assert(!unordered_map_contains(me, &num));
    num = 4;
    assert(unordered_map_remove(me, &num));
    assert(unordered_map_size(me) == 7);
    assert(!unordered_map_contains(me, &num));
    num = 7;
    assert(unordered_map_remove(me, &num));
    assert(unordered_map_size(me) == 6);
    assert(!unordered_map_contains(me, &num));
    num = 9;
    assert(unordered_map_remove(me, &num));
    assert(unordered_map_size(me) == 5);
    assert(!unordered_map_contains(me, &num));
    num = -5;
    assert(unordered_map_remove(me, &num));
    assert(unordered_map_size(me) == 4);
    assert(!unordered_map_contains(me, &num));
    num = 0;
    assert(unordered_map_remove(me, &num));
    assert(unordered_map_size(me) == 3);
    assert(!unordered_map_contains(me, &num));
    num = 1;
    assert(unordered_map_remove(me, &num));
    assert(unordered_map_size(me) == 2);
    assert(!unordered_map_contains(me, &num));
    num = 5;
    assert(unordered_map_remove(me, &num));
    assert(unordered_map_size(me) == 1);
    assert(!unordered_map_contains(me, &num));
    num = 2;
    assert(unordered_map_remove(me, &num));
    assert(unordered_map_size(me) == 0);
    assert(!unordered_map_contains(me, &num));
    // Add a lot of items and remove individually.
    for (int i = 5000; i < 6000; i++) {
        unordered_map_put(me, &i, &value);
        assert(unordered_map_contains(me, &i));
    }
    assert(unordered_map_size(me) == 1000);
    for (int i = 5000; i < 6000; i++) {
        unordered_map_remove(me, &i);
        assert(!unordered_map_contains(me, &i));
    }
    assert(unordered_map_size(me) == 0);
    assert(unordered_map_is_empty(me));
    unordered_map_clear(me);
    assert(unordered_map_size(me) == 0);
    assert(unordered_map_is_empty(me));
    // Add a lot of items and clear.
    for (int i = 5000; i < 6000; i++) {
        unordered_map_put(me, &i, &value);
        assert(unordered_map_contains(me, &i));
    }
    assert(unordered_map_size(me) == 1000);
    hash_count = 0;
    unordered_map_rehash(me);
    assert(hash_count == 1000);
    unordered_map_clear(me);
    key = 0xdeadbeef;
    assert(!unordered_map_remove(me, &key));
    assert(unordered_map_size(me) == 0);
    assert(unordered_map_is_empty(me));
    me = unordered_map_destroy(me);
    assert(me == NULL);
}

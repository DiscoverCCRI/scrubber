#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../scrubber/util.h"

unsigned int **rm_dupes(unsigned int **keys, unsigned int *num_keys);

static void test_rm_dupes(void **state) {
    // Test case 1
    unsigned int key1 = 1;
    unsigned int key2 = 2;
    unsigned int key3 = 2;
    unsigned int *keys[] = {&key1, &key2, &key3};
    unsigned int num_keys = 3;

    unsigned int **unique_keys = rm_dupes(keys, &num_keys);

    assert_int_equal(num_keys, 2);
    assert_int_equal(**unique_keys, 1);
    assert_int_equal(**(unique_keys + 1), 2);

    free(unique_keys);

    // Test case 2
    unsigned int key4 = 1;
    unsigned int key5 = 1;
    unsigned int key6 = 1;
    unsigned int *keys2[] = {&key4, &key5, &key6};
    unsigned int num_keys2 = 3;

    unsigned int **unique_keys2 = rm_dupes(keys2, &num_keys2);

    assert_int_equal(num_keys2, 1);
    assert_int_equal(**unique_keys2, 1);

    free(unique_keys2);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_rm_dupes),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}


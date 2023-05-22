/**
 * @file util.c
 */
#include <stdlib.h>

void sort(unsigned int **input, int left, int right) {
    // nothing to sort if left index is greater
    if (left >= right) {
        return;
    }
    // set start index as the pivot
    unsigned int *pivot = input[left];
    // set i index to element next to pivot
    int i = left + 1;
    // set j index as end element
    int j = right;

    // loop as long as i and j do not intersect
    while (i <= j) {
        // move i index right until i is greater than the pivot value
        while (i <= j && *input[i] <= *pivot) {
            i++; // increment i index
        }
        // move j to the left until j is less than the pivot value
        while (i <= j && *input[j] > *pivot) {
            j--; // decrement j index
        }
        // if i & j have not intersected
        if (i < j) {
            // swap the i and j index of the input array
            unsigned int *temp = input[i];
            input[i] = input[j];
            input[j] = temp;
        }
    }
    // once final position is reached, swap the established pivot(input[left])
    // with j index of array
    unsigned int *temp = input[left];
    input[left] = input[j];
    input[j] = temp;

    // recursively sort the left partition of the input
    sort(input, left, j - 1);
    // recursively sort the right parition of the input
    sort(input, j + 1, right);
}

unsigned int **rm_dupes(unsigned int **keys, unsigned int *num_keys) {
    unsigned int unique_count = 0;

    /** compare each element to the previous previous, traverse array of keys */
    for (unsigned int i = 1; i < *num_keys; i++) {
        /** check if current element, i, is not equal to previous element */
        if (**(keys + i) != **(keys + unique_count)) {
            /** update count of unique elements */
            unique_count++;
            /** store the unique element at the next element of they keys array
             */
            *(keys + unique_count) = *(keys + i);
        }
    }
    // update number of keys
    *num_keys = unique_count + 1;

    // reallocate memory to remove duplicates
    unsigned int **unique_keys =
        (unsigned int **)realloc(keys, (*num_keys) * sizeof(unsigned int *));

    return unique_keys;
}

/**
 * @file utils.h 
 */

#ifndef UTILS_H
#define UTILS_H


/**
 * @brief Function swap two integers
 */
void swap(int *i, int *j);

/**
 * @brief Merge sort-like function to sort a given array
 */
void sort(int array[], int lower, int upper);

/**
 * @brief Function to split/partition data
 */
int split(int array[], int, int lower, int upper);

/**
 * @brief Function to remove duplicates
 */
void remove(int array[]);



#endif


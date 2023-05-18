/**
 * @file utils.h
 * This file contains
 *  - sort() : quicksort function that sorts an array of unsigned ints in
 *    ascending order
 *  - rm_dupes() : function to remove duplicates from an array of unsigned ints
 */

#ifndef UTILS_H
#define UTILS_H

/*
 * @brief Sorts an array (ascending) of unsigned integers using quicksort
 * @param[in,out] input double pointer to array of unsigned integers
 * @param[in] left start index of left partition to be sorted
 * @param[in] right end index of right parition to be sorted
 * @details This function follows the traditional quicksort algorithm
 * implementation that partitions the data set into 2 subarrays using a pivot
 * elements. This function goes on to recursively sort on the 2 parititions
 * @note This function WILL modify the original array passed in
 */
void sort(unsigned int **input, int left, int right);

/**
 * @brief Remove duplicates from an array of unsigned integers
 * @param[in] keys an array of unsigned integers
 * @param[in] num_keys pointer to the number of elements in the array
 * @return Updated array without duplicates
 * @note This function WILL modify the original array passed in
 */
unsigned int **rm_dupes(unsigned int **keys, unsigned int *num_keys);

#endif

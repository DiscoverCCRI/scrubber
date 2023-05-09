/**
 * @file scrub.h
 * This file performs cleanup on the insertions made from the weather
 * station MQTT subscribe code. Detecting outliers/anomalies in data
 *
 * NOTE: this program requires root privledges to your MariaDB/MySQL
 * instance
 */

#ifndef SCRUB_H
#define SCRUB_H
#include <mysql.h>

#define BUFFER_SZ 128

/**
 * @brief Struct to hold SQL Table info
 */
typedef struct {
    /** array of columns names */
    char **columns;
    /** number of columns in table */
    unsigned int num_cols;
    /** number of rows in table */
    unsigned int num_rows;
    /** minimum allowed value for given column's rows, see constants.h */
    double **rng_min;
    /** maximum allowed value for given column's rows, see constants.h */
    double **rng_max;
    /** array of keys from detected outliers to drop data */
    unsigned int **keys;
    /** number of outliers */
    unsigned int num_keys;
} TableInfo;

/**
 * @brief Establishes a MariaDB/MySQL connection and returns a MYSQL pointer
 * @param addr server address
 * @param user user name
 * @param pass user password
 * @return A MYSQL pointer if the connection is successful, NULL otherwise
 */
MYSQL *db_connect(char *addr, char *user, char *pass, char *db);

/**
 * @brief Retrieve table information, column names, number of columns, number
 * of rows
 * @param con A pointer to a MySQL connection object.
 * @return A pointer to a dynamically allocated TableInfo struct that contains
 * the column names, number of columns and number of rows
 */
TableInfo *get_info(MYSQL *con);

/**
 * @brief Given an upper and lower bound, determine if a given column name's
 * rows are outside the given bounds and append an array with the keys of rows
 * not in range
 * @param connection pointer to a MySQL connection object
 * @param info_ptr pointer to TableInfo struct that holds data about table
 * @param column_name The name of the column to query
 * @param lower pointer to a double that holds the lower bound of the range
 * @param upper pointer to a double that holds the upper bound of the range
 * @return pointer to a TableInfo struct that contains the newly populated 
 * info_ptr->keys array
 */
TableInfo *outliers(MYSQL *connection, TableInfo *info_ptr, char *column_name,
              double *lower, double *upper);

void print_table_rows(MYSQL *connection, char *column_name);

/**
 * @brief Function for freeing memory allocated by TableInfo struct
 * @param info_ptr A pointer to the TableInfo struct whose memory is
 * to be freed
 */
void free_data(TableInfo *info_ptr);

#endif

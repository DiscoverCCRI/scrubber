/**
 * @file scrub.h
 * This file contains functions that perform the following
 *  - Establish MySQL/MariaDB connection
 *      * Requires username, password, database address, database name, and
 *        table name.
 *  - Retrieve database table information
 *      * Number of columns, columns names, number of rows,
 *  - Populate arrays containing the lower and upper bounds for acceptable
 * ranges for columns rows
 *  - Given the upper/lower bounds, detect id's of data on in this range.
 *    Populate an array containing the id's of these rows
 *  - Given the populated row of outliers, drop these id's from the database
 *    table and prompt for confirmation to do so
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
    unsigned int **keys; /** keys will always be positive */
    /** original number of keys (before removed dupes) */
    unsigned int orig_num_keys;
    /** number of outliers */
    unsigned int num_keys; /** # of keys will always be positive */

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
TableInfo *get_info(MYSQL *connection);

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

/**
 * @brief Drop a row from the MySQL/MariaDB table. This function is general
 * purpose with original intentions for dropping rows with cell(s) containing
 * values not within the ranges defined
 * @param connection A pointer to the MySQL connection object
 * @param info_ptr A pointer to TableInfo struct
 * @param row_id The ID of the row to be deleted
 */
void drop(MYSQL *connection, TableInfo *info_ptr, unsigned int row_id);

void print_table_rows(MYSQL *connection, char *column_name);

/**
 * @brief Function for freeing memory allocated by TableInfo struct
 * @param info_ptr A pointer to the TableInfo struct whose memory is
 * to be freed
 */
void free_data(TableInfo *info_ptr);

#endif

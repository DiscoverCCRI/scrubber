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
 * @brief Function for detecting outliers for weather readings
 */
// void outliers(MYSQL *connection, char *column_name, TableInfo *info_ptr);
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

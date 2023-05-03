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
    char **columns;
    unsigned int num_rows;
    unsigned int num_cols;
    // reference more?
} TableInfo;

typedef struct {

    char db_name[BUFFER_SZ];
    char db_addr[BUFFER_SZ];
    char db_user[BUFFER_SZ];
    char db_pass[BUFFER_SZ];
} DBInfo;

typedef struct {
    // keys of rows to drop
    unsigned int keys;
} OutlierData;

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
void outliers(MYSQL *connection, char *column_name, double lower, double upper);

void print_table_rows(MYSQL *connection, char *column_name);

/**
 * @brief Function for freeing memory allocated by TableInfo struct
 * @param info_ptr A pointer to the TableInfo struct whose memory is
 * to be freed
 */
void free_data(TableInfo *info_ptr);

#endif

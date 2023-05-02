/**
 * @file
 * This file performs cleanup on the insertions made from the weather
 * station MQTT subscribe code. Detecting outliers/anomalies in data
 *
 * NOTE: this program requires root privledges to your MariaDB/MySQL
 * instance
 */

#ifndef SCRUB_H
#define SCRUB_H
#include <mysql.h>

/**
 * @brief Struct to hold SQL Table info
 */
typedef struct {
    char **columns;
    unsigned int num_rows;
    // reference more?

} TableInfo;

/**
 * @brief Establishes a MariaDB/MySQL connection and returns a MYSQL pointer
 * @param addr server address
 * @param user user name
 * @param pass user password
 * @return A MYSQL pointer if the connection is successful, NULL otherwise.
 */
MYSQL *db_connect(char *addr, char *user, char *pass, char *db);

/**
 * @brief Returns an array of strings containing the names of the columns in
 * mqtt_data
 * @param con A pointer to a MySQL connection object.
 * @return A pointer to an array of strings containing the names of the columns
 *
 */
char **col_names(MYSQL *con);

void outliers(MYSQL *con, char **columns);

/**
 * @brief Frees memory allocated to col_arr and its elements
 * @param col_arr Array of strings containing table columns names
 */
void col_free(char **col_arr);

#endif

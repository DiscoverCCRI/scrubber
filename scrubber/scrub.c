/**
 * @file scrub.c
 */
#include "scrub.h"
#include "constants.h"
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "threadpool.hpp"

MYSQL *db_connect(char *addr, char *user, char *pass, char *db) {
    // mysql connection object
    MYSQL *con = mysql_init(NULL);

    // verify object is initialized
    if (!con) {
        fprintf(stderr, "[!] Error Initializing Connection Object : %s\n",
                mysql_error(con));
        exit(1);
    }

    // establish connection, close & exit if not successful
    if (!mysql_real_connect(con, addr, user, pass, db, 0, NULL, 0)) {
        fprintf(stderr, "[!] Error Connecting to Database : %s\n",
                mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    else {
        printf("\n[+] Connection SUCCESS\n");
    }

    // return connection object
    return con;
}

TableInfo *get_info(MYSQL *con) {
    // MYSQL result struct
    MYSQL_RES *result;
    // MYSQL field(column) struct
    MYSQL_FIELD *field;
    // define array of double to hold upper/lower bounds. see macro defs in
    // constants.h
    //
    // TODO: edit these upper/lower bound arrays to be more dynamic c
    //  - constants for S900 weather station are not universal to other metrics
    //  that could be analyzed
    //  - perhaps define file of constants and create way to read the values set
    //  in there?
    double low_val[] = {ID_LO,       TIME_LO,  TEMP_LO,     HUM_LO,
                        PRESS_LO,    LIGHT_LO, WIND_DIR_LO, WIND_DIR_LO,
                        WIND_DIR_LO, WIND_LO,  WIND_LO,     WIND_LO,
                        RAIN_LO,     RAIN_LO,  RAIN_LO,     RAIN_LO,
                        TEMP_LO,     DUMP_LO,  PM_LO,       PM_LO};

    double up_val[] = {ID_HI,       TIME_HI,  TEMP_HI,     HUM_HI,
                       PRESS_HI,    LIGHT_HI, WIND_DIR_HI, WIND_DIR_HI,
                       WIND_DIR_HI, WIND_HI,  WIND_HI,     WIND_HI,
                       RAIN_HI,     RAIN_HI,  RAIN_HI,     RAIN_HI,
                       TEMP_HI,     DUMP_HI,  PM_HI,       PM_HI};

    // query mqtt_data table
    if (mysql_query(con, "SELECT * FROM mqtt_data")) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }
    // stores query result in struct
    result = mysql_store_result(con);
    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }
    // allocate memory for TableInfo struct
    TableInfo *info_ptr = (TableInfo *)malloc(sizeof(TableInfo));
    // set number of columns = to return of mysql_num_fields()
    info_ptr->num_cols = mysql_num_fields(result);
    // set number of rows = to return of mysql_num_rows()
    info_ptr->num_rows = mysql_num_rows(result);
    // allocate memory for columns array
    info_ptr->columns = (char **)malloc(info_ptr->num_cols * sizeof(char *));
    // allocate memory for array holding lower bound ranges
    info_ptr->rng_min =
        (double **)malloc(info_ptr->num_cols * sizeof(double *));
    // allocated memory for array holding upper bound ranges
    info_ptr->rng_max =
        (double **)malloc(info_ptr->num_cols * sizeof(double *));

    // iterate through column names (fields) and populate the result array
    for (unsigned int i = 0; i < info_ptr->num_cols; i++) {
        // field for current column index
        field = mysql_fetch_field_direct(result, i);
        if (field == NULL) {
            fprintf(stderr, "%s\n", mysql_error(con));
            exit(1);
        }
        // allocate memory for result array elements and the lower/upper bound
        // array elements
        info_ptr->columns[i] = (char *)malloc(strlen(field->name) + 1);
        info_ptr->rng_min[i] = (double *)malloc(sizeof(double));
        info_ptr->rng_max[i] = (double *)malloc(sizeof(double));
        /* define upper and lower bounds, copy data from defined array into
         * struct upper/lower bound variables. NOTE: struct variables are
         * pointers
         */
        *info_ptr->rng_min[i] = low_val[i];
        *info_ptr->rng_max[i] = up_val[i];
        // copy column name to result array
        strcpy(info_ptr->columns[i], field->name);
    }
    // free allocated result memory
    mysql_free_result(result);
    // return result_arr;
    return info_ptr;
}

TableInfo *outliers(MYSQL *connection, TableInfo *info_ptr, char *column_name,
                    double *lower, double *upper) {
    MYSQL_RES *result;
    MYSQL_ROW row;
    unsigned int id;

    char query[1024];
    // query a column not in its range and the id of this row of data
    snprintf(query, sizeof(query),
             "SELECT id, %s FROM mqtt_data WHERE %s NOT BETWEEN %f AND %f",
             column_name, column_name, *lower, *upper);

    if (mysql_query(connection, query) != 0) {
        fprintf(stderr, "Error executing query: %s\n", mysql_error(connection));
        exit(1);
    }

    result = mysql_use_result(connection);
    /** static variables to keep count of the count and id's we query */
    static int key_idx = 0;
    static unsigned int count = 0;
    while ((row = mysql_fetch_row(result)) != NULL) {
        id = atoi(row[0]);
        double value = atof(row[1]);
        printf("%u: id=%u, %s=%f\n", ++count, id, column_name, value);

        // Reallocate the memory for keys array
        /** instead of continuously allocating memory reallocate to save the state between
         * function calls. this reallocated memory for array of keys */
        info_ptr->keys = realloc(info_ptr->keys, (key_idx + 1) * sizeof(unsigned int *));

        // allocate memory for the current key (element)
        info_ptr->keys[key_idx] = malloc(sizeof(unsigned int));
        // copy key value
        *info_ptr->keys[key_idx] = id;
        ++key_idx;
    }
    // populate number of keys with the index
    info_ptr->num_keys = key_idx;

    mysql_free_result(result);
    return info_ptr;
}

void scrubber(MYSQL *con, TableInfo *info_ptr) {
    printf("HELLOO\n");

    // traverse columns, for each column name, check if rows are in range
    // outliers(con, info_ptr, column_name);

    // average(con, info_ptr, column_name);
}

void print_table_rows(MYSQL *connection, char *column_name) {
    MYSQL_RES *result;
    MYSQL_ROW row;

    // Construct the SELECT query string
    char query[1024];
    snprintf(query, sizeof(query), "SELECT %s FROM mqtt_data LIMIT 5",
             column_name);

    // Execute the query and get the result set
    if (mysql_query(connection, query) != 0) {
        fprintf(stderr, "[!] Error executing query : %s\n",
                mysql_error(connection));
        return;
    }
    result = mysql_use_result(connection);

    // Print the column header
    printf("%s\n", column_name);

    // Print the rows
    while ((row = mysql_fetch_row(result)) != NULL) {
        printf("%s\n", row[0]);
    }

    // Free the result set
    mysql_free_result(result);
}

void free_data(TableInfo *info_ptr) {
    printf("Freeing memory...\n");
    // free column names and ranges
    for (unsigned int i = 0; i < info_ptr->num_cols; i++) {
        free(info_ptr->columns[i]);
        free(info_ptr->rng_min[i]);
        free(info_ptr->rng_max[i]);
    }   
    // free allocated keys of outliers
    for (unsigned int j = 0; j < info_ptr->num_keys; j++) {
        free(info_ptr->keys[j]);
    }

    // free arrays
    free(info_ptr->columns);
    free(info_ptr->rng_min);
    free(info_ptr->rng_max);
    free(info_ptr->keys);

    // free TableInfo struct
    free(info_ptr);

    printf("[+] Memory freed\n");
}

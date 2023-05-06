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
    double low_val[] = {ID_LO,    TIME_LO,     TEMP_LO,     HUM_LO,      PRESS_LO,
                      LIGHT_LO, WIND_DIR_LO, WIND_DIR_LO, WIND_DIR_LO, WIND_LO,
                      WIND_LO,  WIND_LO,     RAIN_LO,     RAIN_LO,     RAIN_LO,
                      RAIN_LO,  TEMP_LO,     DUMP_LO,     PM_LO,       PM_LO};

    double up_val[] = {ID_HI,    TIME_HI,     TEMP_HI,     HUM_HI,      PRESS_HI,
                      LIGHT_HI, WIND_DIR_HI, WIND_DIR_HI, WIND_DIR_HI, WIND_HI,
                      WIND_HI,  WIND_HI,     RAIN_HI,     RAIN_HI,     RAIN_HI,
                      RAIN_HI,  TEMP_HI,     DUMP_HI,     PM_HI,       PM_HI};
    // initialize as arrays
    //double *lower = low_val;
    //double *upper = up_val;

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
    // allocate memory for TableInfo struct, column names, num cols, num rows
    TableInfo *info_ptr = (TableInfo *)malloc(sizeof(TableInfo));
    info_ptr->num_cols = mysql_num_fields(result);
    info_ptr->num_rows = mysql_num_rows(result);
    info_ptr->columns = (char **)malloc(info_ptr->num_cols * sizeof(char *));
    info_ptr->rng_min = (double **)malloc(info_ptr->num_cols * sizeof(double *));
    info_ptr->rng_max = (double **)malloc(info_ptr->num_cols * sizeof(double *));

    // iterate through column names (fields) and populate the result array
    for (unsigned int i = 0; i < info_ptr->num_cols; i++) {
        // field for current column index
        field = mysql_fetch_field_direct(result, i);
        if (field == NULL) {
            fprintf(stderr, "%s\n", mysql_error(con));
            exit(1);
        }
        // allocate memory for result array elements and the lower/upper bound array elements
        info_ptr->columns[i] = (char *)malloc(strlen(field->name) + 1);
        info_ptr->rng_min[i] = (double *)malloc(sizeof(double));
        info_ptr->rng_max[i] = (double *)malloc(sizeof(double));
        /*define upper and lower bounds, copy data from defined array into
         * struct upper/lower bound variables. NOTE: struct variables are pointers
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

void outliers(MYSQL *connection, char *column_name, double *lower, double *upper) {
    MYSQL_RES *result;
    MYSQL_ROW row;

    char query[1024]; // edit this buffer?
    // snprintf(query, sizeof(query), "SELECT %s FROM mqtt_data WHERE %s >= %f
    // AND %s <= %f",
    //       column_name, column_name, lower, column_name, upper);
    snprintf(query, sizeof(query),
             "SELECT %s FROM mqtt_data WHERE %s NOT BETWEEN %f AND %f", column_name,
             column_name, *lower, *upper);

    // Execute the query and get the result set
    if (mysql_query(connection, query) != 0) {
        fprintf(stderr, "Error executing query: %s\n", mysql_error(connection));
        return;
    }   
    result = mysql_use_result(connection);
    // Print the rows
    int count = 1;
    while ((row = mysql_fetch_row(result)) != NULL) {
        int i = 0;
        double value = atof(row[i]);
        //if (value >= *lower && value <= *upper) {
            printf("%d : %s : %s\n", count++, column_name, row[i]);
        //}
        i++;
        
    }

    // Free the result set
    mysql_free_result(result);

}

/*
void outliers(MYSQL *connection, char *column_name, double lower,
              double upper) {
    MYSQL_RES *result;
    MYSQL_ROW row;

    char query[1024];
    // snprintf(query, sizeof(query), "SELECT %s FROM mqtt_data WHERE %s >= %f
    // AND %s <= %f",
    //       column_name, column_name, lower, column_name, upper);
    snprintf(query, sizeof(query),
             "SELECT %s FROM mqtt_data WHERE %s BETWEEN %f AND %f", column_name,
             column_name, lower, upper);

    // Execute the query and get the result set
    if (mysql_query(connection, query) != 0) {
        fprintf(stderr, "Error executing query: %s\n", mysql_error(connection));
        return;
    }
    result = mysql_use_result(connection);
    // Print the rows
    int count = 1;
    while ((row = mysql_fetch_row(result)) != NULL) {
        double value = atof(row[0]);
        if (value >= lower && value <= upper) {
            printf("%d : %s\n", count, row[0]);
        }
        count++;
    }

    // Free the result set
    mysql_free_result(result);
    // return count of outliers and keys for outliers to drop the row they are
    // in
    //
}*/

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
    for (unsigned int i = 0; i < info_ptr->num_cols; i++) {
        // free memory for column elements (col names)
        free(info_ptr->columns[i]);
        // free memory for bounds elements
        free(info_ptr->rng_min[i]);
        free(info_ptr->rng_max[i]);
    }
    // free memory for column array
    free(info_ptr->columns);
    // free memory for bounds arrays
    free(info_ptr->rng_min);
    free(info_ptr->rng_max);
    // free TableInfo struct memory
    free(info_ptr);
    printf("[+] Memory freed\n");
}

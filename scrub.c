/**
 * @file scrub.c
 */
#include "scrub.h"
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
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    // establish connection, close & exit if not successful
    if (!mysql_real_connect(con, addr, user, pass, db, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    else {
        printf("\n[+] Connection SUCCESS\n");
    }

    // return connection object
    return con;
}

TableInfo *get_data(MYSQL *con) {
    // MYSQL result struct
    MYSQL_RES *result;
    // MYSQL field(column) struct
    MYSQL_FIELD *field;
    // MYSQL row struct
    MYSQL_ROW row;

    unsigned int num_fields;
    // result array
    char **result_arr;

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
    TableInfo *info_ptr = (TableInfo*)malloc(sizeof(TableInfo));
    info_ptr->num_cols = mysql_num_fields(result);
    info_ptr->num_rows = mysql_num_rows(result);
    info_ptr->columns = (char**)malloc(info_ptr->num_cols * sizeof(char*));

    // iterate through column names (fields) and populate the result array
    for (unsigned int i = 0; i < info_ptr->num_cols; i++) {
        // field for current column index
        field = mysql_fetch_field_direct(result, i);

        if (field == NULL) {
            fprintf(stderr, "%s\n", mysql_error(con));
            exit(1);
        }
        // allocate memory for result array elements
        info_ptr->columns[i] = (char *)malloc(strlen(field->name) + 1);
        // copy column name to result array
        strcpy(info_ptr->columns[i], field->name);
    }
    // free allocated result memory
    mysql_free_result(result);
    //return result_arr;
    return info_ptr;
}

/*
outliers(MYSQL *con, column_name, ) {
    // define struct for min/max for ranges

    // for all 20 readings, establish baseline set of ranges, given
    // column names, check if the rows for a given column are in range
    for (i=0; i < num of rows for given col; i++) {
        if (row val is not in range) {
            // print some pretty table of information
        }
    }
    // think of way to return outliers in a easy to use format
}
*/

void outliers(MYSQL *con, char **columns) {
    mysql_query(con, "SELECT * FROM mqtt_data");

    MYSQL_RES *result = mysql_store_result(con);

    unsigned int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;
    MYSQL_FIELD *field;

    unsigned int name_field;
    char *field_name = "id";
    char *headers[num_fields];



    for (unsigned int i = 0; (field = mysql_fetch_field(result)); i++) {
        headers[i] = field->name;
        printf("<<<%s\n", field->name);
        if (strcmp(field_name, headers[i]) == 0) {
            name_field = i;
        }
        //printf("%s\n", headers[i]);
        //printf("  %s\n", columns[i]);
    }
    while ((row = mysql_fetch_row(result))) {
        printf("NAME: %s\n", row[name_field]);
    }


    mysql_free_result(result);
    // query; const char *
    // column name
    // combine the two? strcat?
}

// find average over a given time interval

/*
scrubber(MYSQL *con) {
    // parse column names

    // define outlier ranges for the 20 fields

    // given column name, check the colums rows for outliers

    // check for NULL/empty data?

    // <---->
    // Several optimizations can be made
    // - Merge sort approach, partition data (rows) of each column
    // - Spawn worker threads for equal parts of the rows
    // - ThreadPool, for each column name clean it up, each function call can
    // be a thread

}
*/

void col_free(TableInfo *info_ptr) {
    printf("Freeing memory...\n");
    for (unsigned int i = 0; i < info_ptr->num_cols; i++) {
        // free memory for column elements (col names)
        free(info_ptr->columns[i]);
    }
    // free memory for column array
    free(info_ptr->columns);

    // free TableInfo struct memory
    free(info_ptr);
    printf("[+] Memory freed\n");

}

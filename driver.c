/**
 * @file driver.c
 *
 * TODO: 
 *  - put climate reading into a struct for consistent referencing?
 *  - struct for MySQL DB info, connection, db, user, etc.?
 */
#include "scrub.h"
#include <stdio.h>
#include <stdlib.h>

void menu(TableInfo *info_ptr) {
    printf("---------------------------\n");
    printf("NODE READINGS : %d\n", info_ptr->num_cols);
    for (unsigned int i = 0; i < info_ptr->num_cols; i++) {
        printf("    %s\n", info_ptr->columns[i]);
    }   
    printf("---------------------------\n");
    printf("TOTAL ROWS : \n");
    printf("    %d\n", info_ptr->num_rows);
    printf("---------------------------\n");
}

/**
 * @brief main()
 */
int main() {
    char addr[] = "localhost";
    char user[] = "aba275";
    char pass[] = "";
    char db[] = "SEEED_WEATHER";
    char table[] = "mqtt_data";

    // establish connection
    MYSQL *db_con = db_connect(addr, user, pass, db);
    // query column names
    TableInfo *col_res = get_data(db_con);
    //char **col_arr = get_data(db_con);

    //outliers(db_con, col_arr);

    // display results
    menu(col_res);

    // call scrubber with connection object?

    // free memory allocated for column array
    col_free(col_res);
    //for (int i = 0; col_arr[i] != NULL; i++) {
        // free the memory allocated for each element
        //free(col_arr[i]);
    //}
    //free(col_res);

    // close connection
    mysql_close(db_con);
    return 0;
}


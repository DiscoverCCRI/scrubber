/**
 * @file
 * Scrubber Driver
 *
 * TODO: 
 *  - put climate reading into a struct for consistent referencing?
 *  - struct for MySQL DB info, connection, db, user, etc.?
 */
#include "scrub.h"
#include <stdio.h>
#include <stdlib.h>

void menu(char **columns) {
    printf("---------------------------\n");
    printf("NODE READINGS : \n");
    for (int i = 0; columns[i] != NULL; i++) {
        printf("    %s\n", columns[i]);
    }   
    printf("---------------------------\n");
    printf("TOTAL ROWS : \n");
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
    // MYSQL_RES *col_res = cols(db_con);
    char **col_arr = col_names(db_con);

    outliers(db_con, col_arr);

    // display results
    menu(col_arr);

    // call scrubber with connection object?

    // free memory allocated for column array
    col_free(col_arr);
    //for (int i = 0; col_arr[i] != NULL; i++) {
        // free the memory allocated for each element
        //free(col_arr[i]);
    //}
    //free(col_arr);

    // close connection
    mysql_close(db_con);
    return 0;
}


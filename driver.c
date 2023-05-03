/**
 * @file driver.c
 *
 * TODO:
 *  - put climate reading into a struct for consistent referencing?
 *  - struct for MySQL DB info, connection, db, user, etc.?
 *  - CLI options/flags? -d delete, confirm deletion
 *  - finding outliers and storing them in a data structure? likely
 *    something simple, array, LL, DLL
 *      - find outliers and store ID (col 0)
 *      - delete on confirmation
 *  - Statistical methods on data, range of data, display data
 *    over a given range, correlation?
 *  - Graphing, switch -> Python
 *
 * - Docs, CI/CD,,
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
    char db[] = "TEST_DB";
    // char table[] = "mqtt_data";

    // establish connection
    MYSQL *db_con = db_connect(addr, user, pass, db);
    // query column names
    TableInfo *data_res = get_info(db_con);
    // char **col_arr = get_data(db_con);

    // scrubber(db_con, info_ptr);

    //print_table_rows(db_con, data_res->columns[2]);
    outliers(db_con, data_res->columns[2], 21.03, 21.07);

    // display results
    menu(data_res);


    // free memory allocated for column array
    free_data(data_res);

    // close connection
    mysql_close(db_con);
    return 0;
}

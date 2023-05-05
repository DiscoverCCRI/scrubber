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
    printf("\n---------------------------\n");
    printf("SenseCAP900 NODE READINGS & BOUNDS : %d\n", info_ptr->num_cols);
    for (unsigned i = 0 ; i < info_ptr->num_cols; i++) {
        printf("%s MIN : %f MAX : %f \n", info_ptr->columns[i], 
                *info_ptr->rng_min[i], *info_ptr->rng_max[i]);
    }   
    printf("\n---------------------------\n");
    printf("TOTAL ROWS : \n");
    printf("    %d\n", info_ptr->num_rows);
    printf("\n---------------------------\n");
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

    // allocate memory for ranges
    // TableInfo *info_data =
    // define ranges for readings

    // establish connection
    MYSQL *db_con = db_connect(addr, user, pass, db);
    // get table information
    TableInfo *data_res = get_info(db_con);
/*
    for (unsigned i = 0 ; i < data_res->num_cols; i++) {
        printf("%s MIN: %f MAX: %f \n", data_res->columns[i], 
                *data_res->low[i], *data_res->up[i]);
    }
*/
    // print_table_rows(db_con, data_res->columns[2]);

    // temperature = 2, pressure = 4
    for (unsigned int i = 0; i < data_res->num_cols; i++) {
        outliers(db_con, data_res->columns[i], data_res->rng_min[i], 
                data_res->rng_max[i]);
    }


    //outliers(db_con, data_res->columns[2], 21.03, 21.07);

    // display results
    menu(data_res);

    // free memory allocated for column array
    free_data(data_res);

    // close connection
    mysql_close(db_con);
    return 0;
}

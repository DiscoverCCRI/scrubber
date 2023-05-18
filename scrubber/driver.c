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
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void menu(TableInfo *info_ptr) {
    printf("\n---------------------------\n");
    printf("SenseCAP900 NODE READINGS & BOUNDS : %d\n", info_ptr->num_cols);
    for (unsigned i = 0; i < info_ptr->num_cols; i++) {
        printf("%s MIN : %f MAX : %f \n", info_ptr->columns[i],
               *info_ptr->rng_min[i], *info_ptr->rng_max[i]);
    }
    printf("\n---------------------------\n");
    printf("OUTLIERS: %d\n", info_ptr->num_keys);
    printf("\n---------------------------\n");
    printf("TOTAL ROWS : \n");
    printf("    %d\n", info_ptr->num_rows);
    printf("\n---------------------------\n");
}

/**
 * @brief main()
 */
int main() {
    // TODO: put this data into its own struct?
    char addr[] = "localhost";
    char user[] = "aba275";
    char pass[] = "";
    char db[] = "TEST_DB2";
    // TODO: integrate db table name as a dynamic variable,
    // char table[] = "mqtt_data";

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

    // TODO: rethink logic on populating TableInfo keys array using outliers
    // function outliers function finds outliers of a given column's rows and
    // append the keys (row number) to an array to hold them (data_res->keys[i])
    for (unsigned int i = 0; i < data_res->num_cols; i++) {
        data_res = outliers(db_con, data_res, data_res->columns[i],
                            data_res->rng_min[i], data_res->rng_max[i]);
    }
    for (unsigned int i = 0; i < data_res->num_keys; i++) {
        printf("%u ", *data_res->keys[i]);
    }

    // TODO:
    //  - gather keys for outliers
    //  - sort keys in order
    //  - remove duplicate keys
    //      - edit free_data() function to account for the num_keys variable
    //        that gets changed from the duplicate removal function
    //  - drop keys (rows) given the array of keys
    //      - prompt for confirmation to drop x amount of rows
    //  - update menu to show more data?
    //
    //  STRETCH:
    //  - mean median mode of each columns?
    //      - display in menu next to column names
    //  - update menu to show in tabular format with column names (readings)
    //  taking up column 0 with information following it

    sort(data_res->keys, 0, data_res->num_keys - 1);

    printf("\nSORTED KEYS: \n");
    for (unsigned int i = 0; i < data_res->num_keys; i++) {
        printf("%u ", **(data_res->keys + i));
    }

    data_res->keys = rm_dupes(data_res->keys, &(data_res->num_keys));
    printf("\nSORTED KEYS + REMOVED DUPLICATES: \n");
    for (unsigned int i = 0; i < data_res->num_keys; i++) {
        printf("%u ", **(data_res->keys + i));
    }

    printf("\nDEBUG\n");
    // drop rows containing outliers
    // for (unsigned int i = 0; i < data_res->num_keys; i ++) {
    // TODO: dynamic way to eliminate passing MYSQL db_con object around
    //     drop(db_con, *data_res->keys[i]);
    //}

    // display results
    menu(data_res);

    // free memory allocated for column array
    free_data(data_res);

    // close connection
    mysql_close(db_con);
    return 0;
}

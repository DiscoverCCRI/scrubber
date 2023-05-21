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
#include <string.h>

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

void usage() {
    printf("    -o  find outliers based on constants.h\n");
    printf("    -m  print menu of database information\n");
    printf("    -h  headless mode, bypass entering options\n");
}

/**
 * @brief main()
 */
int main(int argc, char *argv[]) {
    // if invalid amount of args are passed in, print usage
    if (argc < 2) {
        printf("\n[!] Incorrect amount of arguments\n");
        usage();
        return 1;
    }

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

    // find outliers in the table
    for (unsigned int i = 0; i < data_res->num_cols; i++) {
        data_res = outliers(db_con, data_res, data_res->columns[i],
                            data_res->rng_min[i], data_res->rng_max[i]);
    }
    if (data_res->keys != 0) {
        // for (unsigned int i = 0; i < data_res->num_keys; i++) {
        //    printf("%u ", *data_res->keys[i]);
        //}
        // sort keys
        sort(data_res->keys, 0, data_res->num_keys - 1);
        // remove duplicate keys
        data_res->keys = rm_dupes(data_res->keys, &(data_res->num_keys));
    } else {
        printf("\n[+] No outliers found.\n");
    }

    // parse all arguments passed in
    for (int i = 0; i < argc; i++) {
        // if option = m (menu)
        if (strcmp(argv[i], "-m") == 0) {
            menu(data_res);
        }

        // if option = d (drop)
        if (strcmp(argv[i], "-d") == 0) {
            // drop rows containing outliers
            for (unsigned int i = 0; i < data_res->num_keys; i++) {
                // TODO: eliminate passing MYSQL db_con object around ?
                drop(db_con, data_res, *data_res->keys[i]);
            }
        }

        // if option = po (print outliers)
        if (strcmp(argv[i], "-po") == 0) {
        }

        // if option = D (Debug)
        if (strcmp(argv[i], "-D") == 0) {
            print_table_rows(db_con, data_res->columns[2]);
            printf("\nSORTED KEYS: \n");
            for (unsigned int i = 0; i < data_res->num_keys; i++) {
                printf("%u ", **(data_res->keys + i));
            }
            printf("\nSORTED KEYS + REMOVED DUPLICATES: \n");
            for (unsigned int i = 0; i < data_res->num_keys; i++) {
                printf("%u ", **(data_res->keys + i));
            }
        }
    }

    // free memory allocated for column array
    free_data(data_res);

    // close connection
    mysql_close(db_con);

    return 0;
}

int foo() {
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

    // print_table_rows(db_con, data_res->columns[2]);

    // find outliers in the table
    for (unsigned int i = 0; i < data_res->num_cols; i++) {
        data_res = outliers(db_con, data_res, data_res->columns[i],
                            data_res->rng_min[i], data_res->rng_max[i]);
    }
    if (data_res->keys != 0) {
        for (unsigned int i = 0; i < data_res->num_keys; i++) {
            printf("%u ", *data_res->keys[i]);
        }
        // sort keys
        sort(data_res->keys, 0, data_res->num_keys - 1);

        printf("\nSORTED KEYS: \n");
        for (unsigned int i = 0; i < data_res->num_keys; i++) {
            printf("%u ", **(data_res->keys + i));
        }
        // remove
        data_res->keys = rm_dupes(data_res->keys, &(data_res->num_keys));
        printf("\nSORTED KEYS + REMOVED DUPLICATES: \n");
        for (unsigned int i = 0; i < data_res->num_keys; i++) {
            printf("%u ", **(data_res->keys + i));
        }
        // drop rows containing outliers
        for (unsigned int i = 0; i < data_res->num_keys; i++) {
            // TODO: eliminate passing MYSQL db_con object around ?
            drop(db_con, data_res, *data_res->keys[i]);
        }
    } else {
        printf("\n[+] No outliers found.\n");
    }

    // display results
    menu(data_res);

    // free memory allocated for column array
    free_data(data_res);

    // close connection
    mysql_close(db_con);
    return 0;
}

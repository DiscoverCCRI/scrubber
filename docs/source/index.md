Scrubber Documentation
======================
The extensive source code documentation can be seen here https://discoverccri.github.io/scrubber/globals_func.html.

# Development Notes as of 05/21/2023 :
## Overview:
The goal of this scrubber util is to :
1. take a SQL/MariaDB username, password, DB name, DB address, and DB    
table name, and establish a connection to the database
2. get table information and populate information into the `TableInfo` struct that contains 
columns names, number of columns, number of rows, an array of lower bounds of ranges + a 
corresponding array of upper bounds, an array of keys of detected outliers and the number of 
outliers found. 
3. given a column name check if the rows of that column are within a define range, in this case they are
defined in [`constants.h`](https://github.com/DiscoverCCRI/scrubber/blob/main/scrubber/constants.h). 
4. drop rows that contain values not within the specified ranges

## What I was able to get done
This scrubber utility can be compiled into a binary using the [`Makefile`](https://github.com/DiscoverCCRI/scrubber/blob/main/scrubber/Makefile) in the [scrubber](https://github.com/DiscoverCCRI/scrubber/tree/main/scrubber) directory. Type `$ make` at the cmd line
to compile the files. From there, there are several command line flags created

```
    -O  find outliers based on constants.h
    -d  drop rows containing outliers
    -m  print menu of database information
    -h  headless mode, bypass entering options
```
Note, that these option rely on order they are passed in. `-d` that drops rows from the SQL database table is populated
by way of the `-O` flag which finds outliers to find in the first place. `-H` is meant to bypass all flags as well as 
the confirmation message the appears when dropping rows, this flag allows the utility to be run in the background without
user intervention. `-m` display a menu of information related to the database table, the constants defined in `constants.h`
that contains the lower/upper bounds for the 20 readings involved. 

## Unable to get done
The code overall is a bit modular but the tool itself is not as general purpose as I desired.
1. starting form the beginning of the `main()` function in [`driver.c`](https://github.com/DiscoverCCRI/scrubber/blob/main/scrubber/driver.c), credentials for the database could be stored
in a struct and possibly be asked as a prompt from the user
2. The command line arguments cannot be called in any order and overall are not well designed. Using some seperate
function for command line parsing may be ideal and storing that data in its own struct
3. For the outliers function in [`scrub.c`](https://github.com/DiscoverCCRI/scrubber/blob/main/scrubber/scrub.c), these
arrays that are created are not as modular as I had planned for. They require constants to be defined in [`constants.h`](https://github.com/DiscoverCCRI/scrubber/blob/main/scrubber/constants.h). A more dynamic way to find this as well as 
define these ranges would be very ideal.
```
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
```

## Known Bugs
This program makes use of dynamic memory allocation with `malloc()`, `realloc()`, and `free()`, in specific the issue
pertains the the `outliers()` function on this line
```
        // allocate memory for the current key (element)
        info_ptr->keys[key_idx] = malloc(sizeof(unsigned int));
```

Scrubber Documentation
======================

# Development Notes as of 05/21/2023 :
## GOAL:
The goal of this scrubber util is to :
    1. take a SQL/MariaDB username, password, DB name, DB address, and DB
    table name, and establish a connection to the database
    2. get table information and populate information into the `TableInfo` struct that contains 
    columns names, number of columns, number of rows, an array of lower bounds of ranges + a 
    corresponding array of upper bounds, an array of keys of detected outliers and the number of 
    outliers found. 




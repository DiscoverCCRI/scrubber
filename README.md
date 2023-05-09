SQL Database Cleanser & Analyzer

# Overview
A SQL database table analyzer and cleanser using MySQL C API. Accepts a SQL/MariaDB address, username, password, 
database name and table name to establish a connection and run a variety of cleaning/analysis methods.

See the Doxygen-based documentation here: https://discoverccri.github.io/scrubber/

# Details 
1. Establish connection to database with username, password, database name, table name, and the database instance 
address.
2. Populate table information, column names, number of columns, number of rows, array with lower bounds of reading
value, and an array with upper bounds of reading value. 
3. Given the arrays of lower/upper bounds and a column name, determine if the column's rows are outside this given range.
For each row out of range, it's key is appended to an array to possibly drop/analyze later on. 
The constants found for the lower and upper bounds are defined in `constants.h`

?
* Mean, Median, Mode, Range.
* Data over specified time/interval
?

# Install
## Dependencies
- MySQL C API

## Run


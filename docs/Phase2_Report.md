# Data System Project Report - Phase II

#### Team Members:

1. Polakampalli Sai Namrath (2020101063)
2. Vamshi Krishna Boyina (2020101071)
3. Konduru Nithin (2020101104)



##### Summary : 

In Phase II ,we made progress in implementing various commands like SORT, JOIN, ORDER BY and GROUP BY. This report provides an overview of the logic and implementation of key commands from this phase.


## Command: SORT

Syntax and Semantics Checks: We begin by validating the user's `SORT` command. This ensures the command is well-formed and the parameters are valid.
Internal Sorting Phase: The `sort()` method of the Table class is invoked. This method consists of two phases - the internal sorting phase and the merging phase.
- Internal Sorting: In this phase, we fetch a specified number of blocks (determined by the 'buffersize') at a time. These blocks are sorted within the main memory and written back as subfiles or 'runs'. 
- Merging Phase: In the merging phase, we recursively merge these subfiles. A priority queue is used to handle the merging process. We fetch (buffersize - 1) subfiles at a time, store them in the queue, and repeatedly pop rows from the queue to create larger subfiles. This process continues until only one subfile remains - the sorted data file.
- Block Accesses : O(nlogn) 

## Command: JOIN


Syntax and Semantics Checks: We perform checks on the `JOIN` command to ensure it's valid.
- Sorting Tables: Both tables involved in the join are sorted in ascending order based on the specified columns.
- Join Operations: We support three types of join operations - equality, less than, and greater than.
- For equality, we compare the specified columns in the current row for both tables. If they match, the row is joined and added to the resultant table. We advance one row for each table.
- For less than, the logic is similar, but we advance only the table with the lesser value.
- For greater than, we start from the end of the second table.
 Combined rows are stored in the resultant table.
- Block Accesses : worst case O(m*n) + O(nlogn) + O(mlogm) where m and n are the blockcounts of the tables.




## Command: ORDER BY


Syntax and Semantics Checks: We validate the `ORDER BY` command to ensure correctness.
- The `orderby()` method of the Table class is called, which sorts the table based on the specified column. The results are then copied to a new table.
- Block Accesses : Same as SORT. O(nlogn) 

## Command: GROUP BY

Syntax and Semantics Checks: We verify the correctness of the `GROUP BY` command.
- In the `groupby()` method, we first sort the table based on the group by column and calculate values corresponding to aggregate functions.
- We simultaneously calculate values for multiple aggregate functions and compare them with the provided conditions.
- Rows that meet the condition are added to the resultant table, consisting of the group by column value and the aggregate function values.
- Block Accesses: O(nlogn)

### Assumptions : 

 We assume that at least one row can fit completely in a block (page).
The block accesses are explained in terms of Big O notation complexity as they give us a better picture of the time than numericals.
 It is the responsibility of the tester to ensure that at least one row fits in one page for the final joined table.


### Learnings :

Minimizing page access is crucial for performance, given that reading/writing to disk is typically a slow operation.
We employed an optimal algorithm for transposing the matrix to minimize page write operations.
Performing syntactic and semantic checks before executing a command is crucial to ensure data integrity and prevent errors.
 We gained a deep understanding of how the external merge sort algorithm works.
Linking the `ORDER BY` and `GROUP BY` queries with the sort function helped minimize page accesses and improve overall performance.


### Team contribution:

As all three of us were aware of the interdependencies among the various components of the SimpleRa code, we decided to collaborate closely. We worked together by engaging in discussions, debugging sessions, and coding activities to collectively implement all the given commands in assignment.

### Project Phase -1

##### Team:
  Boyina Vamsi Krishna, 2020101071, vamsi.boyina@students.iiit.ac.in  
  Polakampalli Sai Namrath, 2020101063, polakampalli.namrath@students.iiit.ac.in  
  Konduru Nithin, 2020101104, konduru.nithin@students.iiit.ac.in  

#### Commands

### LOAD : 
number of blocks written: O(N^2/250)

To implement the `LOAD MATRIX <matrix name>` command, we followed  below process, making necessary changes in the codebase across several files. Here's the breakdown of our approach:

Server.cpp: Initially, we examined server.cpp, but found that it didn't require any changes to accommodate the new command. The existing structure was sufficient to handle it.

SyntaxParser.cpp: Next, we moved to SyntaxParser.cpp. Here, we focused on the SyntacticParseLOAD function within load.cpp that needed modifications to handle the `LOAD MATRIX <matrix name>` command.

load.cpp: Inside the SyntacticParseLoad function, we added a new function called SyntacticParseLOAD_M. This function was designed to specifically handle commands with three tokens, ensuring that the syntax was correct. It verifies that the command is structured as `LOAD MATRIX <matrix name>`.

Semantic Parsing: After successfully implementing the syntactic parser, we extended our changes to the semantic parser. This step involved ensuring that the semantic meaning of the `LOAD MATRIX` command was correctly interpreted and we did modifications in the same way as syntactic parser.

ExecuteLOAD Function: For the executeLOAD function for matrices, we wrote new function consisting of crucial adjustment. Instead of indexing columns by names, we adopted utilizing row indices ranging from 0 to n-1.

We have also added the relevant function names in executor.h, syntacticParser.h and SymanticParser.h for our approach.When we give incorrect commands it will give display message of "Syntactic error" and if files are not there,or tables is not there then it will display "Symantic error" message.


### PRINT :
number of blocks read: O(2N/25)

Server.cpp: Initially, we assessed the server.cpp file and determined that it did not require any modifications to accommodate the new command. The existing structure and functionality were already sufficient to handle it seamlessly.

SyntaxParser.cpp: Our next focus was on the SyntaxParser.cpp file. Within this file, we directed our attention to the SyntacticParsePRINT function, which needed adjustments to handle the `PRINT MATRIX <matrix name>` command.

print.cpp: Inside the SyntacticParsePRINT function in the PRINT.cpp file, we introduced a new function named SyntacticParsePRINT_M. This function was specifically designed to handle commands consisting of three tokens, ensuring that the syntax adhered to the expected format, i.e., `PRINT MATRIX <matrix name>`.

Semantic Parsing: Following the successful implementation of the syntactic parser, we proceeded to extend our changes to the semantic parser. Our goal was to ensure that the `"PRINT MATRIX"` command was interpreted correctly in terms of its semantic meaning. We applied modifications to the semantic parser in a manner similar to the syntactic parser.

ExecutePRINT Function: In the executePRINT function for matrices, we introduced a new function with a crucial adjustment. Instead of printing column names, we called a new function, Table::print_matrix(), which was responsible for displaying the matrix content. This change allowed us to output the matrix data effectively without column names.

Header Files: To maintain code organization and accessibility, we added relevant function declarations to the header files.

In executor.h, we included a declaration for the executePRINT_MATRIX function.
In syntacticParser.h, we added a declaration for the SyntacticParsePrint_M function.
In semanticParser.h, we included a declaration for the semantic parsing function related to the `"PRINT MATRIX"` command.
In terms of error handling, we ensured that the system provided appropriate feedback for incorrect commands. For syntactic errors, the system would display a "Syntax Error" message, and for issues related to missing files or tables, it would output a "Semantic error" message.



### EXPORT : 
number of blocks read: O(N^2/250)

Server.cpp: We began by examining the server.cpp file. After careful evaluation, we concluded that no changes were required in this file. The existing structure and functionality were already well-equipped to seamlessly handle the new `EXPORT MATRIX <matrix name>` command.

SyntaxParser.cpp: Our next focus was directed towards the SyntaxParser.cpp file. Within this file, we honed in on the SyntacticParseEXPORT function, which required adjustments to effectively process the `EXPORT MATRIX <matrix name>` command.

export.cpp: Inside the SyntacticParseExport function located in the EXPORT.cpp file, we introduced a new function named SyntacticParseEXPORT_M. This specialized function was purpose-built to handle commands comprising three tokens, ensuring that the syntax strictly adhered to the expected format, which is `EXPORT MATRIX <matrix name>`.

Semantic Parsing: Following the successful implementation of the syntactic parser, we proceeded to extend our changes to the semantic parser. Our objective was to ensure that the `"EXPORT MATRIX"` command was correctly interpreted in terms of its semantic meaning. We applied modifications to the semantic parser in a manner analogous to the syntactic parser.

ExecuteEXPORT Function: In the executeExport function designed for matrices, we introduced a new function called executeExport_M. This new function played a pivotal role by making a crucial adjustment. Instead of exporting the matrix with column indexes, it removed column indexes and proceeded to export the matrix data to a designated data folder.

Header Files: To maintain code organization and facilitate accessibility, we added relevant function declarations to the corresponding header files:

In executor.h, we included a declaration for the executeExport_M function.
In syntacticParser.h, we added a declaration for the SyntacticParseExport_M function.
In semanticParser.h, we included a declaration for the semantic parsing function associated with the `"EXPORT MATRIX"` command.
Regarding error handling, we ensured that the system provided clear and appropriate feedback for incorrect commands. For instances of syntactic errors, the system displayed a "Syntax error" message, while for issues related to missing files or tables, it produced a "Semantic error" message.


### RENAME
Server.cpp: We began by reviewing the server.cpp file. After thorough evaluation, we determined that no modifications were needed in this file. The existing structure and functionality were already well-suited to seamlessly handle the new `RENAME MATRIX <matrix_current_name> <matrix_new_name>` command.

SyntaxParser.cpp: Our primary focus shifted to the SyntaxParser.cpp file. Within this file, we specifically addressed the SyntacticParseRENAME function, which required adaptations to effectively process the `RENAME MATRIX <matrix_current_name> <matrix_new_name>` command.

rename.cpp: Within the RENAME.cpp file, we introduced a new function named SyntacticParseRENAME_M inside the SyntacticParseEXPORT function. This new function was custom-tailored to handle commands consisting of four tokens, ensuring strict adherence to the expected syntax, i.e., `RENAME MATRIX <matrix_current_name> <matrix_new_name>`.

Semantic Parsing: Building on the successful implementation of the syntactic parser, our next step was to extend our changes to the semantic parser. Our aim was to ensure that the "RENAME MATRIX" command was accurately interpreted in terms of its semantic meaning. Similar to the syntactic parser, we applied necessary modifications to the semantic parser.

ExecuteRENAME(): In the executeRENAME function designed for matrices, we introduced a new function named executeRENAMEM. This function played a pivotal role by making a significant adjustment. First, it changed the name of the matrix in the table catalog, then change the name of the table name to the new name. Then, it replaced the existing data file name with  with a new desired name in the Data/temp directory. We implemented a separate function called 'filechange' to manage the renaming of the data files.

Header Files: To maintain code organization and facilitate accessibility, we added the relevant function declarations to the corresponding header files:

In executor.h, we included a declaration for the executeRENAMEM function.
In syntacticParser.h, we added a declaration for the SyntacticParseRENAME_M function.
In semanticParser.h, we included a declaration for the semantic parsing function associated with the `"RENAME MATRIX"` command.
Regarding error handling, we ensured that the system provided clear and appropriate feedback for incorrect commands. In cases of syntactic errors, the system displayed a "Syntax error" message. For issues related to missing files or tables, it generated a "Semantic error" message.

### TRANSPOSE
number of blocks written & read: O(N^3/250)
number of blocks accessed: O(N^3/125)

Server.cpp: We began by reviewing the server.cpp file. After thorough evaluation, we determined that no modifications were needed in this file. The existing structure and functionality were already well-suited to seamlessly handle the new `TRANSPOSE MATRIX <matrix_name>` command.

SyntaxParser.cpp: Our primary focus shifted to the SyntaxParser.cpp file. Within this file, we specifically addressed the SyntacticParseTRANSPOSE function, which required adaptations to effectively process the `TRANSPOSE MATRIX <matrix_name>` command. This function was custom-tailored to handle commands consisting of three tokens, ensuring strict adherence to the expected syntax, i.e. `TRANSPOSE MATRIX <matrix_name>`. 

Semantic Parsing: Building on the successful implementation of the syntactic parser, our next step was to extend our changes to the semantic parser. Our aim was to ensure that the "RENAME MATRIX" command was accurately interpreted in terms of its semantic meaning. Similar to the syntactic parser, we applied necessary modifications to the semantic parser.

executeTRANSPOSE():  for this we declared new funtion "Table::transpose()" where we first cleared the BufferManager.pages (Deque) then we loaded the pages containing the indexes matrix[i][j] and matrix[j][i] for all i>j followed by the swap of the both values and finally writing back the changes in the page files. And for similarly for RowSize>PageSize case: we get matrix[i][j] value by first getting `i`th row starting page and then `j`th colomn page with required offset.

Header Files: To maintain code organization and facilitate accessibility, we added the relevant function declarations to the corresponding header files:
- In executor.h, we included a declaration for the executeTRANSPOSE function.
- In syntacticParser.h, we added a declaration for the SyntacticParseTRANSPOSE function.
- In semanticParser.h, we included a declaration for the semantic parsing function associated with the `"TRANSPOSE MATRIX"` command.

Regarding error handling, we ensured that the system provided clear and appropriate feedback for incorrect commands. In cases of syntactic errors, the system displayed a "Syntax error" message. For issues related to missing files or tables, it generated a "Semantic error" message.

### CHECKSYMMETRY
number of blocks read: O(N^3/250)
number of blocks accessed: O(N^3/125)

Server.cpp: We began by reviewing the server.cpp file. After thorough evaluation, we determined that no modifications were needed in this file. The existing structure and functionality were already well-suited to seamlessly handle the new `CHECKSYMMETRY <matrix_name>` command.

SyntaxParser.cpp: Our primary focus shifted to the SyntaxParser.cpp file. Within this file, we specifically addressed the SyntacticParseSYMMETRY function, which required adaptations to effectively process the `CHECKSYMMETRY <matrix_name>` command. This function was custom-tailored to handle commands consisting of two tokens, ensuring strict adherence to the expected syntax, i.e. `CHECKSYMMETRY <matrix_name>`. 

Semantic Parsing: Building on the successful implementation of the syntactic parser, our next step was to extend our changes to the semantic parser. Our aim was to ensure that the "CHECKSYMMETRY" command was accurately interpreted in terms of its semantic meaning. Similar to the syntactic parser, we created necessary function to the semantic parser.

executeSYMMETRY(): for this we declared a new function bool Table::symmetry() which operates similar to transpose funtion i.e. first we clear the bufferManager.pages(Deque) and push the pages containing matrix[i][j] and matrix[j][i] for all i>j and instead of swapping we compare these two values and return a bool value which represents the relation between matrix[i][j] and matrix[j][i] i.e (matrix[i][j] == matrix[j][i]) And for similarly for RowSize > PageSize case: we get matrix[i][j] value by first getting `i`th row starting page and then `j`th colomn page with required offset.

Header Files: To maintain code organization and facilitate accessibility, we added the relevant function declarations to the corresponding header files:
- In executor.h, we included a declaration for the executeTRANSPOSE function.
- In syntacticParser.h, we added a declaration for the SyntacticParseSYMMETRY function.
- In semanticParser.h, we included a declaration for the semantic parsing function associated with the `"CHECKSYMMETRY"` command.

Regarding error handling, we ensured that the system provided clear and appropriate feedback for incorrect commands. In cases of syntactic errors, the system displayed a "Syntax error" message. For issues related to missing files or tables, it generated a "Semantic error" message.

### COMPUTE
number of blocks written & read: O(N^3/250) + O(N^2/125)
number of blocks accessed: O(N^3/125) + O(N^2/125)

Server.cpp: We began by reviewing the server.cpp file. After thorough evaluation, we determined that no modifications were needed in this file. The existing structure and functionality were already well-suited to seamlessly handle the new `COMPUTE <matrix_name>` command.

SyntaxParser.cpp: Our primary focus shifted to the SyntaxParser.cpp file. Within this file, we specifically addressed the SyntacticParseCOMPUTE function, which required adaptations to effectively process the `COMPUTE <matrix_name>` command. This function was custom-tailored to handle commands consisting of two tokens, ensuring strict adherence to the expected syntax, i.e. `COMPUTE <matrix_name>`. 

Semantic Parsing: Building on the successful implementation of the syntactic parser, our next step was to extend our changes to the semantic parser. Our aim was to ensure that the "RENAME MATRIX" command was accurately interpreted in terms of its semantic meaning. Similar to the syntactic parser, we created necessary functions to the semantic parser.

executeCOMPUTE():  for this we first copied the original matrix relation into matrixName_RESULT relations declared a new function bool Table::compute() which operates similar to transpose funtion i.e. first we clear the bufferManager.pages(Deque) and push the pages containing matrix_RESULT[i][j](new matrix) and matrix[j][i] for all i>j and instead of swapping we subtract these two values and store it in new matrix i.e. (matrix_RESULT[i][j] = matrix_RESULT[i][j]-matrix[j][i]) And for similarly for RowSize > PageSize case: we get matrix[i][j] value by first getting `i`th row starting page and then `j`th colomn page with required offset.

Header Files: To maintain code organization and facilitate accessibility, we added the relevant function declarations to the corresponding header files:
- In executor.h, we included a declaration for the executeTRANSPOSE function.
- In syntacticParser.h, we added a declaration for the SyntacticParseCOMPUTE function.
- In semanticParser.h, we included a declaration for the semantic parsing function associated with the `"COMPUTE"` command.

Regarding error handling, we ensured that the system provided clear and appropriate feedback for incorrect commands. In cases of syntactic errors, the system displayed a "Syntax error" message. For issues related to missing files or tables, it generated a "Semantic error" message.

### Single Row - Multi Page Implementation
for the matrixes whose row can't fit in a single page we divied the single rows into multiple part where each part is of size 1000 bytes i.e. each part containing 250 integers. for this Implementation we added an aditional condition for `maxRowsPerBlock < 1` and then for every line read, we are storing the each 250 integers in a page and for the last set of integers which less than 250 in number we store them in a single page and for next row we will move to new page. for retriving the `j`th colomn in `i`th row we can use simple maths Page_colomn_index = j/250.

#### Learnings

Our learning are,

* We have gone through the code completely before implementing because it's essential to thoroughly assess the existing codebase. In some cases, the structure and functionality might already be sufficient to accommodate new commands, saving unnecessary modifications and potential complications.

* Modular Approach coding that is breaking down the implementation process into distinct modules helps in maintaining code organization and readability. This modular approach makes it easier to debug, test, and expand functionality.

* Implemented Specialized Functions that is designing specialized functions for handling specific command structures (e.g., SyntacticParseRENAME_M for three-token commands) helps enforce syntax rules and ensures that the code adheres to expected formats. This enhances code clarity and maintainability.

Semantic Interpretation: Semantic parsing plays a critical role in ensuring that commands are correctly interpreted in terms of their intended meaning. It's important to align semantic actions with the desired outcomes to ensure that the system behaves as expected.

Error Handling: Robust error handling is essential. Providing clear and informative error messages for both syntactic and semantic errors helps users understand and resolve issues. This contributes to a user-friendly and reliable system.


## Team contribution

As all three of us were aware of the interdependencies among the various components of the SimpleRa code, we decided to collaborate closely. We worked together by engaging in discussions, debugging sessions, and coding activities to collectively implement all the given commands in assignment.

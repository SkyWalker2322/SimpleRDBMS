#include"semanticParser.h"

void executeCommand();

void executeCLEAR();
void executeCROSS();
void executeDISTINCT();
void executeEXPORT();
void executeINDEX();
void executeJOIN();
void executeLIST();
void executeLOAD();
void executePRINT();
void executePROJECTION();
void executeRENAME();
void executeSELECTION();
void executeSORT();
void executeSOURCE();
void executeLOADM();
void executePRINTM();
void executeEXPORTM();
void executeRENAMEM();
void executeTRANSPOSE();
void executeSYMMETRY();
void executeCOMPUTE();
void executeGROUP();
void executeORDER();

void filechange(string old_name, string new_name);
void renameFilesInFolder(string folderPath,string oldName,  string newName);
bool evaluateBinOp(int value1, int value2, BinaryOperator binaryOperator);
void printRowCount(int rowCount);
vector<string> extractAggregateFun(string str);
void writeToPage(vector<vector<int>> &rows, int pgIndex, string tablename);
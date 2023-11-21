#include "global.h"

bool syntacticParseCOMPUTE()
{
    logger.log("syntacticParseCOMPUTE");
    if(tokenizedQuery.size() != 2){
        cout<<"SYNTAX ERROR SYMMETRY"<<endl;
        return false;
    }
    parsedQuery.queryType = COMPUTE;
    parsedQuery.transposeTableName = tokenizedQuery[1];
    return true;
}

bool semanticParseCOMPUTE()
{
    logger.log("semanticParseCOMPUTE");
    if (!tableCatalogue.isTable(parsedQuery.transposeTableName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }
    return true;
}

void executeCOMPUTE()
{
    logger.log("executeCOMPUTE");
    Table* table = tableCatalogue.getTable(parsedQuery.transposeTableName);
    
    // export A.csv
    table->makePermanent();

    //rename A to A_RESULT
    tableCatalogue.removerTable(table->tableName);
    // filechange(parsedQuery.transposeTableName, parsedQuery.transposeTableName + "_RESULT");
    renameFilesInFolder("../data/temp/" ,parsedQuery.transposeTableName , parsedQuery.transposeTableName+"_RESULT");
    table->tableName = parsedQuery.transposeTableName + "_RESULT";
    tableCatalogue.insertTable(table);

    // //LOAD A again
    Table *tab = new Table(parsedQuery.transposeTableName, true);
    if(tab->load_Matrix()){
        tableCatalogue.insertTable(tab);
        cout<< "Loaded Matrix BACK. Column Count: "<<tab->columnCount<<" Row Count: "<< tab->rowCount <<endl;
    }

    // //Now Apply COMPUTE on A_RESULT
    table->compute();

    return;
} 

void renameFilesInFolder(string folderPath,string oldName,  string newName) {
    
    int i =0;
    while(1){
        string base = folderPath;
        base += oldName;
        base += "_Page";
        
        base+= to_string(i);

        // try to find this page
        if(filesystem::exists(base)){
            // then rename
            auto newname = folderPath + newName + "_Page" + to_string(i);
            if(rename(base.c_str(), newname.c_str())!=0){
                cout<<"Problem in rename !"<<endl;
            }
        }
        else break;
        i+=1;
    }
        
}
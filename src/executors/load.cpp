#include "global.h"
/**
 * @brief 
 * SYNTAX: LOAD relation_name
 */
bool syntacticParseLOAD()
{
    if(tokenizedQuery.size()==2){
        logger.log("syntacticParseLOAD");
        if (tokenizedQuery.size() != 2)
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
        parsedQuery.queryType = LOAD;
        parsedQuery.loadRelationName = tokenizedQuery[1];
        // cout<<tokenizedQuery[1]<<" "<<true<<endl;
        return true;
    }
    else
    return syntacticParseLOAD_M();
}

bool syntacticParseLOAD_M()
{
    if(tokenizedQuery.size()!=3){
        cout<<"SYNTAX ERROR LOAD M"<<endl;
        return false;
    }
    parsedQuery.queryType = LOAD_M;
    parsedQuery.loadRelationName = tokenizedQuery[2];
    return true;
}

bool semanticParseLOAD()
{
    logger.log("semanticParseLOAD");
    if (tableCatalogue.isTable(parsedQuery.loadRelationName))
    {
        cout << "SEMANTIC ERROR: Relation already exists" << endl;
        return false;
    }

    if (!isFileExists(parsedQuery.loadRelationName))
    {
        cout << "SEMANTIC ERROR: Data file doesn't exist" << endl;
        return false;
    }
    return true;
}

void executeLOAD()
{
    logger.log("executeLOAD");

    Table *table = new Table(parsedQuery.loadRelationName);
    if (table->load())
    {
        tableCatalogue.insertTable(table);
        cout << "Loaded Table. Column Count: " << table->columnCount << " Row Count: " << table->rowCount << endl;
    }
    return;
}

void executeLOADM(){
    logger.log("executeLOADMatrix");
    Table *table = new Table(parsedQuery.loadRelationName , true);
    if(table->load_Matrix()){
        tableCatalogue.insertTable(table);
        cout<< "Loaded Matrix. Column Count: "<<table->columnCount<<" Row Count: "<< table->rowCount <<endl;
    }
    return;
}
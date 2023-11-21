#include "global.h"

bool syntacticParseSYMMETRY()
{
    logger.log("syntacticParseSYMMETRY");
    if(tokenizedQuery.size() != 2){
        cout<<"SYNTAX ERROR SYMMETRY"<<endl;
        return false;
    }
    parsedQuery.queryType = CHECKSYMMETRY;
    parsedQuery.transposeTableName = tokenizedQuery[1];
    return true;
}

bool semanticParseSYMMETRY()
{
    logger.log("semanticParseSymmetry");
    if (!tableCatalogue.isTable(parsedQuery.transposeTableName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }
    return true;
}

void executeSYMMETRY()
{
    logger.log("executeSYMMETRY");
    Table* table = tableCatalogue.getTable(parsedQuery.transposeTableName);
    auto check = table->symmetry();
    
    if(check) cout<<"TRUE"<<endl;
    else cout<<"FALSE"<<endl;

    return;
} 

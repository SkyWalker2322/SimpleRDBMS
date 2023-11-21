#include "global.h"

bool syntacticParseTRANSPOSE()
{
    logger.log("syntacticParseTranspose");
    if(tokenizedQuery.size() !=3){
        cout<<"SYNTAX ERROR TRANSPOSE"<<endl;
        return false;
    }
    parsedQuery.queryType = TRANSPOSE;
    parsedQuery.transposeTableName = tokenizedQuery[2];
    return true;
}

bool semanticParseTRANSPOSE()
{
    logger.log("semanticParseTRanspose");
    if (!tableCatalogue.isTable(parsedQuery.transposeTableName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }
    return true;
}

void executeTRANSPOSE()
{
    logger.log("executeTRANSPOSE");
    Table* table = tableCatalogue.getTable(parsedQuery.transposeTableName);
    table->transpose();
    return;
}
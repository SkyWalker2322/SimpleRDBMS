#include "global.h"

/**
 * @brief 
 * SYNTAX: EXPORT <relation_name> 
 */

bool syntacticParseEXPORT()
{
    if(tokenizedQuery.size()==2){
        logger.log("syntacticParseEXPORT");
        if (tokenizedQuery.size() != 2)
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
        parsedQuery.queryType = EXPORT;
        parsedQuery.exportRelationName = tokenizedQuery[1];
        return true;
    }
    else 
    return syntacticParseEXPORTM();
}

bool syntacticParseEXPORTM(){
    if(tokenizedQuery.size()!=3){
        cout<<"SYNTAX ERROR inside MATRIX"<<endl;
        return false;
    }
    parsedQuery.queryType = EXPORT_M;
    parsedQuery.exportRelationName = tokenizedQuery[2];
    return true;
}

bool semanticParseEXPORT()
{
    logger.log("semanticParseEXPORT");
    //Table should exist
    if (tableCatalogue.isTable(parsedQuery.exportRelationName))
        return true;
    cout << "SEMANTIC ERROR: No such relation exists" << endl;
    return false;
}

void executeEXPORT()
{
    logger.log("executeEXPORT");
    Table* table = tableCatalogue.getTable(parsedQuery.exportRelationName);
    table->makePermanent();
    return;
}

void executeEXPORTM()
{
    logger.log("executeEXPORTMATRIX called");
    Table* table = tableCatalogue.getTable(parsedQuery.exportRelationName);
    table->makePermanent();
    return;
}
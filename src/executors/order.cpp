#include "global.h"

/**
 * @brief 
 * SYNTAX: R <- CROSS relation_name relation_name
 */
bool syntacticParseORDER()
{
    logger.log("syntacticParseORDER");
    if (tokenizedQuery.size()!=8)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = ORDER;
    parsedQuery.orderResultRelationName = tokenizedQuery[0];
    parsedQuery.orderconditionattribute = tokenizedQuery[4];
    parsedQuery.ordercondition = tokenizedQuery[5];
    parsedQuery.orderRelationName = tokenizedQuery[7];

    if(!(parsedQuery.ordercondition=="ASC" || parsedQuery.ordercondition=="DESC")){
        cout << "SYNTAX ERROR: Incorrect Ordering Strategy";
        return false;
    }
    return true;
}

bool semanticParseORDER()
{
    logger.log("semanticParseGROUP");
    //Both tables must exist and resultant table shouldn't
    if (tableCatalogue.isTable(parsedQuery.orderResultRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation already exists" << endl;
        return false;
    }

    if (!tableCatalogue.isTable(parsedQuery.orderRelationName))
    {
        cout << "SEMANTIC ERROR: Group relations don't exist" << endl;
        return false;
    }

    if(!tableCatalogue.isColumnFromTable(parsedQuery.orderconditionattribute, parsedQuery.orderRelationName)){
        cout<<"SEMANTIC ERROR: Column doesn't exist in relation"<<endl;
        return false;
    }
    return true;
}

void executeORDER()
{
    logger.log("executeORDER");

    Table table1 = *(tableCatalogue.getTable(parsedQuery.orderRelationName));

    table1.extendedSort({parsedQuery.orderRelationName},{parsedQuery.ordercondition});
    int blockCount = table1.blockCount;
    vector<string> columns = table1.columns;
    vector<vector<int>> rows;
    
    // create the new table
    Table *resultantTable = new Table(parsedQuery.orderResultRelationName, columns);
    tableCatalogue.insertTable(resultantTable);

    // write everytime a page is filled up
    // we always have two columns of ints ~ 2*4 bytes ~ 8 bytes of a row
    // block size is basically 125
    int blocksize = 1*1000 / (columns.size()*4);
    int pgIndex = 0;

    for(int i=0;i<blockCount;i++){
        auto page = bufferManager.getPage(table1.tableName,i);
        rows.clear();
        int cnt = page.rowCount;
        for(int ind = 0; ind < cnt; ind++)
            rows.push_back(page.rows[ind]);

        for(auto x:rows){
            for(auto y:x) cout<<y<<" ";
            cout<<endl;
        }

        if(rows.size() == blocksize){
            resultantTable->rowsPerBlockCount.emplace_back(rows.size());
            resultantTable->rowCount+= rows.size();
            writeToPage(rows, pgIndex, resultantTable->tableName);
            pgIndex ++;
        }
        
    }
    
    //if anything left
    resultantTable->rowsPerBlockCount.emplace_back(rows.size());
    resultantTable->rowCount+= rows.size();
    writeToPage(rows, pgIndex, resultantTable->tableName);
    rows.clear();

    return;
}
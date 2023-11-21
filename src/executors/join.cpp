#include "global.h"
/**
 * @brief 
 * SYNTAX: R <- JOIN relation_name1, relation_name2 ON column_name1 bin_op column_name2
 */
bool syntacticParseJOIN()
{
    logger.log("syntacticParseJOIN");
    if (tokenizedQuery.size() != 9)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = JOIN;
    parsedQuery.joinResultRelationName = tokenizedQuery[0];
    parsedQuery.joinFirstRelationName = tokenizedQuery[3];
    parsedQuery.joinSecondRelationName = tokenizedQuery[4];
    parsedQuery.joinFirstColumnName = tokenizedQuery[6];
    parsedQuery.joinSecondColumnName = tokenizedQuery[8];
    parsedQuery.joinBinaryOperator= tokenizedQuery[7];
    
    return true;
}

bool semanticParseJOIN()
{
    logger.log("semanticParseJOIN");

    if (tableCatalogue.isTable(parsedQuery.joinResultRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation already exists" << endl;
        return false;
    }

    if (!tableCatalogue.isTable(parsedQuery.joinFirstRelationName) || !tableCatalogue.isTable(parsedQuery.joinSecondRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    if (!tableCatalogue.isColumnFromTable(parsedQuery.joinFirstColumnName, parsedQuery.joinFirstRelationName) || !tableCatalogue.isColumnFromTable(parsedQuery.joinSecondColumnName, parsedQuery.joinSecondRelationName))
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }
    return true;
}

void executeJOIN()
{
    
    logger.log("executeJOIN");
    
    Table table1 = *(tableCatalogue.getTable(parsedQuery.joinFirstRelationName));
    Table table2 = *(tableCatalogue.getTable(parsedQuery.joinSecondRelationName));
    table1.extendedSort({parsedQuery.joinFirstColumnName},{"ASC"});
    if(parsedQuery.joinBinaryOperator == ">=" || parsedQuery.joinBinaryOperator == ">")
        table2.extendedSort({parsedQuery.joinSecondColumnName},{"ASC"});
    else if(parsedQuery.joinBinaryOperator == "<=" || parsedQuery.joinBinaryOperator == "<")
        table2.extendedSort({parsedQuery.joinSecondColumnName},{"DESC"});
    else 
        table2.extendedSort({parsedQuery.joinSecondColumnName},{"ASC"});

    int t1blockcount=table1.blockCount;
    int t2blockcount=table2.blockCount;

    int columnIndex1 = table1.getColumnIndex(parsedQuery.joinFirstColumnName);
    int columnIndex2 = table2.getColumnIndex(parsedQuery.joinSecondColumnName);
    
    vector<string> columns;
    columns.insert(columns.end() , table1.columns.begin() , table1.columns.end());
    columns.insert(columns.end() , table2.columns.begin() , table2.columns.end());
    Table *resultantTable = new Table(parsedQuery.joinResultRelationName, columns);
    tableCatalogue.insertTable(resultantTable);
    // cout << parsedQuery.joinBinaryOperator << endl;
    // write everytime a page is filled up
    // we always have two columns of ints ~ 2*4 bytes ~ 8 bytes of a row
    // block size is basically 125
    int blocksize = 1*1000 / (columns.size()*4);
    int pgIndex = 0;

    vector<vector<int>> results;

    for(int i=0;i<t1blockcount;i++){
        auto page1 = bufferManager.getPage(table1.tableName, i);
        int rowcount1 = page1.rowCount;
        vector<vector<int>> rows1 = page1.rows;
        for(int j=0;j<t2blockcount;j++){
            auto page2 = bufferManager.getPage(table2.tableName, j);
            int rowCount2 = page2.rowCount;
            vector<vector<int>> rows2 = page2.rows;

            if(parsedQuery.joinBinaryOperator=="=="){
                for(int r1=0;r1<rowcount1;r1++){
                    for(int r2=0;r2<rowCount2;r2++){
                        if(rows1[r1][columnIndex1]==rows2[r2][columnIndex2]){
                            vector<int> result;
                            for(auto c:rows1[r1]){
                                result.push_back(c);
                            }
                            for(auto c:rows2[r2]){
                                result.push_back(c);
                            }
                            results.push_back(result);
                            if(results.size() == blocksize){
                                resultantTable->rowsPerBlockCount.emplace_back(results.size());
                                resultantTable->rowCount+= results.size();
                                writeToPage(results, pgIndex, resultantTable->tableName);
                                pgIndex ++;
                            }
                        }
                        else if(rows1[r1][columnIndex1] < rows2[r2][columnIndex2])
                            break; // its asc by default so once u cross the point ur not finding it again
                    }
                }
            }
            else if(parsedQuery.joinBinaryOperator=="!="){
                for(auto r1=0;r1<rowcount1;r1++){
                    for(auto r2=0;r2<rowCount2;r2++){
                        if(rows1[r1][columnIndex1]!=rows2[r2][columnIndex2]){
                            vector<int> result;
                            for(auto c:rows1[r1]){
                                result.push_back(c);
                            }
                            for(auto c:rows2[r2]){
                                result.push_back(c);
                            }
                            results.push_back(result);
                            if(results.size() == blocksize){
                                resultantTable->rowsPerBlockCount.emplace_back(results.size());
                                resultantTable->rowCount+= results.size();
                                writeToPage(results, pgIndex, resultantTable->tableName);
                                pgIndex ++;
                            }
                        }
                    }
                }
            }
            else if(parsedQuery.joinBinaryOperator==">="){
                for(auto r1=0;r1<rowcount1;r1++){
                    for(auto r2=0;r2<rowCount2;r2++){
                        if(rows1[r1][columnIndex1]>=rows2[r2][columnIndex2]){
                            vector<int> result;
                            for(auto c:rows1[r1]){
                                result.push_back(c);
                            }
                            for(auto c:rows2[r2]){
                                result.push_back(c);
                            }
                            results.push_back(result);
                            if(results.size() == blocksize){
                                resultantTable->rowsPerBlockCount.emplace_back(results.size());
                                resultantTable->rowCount+= results.size();
                                writeToPage(results, pgIndex, resultantTable->tableName);
                                pgIndex ++;
                            }
                        }
                        else break;
                    }
                }
            }
            else if(parsedQuery.joinBinaryOperator==">"){
                for(auto r1=0;r1<rowcount1;r1++){
                    for(auto r2=0;r2<rowCount2;r2++){
                        if(rows1[r1][columnIndex1]>rows2[r2][columnIndex2]){
                            vector<int> result;
                            for(auto c:rows1[r1]){
                                result.push_back(c);
                            }
                            for(auto c:rows2[r2]){
                                result.push_back(c);
                            }
                            results.push_back(result);
                            if(results.size() == blocksize){
                                resultantTable->rowsPerBlockCount.emplace_back(results.size());
                                resultantTable->rowCount+= results.size();
                                writeToPage(results, pgIndex, resultantTable->tableName);
                                pgIndex ++;
                            }
                        }
                        else break;
                    }
                }
            }
            else if(parsedQuery.joinBinaryOperator=="<="){
                for(auto r1=0;r1<rowcount1;r1++){
                    for(auto r2=0;r2<rowCount2;r2++){
                        if(rows1[r1][columnIndex1]<=rows2[r2][columnIndex2]){
                            vector<int> result;
                            for(auto c:rows1[r1]){
                                result.push_back(c);
                            }
                            for(auto c:rows2[r2]){
                                result.push_back(c);
                            }
                            results.push_back(result);
                            if(results.size() == blocksize){
                                resultantTable->rowsPerBlockCount.emplace_back(results.size());
                                resultantTable->rowCount+= results.size();
                                writeToPage(results, pgIndex, resultantTable->tableName);
                                pgIndex ++;
                            }
                        }
                        else break;
                    }
                }
            }
            else if(parsedQuery.joinBinaryOperator=="<"){
                for(auto r1=0;r1<rowcount1;r1++){
                    for(auto r2=0;r2<rowCount2;r2++){
                        if(rows1[r1][columnIndex1]<rows2[r2][columnIndex2]){
                            vector<int> result;
                            for(auto c:rows1[r1]){
                                result.push_back(c);
                            }
                            for(auto c:rows2[r2]){
                                result.push_back(c);
                            }
                            results.push_back(result);
                            if(results.size() == blocksize){
                                resultantTable->rowsPerBlockCount.emplace_back(results.size());
                                resultantTable->rowCount+= results.size();
                                writeToPage(results, pgIndex, resultantTable->tableName);
                                pgIndex ++;
                            }
                        }
                        else break;
                    }
                }
            }
        }
    }
    //if anything left
    resultantTable->rowsPerBlockCount.emplace_back(results.size());
    resultantTable->rowCount+= results.size();
    writeToPage(results, pgIndex, resultantTable->tableName);
    results.clear();

    // cout<<table1.maxRowsPerBlock<<endl;
    // cout<<resultantTable->rowCount<<endl;
    // resultantTable.
    return;
}
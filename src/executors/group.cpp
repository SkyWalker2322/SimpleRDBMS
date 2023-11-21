#include "global.h"

/**
 * @brief 
 * SYNTAX: R <- CROSS relation_name relation_name
 */
bool syntacticParseGROUP()
{
    set<string> bin_op = {"<", "<=", ">", ">=", "==", "!="};
    logger.log("syntacticParseGROUP");
    int n = tokenizedQuery.size();
    if (tokenizedQuery.size() < 13)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    string finalValue= "";
    for(int i=10;i<=n-3;i++){
        finalValue+=tokenizedQuery[i];
    }
    tokenizedQuery[10] = finalValue;
    parsedQuery.queryType = GROUP;
    parsedQuery.groupResultRelationName = tokenizedQuery[0];
    parsedQuery.groupSortColumnName = tokenizedQuery[4];
    parsedQuery.groupInputRelationName = tokenizedQuery[6];
    parsedQuery.groupFirstAggregateColumnName = tokenizedQuery[8];
    parsedQuery.groupBinaryOperator = tokenizedQuery[9];
    parsedQuery.groupConditionValue = tokenizedQuery[10];
    parsedQuery.groupSecondAggregateColumnName = tokenizedQuery[n-1];

    if(bin_op.find(parsedQuery.groupBinaryOperator)==bin_op.end()){
        cout << "SYNTAX ERROR: Incorrect Binary Operator";
        return false;
    }
    return true;
}

bool semanticParseGROUP()
{
    logger.log("semanticParseGROUP");
    //Both tables must exist and resultant table shouldn't
    if (tableCatalogue.isTable(parsedQuery.groupResultRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation already exists" << endl;
        return false;
    }

    if (!tableCatalogue.isTable(parsedQuery.groupInputRelationName))
    {
        cout << "SEMANTIC ERROR: Group relations don't exist" << endl;
        return false;
    }

    if(!tableCatalogue.isColumnFromTable(parsedQuery.groupSortColumnName, parsedQuery.groupInputRelationName)){
        cout<<"SEMANTIC ERROR: Column doesn't exist in relation"<<endl;
        return false;
    }
    return true;
}
vector<string> extractAggregateFun(string str){
    std::istringstream iss(str);
    vector<string> tokens(2,"");
    if (std::getline(iss, tokens[0], '(') && std::getline(iss, tokens[1], ')')) {

    } else {
        std::cout << "Invalid format: Unable to tokenize the string." << std::endl;
    }
    return tokens;
}

void writeToPage(vector<vector<int>> &rows, int pgIndex, string tablename){
    
    if(rows.size() !=0){
        bufferManager.writePage(tablename, pgIndex, rows, rows.size());
        rows.clear();
    }
    return ;
}

void executeGROUP()
{
    logger.log("executeGROUP");

    Table table1 = *(tableCatalogue.getTable(parsedQuery.groupInputRelationName));
    unordered_map<int,int> MAX_map;
    unordered_map<int,int> MIN_map;
    unordered_map<int,int> SUM_map;
    unordered_map<int,int> COUNT_map;
    unordered_map<int,double> AVG_map;

    auto tokens = extractAggregateFun(parsedQuery.groupFirstAggregateColumnName);
    string aggregate1 = tokens[0];
    string column1 = tokens[1];
    tokens = extractAggregateFun(parsedQuery.groupSecondAggregateColumnName);
    string aggregate2 = tokens[0];
    string column2 = tokens[1];

    table1.extendedSort({parsedQuery.groupSortColumnName},{"ASC"});
    int blockCount = table1.blockCount;
    int columnIndex1 = table1.getColumnIndex(column1);
    int conditionIndex = table1.getColumnIndex(parsedQuery.groupSortColumnName);
    int conditionValue = stoi(parsedQuery.groupConditionValue);
    int columnIndex2 = table1.getColumnIndex(column2);
    for(int i=0;i<blockCount;i++){
        auto page = bufferManager.getPage(table1.tableName, i);
        int cnt = page.rowCount;
        vector<vector<int>> rows = page.rows; 
        for(int j = 0;j < cnt; j++){
            auto row  = rows[j];
            COUNT_map[row[conditionIndex]]++;

            if(MIN_map.find(row[conditionIndex])==MIN_map.end())
                MIN_map[row[conditionIndex]] = row[columnIndex1];
            else if(MIN_map[row[conditionIndex]] > row[columnIndex1])
                MIN_map[row[conditionIndex]] = row[columnIndex1];

            if(MAX_map.find(row[conditionIndex])==MAX_map.end())
                MAX_map[row[conditionIndex]] = row[columnIndex1];
            else if(MAX_map[row[conditionIndex]] < row[columnIndex1])
                MAX_map[row[conditionIndex]] = row[columnIndex1];
            
            SUM_map[row[conditionIndex]] += row[columnIndex1];
            
            AVG_map[row[conditionIndex]] = SUM_map[row[conditionIndex]] / COUNT_map[row[conditionIndex]];
        }
    }
    vector<string> columns = {parsedQuery.groupSortColumnName,aggregate2+column2};
    vector<vector<int>> rows;
    cout<<"count is "<< COUNT_map[0]<<endl;
    int targetVal = stoi(parsedQuery.groupConditionValue);
    // create the new table
    Table *resultantTable = new Table(parsedQuery.groupResultRelationName, columns);
    tableCatalogue.insertTable(resultantTable);

    // write everytime a page is filled up
    // we always have two columns of ints ~ 2*4 bytes ~ 8 bytes of a row
    // block size is basically 125
    int blocksize = 1*1000 / (2*4);
    int pgIndex = 0;

    if(aggregate1=="MAX"){
        for(auto i:MAX_map){
            if(parsedQuery.groupBinaryOperator=="=="){
                if(i.second == targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator=="!="){
                if(i.second != targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator=="<="){
                if(i.second <= targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator==">="){
                if(i.second >= targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator=="<"){
                if(i.second < targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator==">"){
                if(i.second > targetVal){ 
                    rows.push_back({i.first,i.second});}
            }

            if(rows.size() == blocksize){
                resultantTable->rowsPerBlockCount.emplace_back(rows.size());
                resultantTable->rowCount+= rows.size();
                writeToPage(rows, pgIndex, resultantTable->tableName);
                pgIndex ++;
            }
        } 
    }
    else if(aggregate1=="MIN"){
        for(auto i:MIN_map){
            if(parsedQuery.groupBinaryOperator=="=="){
                if(i.second == targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator=="!="){
                if(i.second != targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator=="<="){
                if(i.second <= targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator==">="){
                if(i.second >= targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator=="<"){
                if(i.second < targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator==">"){
                if(i.second > targetVal){ 
                    rows.push_back({i.first,i.second});}
            }

            if(rows.size() == blocksize){
                resultantTable->rowsPerBlockCount.emplace_back(rows.size());
                resultantTable->rowCount+= rows.size();
                writeToPage(rows, pgIndex, resultantTable->tableName);
                pgIndex ++;
            }
        }
    }
    else if(aggregate1 == "SUM"){
        for(auto i:SUM_map){
            
            if(parsedQuery.groupBinaryOperator=="=="){
                if(i.second == targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator=="!="){
                if(i.second != targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator=="<="){
                if(i.second <= targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator==">="){
                if(i.second >= targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator=="<"){
                if(i.second < targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator==">"){
                if(i.second > targetVal){ 
                    rows.push_back({i.first,i.second});}
            }

            if(rows.size() == blocksize){
                resultantTable->rowsPerBlockCount.emplace_back(rows.size());
                resultantTable->rowCount+= rows.size();
                writeToPage(rows, pgIndex, resultantTable->tableName);
                pgIndex ++;
            }
        }
    }
    else if(aggregate1=="AVG"){
        for(auto j:AVG_map){
            pair<int,int>  i; i.first = j.first;
            i.second = ceil(j.second);
            i.second = ceil(i.second);
            if(parsedQuery.groupBinaryOperator=="=="){
                if(i.second == targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator=="!="){
                if(i.second != targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator=="<="){
                if(i.second <= targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator==">="){
                if(i.second >= targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator=="<"){
                if(i.second < targetVal)
                    rows.push_back({i.first,i.second});
            }
            else if(parsedQuery.groupBinaryOperator==">"){
                if(i.second > targetVal){ 
                    rows.push_back({i.first,i.second});}
            }

            if(rows.size() == blocksize){
                resultantTable->rowsPerBlockCount.emplace_back(rows.size());
                resultantTable->rowCount+= rows.size();
                writeToPage(rows, pgIndex, resultantTable->tableName);
                pgIndex ++;
            }
        }
    }
    

    resultantTable->rowsPerBlockCount.emplace_back(rows.size());
    resultantTable->rowCount+= rows.size();
    writeToPage(rows, pgIndex, resultantTable->tableName);
    rows.clear();
    
    // resultantTable.
    return;
}
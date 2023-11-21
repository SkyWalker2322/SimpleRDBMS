#include"global.h"
/**
 * @brief File contains method to process SORT commands.
 * 
 * syntax:
 * R <- SORT relation_name BY column_name IN sorting_order
 * 
 * sorting_order = ASC | DESC 
 */
bool syntacticParseSORT(){
    logger.log("syntacticParseSORT");
    if(tokenizedQuery.size()!= 8 || tokenizedQuery[4] != "BY" || tokenizedQuery[6] != "IN"){
        cout<<"SYNTAX ERROR"<<endl;
        return false;
    }
    parsedQuery.queryType = SORT;
    parsedQuery.sortResultRelationName = tokenizedQuery[0];
    parsedQuery.sortRelationName = tokenizedQuery[3];
    parsedQuery.sortColumnName = tokenizedQuery[5];
    string sortingStrategy = tokenizedQuery[7];
    if(sortingStrategy == "ASC")
        parsedQuery.sortingStrategy = ASC;
    else if(sortingStrategy == "DESC")
        parsedQuery.sortingStrategy = DESC;
    else{
        cout<<"SYNTAX ERROR"<<endl;
        return false;
    }
    return true;
}

// SORT <table_name> BY <column_name1, column_name2,..., column_namek> IN
// <ASC|DESC, ASC|DESC,..., ASC|DESC>

bool syntacticParseMERGESORT(){
    logger.log("syntacticPARSEMERGESORT");
    // find index of BY and IN
    int in_index = 0;
    for(int i =0;i<tokenizedQuery.size();i++){
        if(tokenizedQuery[i] == "IN") in_index = i;
    }
    // number of columns = in - by -1
    // number of ASC = size - in - 1
    if(!(tokenizedQuery[2] == "BY" && in_index - 3 == tokenizedQuery.size() - in_index -1 )){
        cout<<"SYNTAX ERROR Column Length != Tags"<<endl;
        return false;
    }

    parsedQuery.queryType = MERGESORT;
    parsedQuery.mergeSortColumns.clear();
    parsedQuery.sortRelationName = tokenizedQuery[1];
    for(int i =3;i<in_index;i++){
        parsedQuery.mergeSortColumns.push_back(tokenizedQuery[i]);
    }
    parsedQuery.sortingStrategies.clear();
    for(int i=in_index+1; i<tokenizedQuery.size(); i++){
        if(tokenizedQuery[i] == "ASC")
            parsedQuery.sortingStrategies.push_back(ASC);
        else if(tokenizedQuery[i] == "DESC")
            parsedQuery.sortingStrategies.push_back(DESC);
        else{
            cout<<"SYNTAX ERROR Invalid Tag"<<endl;
            return false;
        }
    }

    return true;
}

bool semanticParseSORT(){
    logger.log("semanticParseSORT");

    if(tableCatalogue.isTable(parsedQuery.sortResultRelationName)){
        cout<<"SEMANTIC ERROR: Resultant relation already exists"<<endl;
        return false;
    }

    if(!tableCatalogue.isTable(parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Relation doesn't exist"<<endl;
        return false;
    }

    if(!tableCatalogue.isColumnFromTable(parsedQuery.sortColumnName, parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Column doesn't exist in relation"<<endl;
        return false;
    }

    return true;
}

bool semanticParseMERGESORT(){
    logger.log("semanticParseMERGESORT");

    if(!tableCatalogue.isTable(parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Relation doesn't exist"<<endl;
        return false;
    }
    for(int i=0;i<parsedQuery.mergeSortColumns.size();i++){
        if(!tableCatalogue.isColumnFromTable(parsedQuery.mergeSortColumns[i] , parsedQuery.sortRelationName)){
            cout<<"SEMANTIC ERROR: Column doesn't exist in relation "+parsedQuery.mergeSortColumns[i]<<endl;
            return false;
        }
    }
    return true;
}

void executeSORT(){
    logger.log("executeSORT");
    Table* table = tableCatalogue.getTable(parsedQuery.sortRelationName);
    vector<string> typ;
    for(auto x:parsedQuery.sortingStrategies){
        if(x== ASC){
            typ.push_back("ASC");
        }
        else { typ.push_back("DESC");}
    }
    if(!table->extendedSort(parsedQuery.mergeSortColumns, typ)){
        cout<<"Sort Failed!"<<endl;
    }

    // update csv
    table->makePermanent();
    
    return;
}
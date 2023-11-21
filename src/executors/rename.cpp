#include "global.h"
/**
 * @brief 
 * SYNTAX: RENAME column_name TO column_name FROM relation_name
 */
bool syntacticParseRENAME()
{
    if(tokenizedQuery.size()==6){
        logger.log("syntacticParseRENAME");
        if (tokenizedQuery.size() != 6 || tokenizedQuery[2] != "TO" || tokenizedQuery[4] != "FROM")
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
        parsedQuery.queryType = RENAME;
        parsedQuery.renameFromColumnName = tokenizedQuery[1];
        parsedQuery.renameToColumnName = tokenizedQuery[3];
        parsedQuery.renameRelationName = tokenizedQuery[5];
        return true;
    }
    else
    return syntacticParseRENAME_M();
}

bool syntacticParseRENAME_M(){
    if(tokenizedQuery.size() != 4){
        cout<<"SYNTAX ERROR inside MATRIX"<<endl;
        return false;
    }
    parsedQuery.queryType= RENAME_M;
    parsedQuery.renameFromColumnName = tokenizedQuery[2];
    parsedQuery.renameToColumnName = tokenizedQuery[3];
    return true;
}

bool semanticParseRENAME()
{
    logger.log("semanticParseRENAME");

    if (!tableCatalogue.isTable(parsedQuery.renameRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    if (!tableCatalogue.isColumnFromTable(parsedQuery.renameFromColumnName, parsedQuery.renameRelationName))
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }

    if (tableCatalogue.isColumnFromTable(parsedQuery.renameToColumnName, parsedQuery.renameRelationName))
    {
        cout << "SEMANTIC ERROR: Column with name already exists" << endl;
        return false;
    }
    return true;
}
bool semanticParseRENAME_M()
{
    logger.log("semanticParseRENAME");

    if (!tableCatalogue.isTable(parsedQuery.renameFromColumnName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    return true;
}


void executeRENAME()
{
    logger.log("executeRENAME");
    Table* table = tableCatalogue.getTable(parsedQuery.renameRelationName);
    table->renameColumn(parsedQuery.renameFromColumnName, parsedQuery.renameToColumnName);
    return;
}

void executeRENAMEM()
{
    logger.log("RENAME MATRIX ");
    Table* table = tableCatalogue.getTable(parsedQuery.renameFromColumnName);
    tableCatalogue.removerTable(table->tableName);
    filechange(parsedQuery.renameFromColumnName, parsedQuery.renameToColumnName);
    table->tableName = parsedQuery.renameToColumnName;
    tableCatalogue.insertTable(table);
    
    return;
}

void filechange(string old_name, string new_name) {
    std::string folder_path = "../data/temp/";
   

    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (std::filesystem::is_regular_file(entry)) {
            std::string old_filename = entry.path().filename().string();
            
            // Check if the file name starts with the old name
            if (old_filename.find(old_name) == 0) {
                std::string new_filename = new_name + old_filename.substr(old_name.length());
                try {
                    std::filesystem::rename(entry.path(), entry.path().parent_path() / new_filename);
                    // std::cout << "Renamed: " << old_filename << " to " << new_filename << std::endl;
                } catch (const std::exception& ex) {
                    std::cerr << "Error renaming file: " << ex.what() << std::endl;
                }
            }
        }
    }

}
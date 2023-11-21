#include "global.h"
/**
 * @brief Construct a new Page object. Never used as part of the code
 *
 */
Page::Page()
{
    this->pageName = "";
    this->tableName = "";
    this->pageIndex = -1;
    this->rowCount = 0;
    this->columnCount = 0;
    this->rows.clear();
}

/**
 * @brief Construct a new Page:: Page object given the table name and page
 * index. When tables are loaded they are broken up into blocks of BLOCK_SIZE
 * and each block is stored in a different file named
 * "<tablename>_Page<pageindex>". For example, If the Page being loaded is of
 * table "R" and the pageIndex is 2 then the file name is "R_Page2". The page
 * loads the rows (or tuples) into a vector of rows (where each row is a vector
 * of integers).
 *
 * @param tableName 
 * @param pageIndex 
 */
Page::Page(string tableName, int pageIndex)
{
    logger.log("Page::Page");
    auto tmp = tableCatalogue.getTable(tableName);
    Table table = *tmp;
    
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->pageName = "../data/temp/" + this->tableName + "_Page" + to_string(pageIndex);
    
    // copyin(tmp, table);

    this->columnCount = table.columnCount;
    uint maxRowCount = table.maxRowsPerBlock;

    

    vector<int> row(columnCount, 0);
    this->rows.assign(maxRowCount, row);
    
    logger.log(to_string(maxRowCount));
    logger.log(to_string(this->columnCount));
    logger.log(to_string(table.rowsPerBlockCount.size()));

    ifstream fin(pageName, ios::in);
    this->rowCount = table.rowsPerBlockCount[pageIndex];
    logger.log("rowcount......");
    logger.log(to_string(this->rowCount));
    int number;
    
    for (uint rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < columnCount; columnCounter++)
        {
            fin >> number;
            
            this->rows[rowCounter][columnCounter] = number;
        }
        
    }
    

    fin.close();
}

Page::Page(string tableName, int pageIndex, bool special)
{
    logger.log("Page::Page special version");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->pageName = "../data/temp/" + this->tableName + "_Page" + to_string(pageIndex);
    auto tmp = tableCatalogue.getTable(tableName);
    
    Table table = *tmp;

    this->columnCount = 250;
    vector<int> row; 

    ifstream fin(pageName, ios::in);
    this->rowCount = 1;
    int number;
    
    while(fin>>number)
    {
        row.emplace_back(number);
    }
    this->columnCount = row.size();
    this->rows.emplace_back(row);
    fin.close();
}

/**
 * @brief Get row from page indexed by rowIndex
 * 
 * @param rowIndex 
 * @return vector<int> 
 */
vector<int> Page::getRow(int rowIndex)
{
    logger.log("Page::getRow");
    vector<int> result;
    result.clear();
    if (rowIndex >= this->rowCount)
        return result;
    return this->rows[rowIndex];
}

Page::Page(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount)
{
    logger.log("Page::Page");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->rows = rows;
    this->rowCount = rowCount;
    this->columnCount = rows[0].size();
    this->pageName = "../data/temp/"+this->tableName + "_Page" + to_string(pageIndex);
}

/**
 * @brief writes current page contents to file.
 * 
 */
void Page::writePage()
{
    logger.log("Page::writePage");
    ofstream fout(this->pageName, ios::trunc);
    for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            if (columnCounter != 0)
                fout << " ";
            fout << this->rows[rowCounter][columnCounter];
        }
        fout << endl;
    }
    fout.close();
}


/**
 * @brief changes the contents of a given row inside a page
 * 
 * @param rowContents, rowIndex
 * 
 */
void Page::changeRow(vector<int> rowContents, int rowIndex)
{
    if(rowContents.size() != this->rows[0].size()){
        cout<<"ERROR WRITING TO PAGE! INCOMPATIBLE SIZES"<<endl;
        return;
    }
    this->rows[rowIndex] = rowContents;
    
    return;

}

/**
 * @brief print the contents of a given row inside a page
 * 
 * 
 * 
 */
void Page::printContents()
{
    logger.log("Printing contents!");
    for(int i =0;i< this->rowCount;i++){
        for(int j =0; j< this->columnCount; j++){
            cout<<this->rows[i][j]<<" ";
        }
        cout<<endl;
    }

    return;
}
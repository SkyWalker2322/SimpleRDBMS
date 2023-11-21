#include "global.h"
typedef pair<vector<int>,pair<int,int>> QUEUE_TYPE;

vector<int> sortOnColumnIndex;
vector<string> sortTypes;


class Compare {
public:
    bool operator()(QUEUE_TYPE a1, QUEUE_TYPE b1){
    int i = 0;
    auto a = a1.first; auto b = b1.first;
    // cout<<a1.second.first<<" "<<b1.second.first<<endl;
    
    while(i<sortOnColumnIndex.size()){
        int target_index = sortOnColumnIndex[i];
        // cout<<a[target_index]<<" "<<b[target_index]<<endl;
        if(a[target_index] == b[target_index]){
            i++; 
            // cout<<"EQUALITY!"<<endl;
        }
        else{
            if(sortTypes[i] == "ASC"){
                return !(a[target_index] < b[target_index]);
            }
            else if(sortTypes[i] == "DESC"){
                
                return !(a[target_index] > b[target_index]);
            } 
        }
    }
    return true;
}
};

bool Table::convertToPages()
{
    int pageNum = 0;
    ifstream myfile("../data/temp/"+ this->tableName+"_sub");
    regex delim("[^\\s,]+");
    vector<vector<int>> contents;
    // cout<<"hi";
    for(pageNum = 0; pageNum < this->rowCount; pageNum++)
    {
        if(pageNum!=0 && pageNum%this->maxRowsPerBlock == 0){
            auto pg = bufferManager.getPage(this->tableName , (pageNum/this->maxRowsPerBlock) -1);
            pg.rows = contents;
            pg.writePage();
            contents.clear();
        } 
        string line;
        // cout<<"hi";
        getline(myfile,line);
        
        if(line != "\0"){
            vector<string> tokens;
            auto words_begin = std::sregex_iterator(line.begin(), line.end(), delim);
            auto words_end = std::sregex_iterator();
            for (std::sregex_iterator i = words_begin; i != words_end; ++i)
                tokens.emplace_back((*i).str());
            
            vector<int> tokenised;
            for(auto x:tokens) tokenised.push_back(stoi(x));
            contents.push_back(tokenised);
        }
    }

    if(contents.size()!=0){
            auto pg = bufferManager.getPage(this->tableName , this->blockCount -1);
            pg.rows = contents;
            pg.writePage();
            contents.clear();
    }

    filesystem::remove("../data/temp/"+ this->tableName+"_sub");
    
    return true;
}

static bool blockSortComparator(vector<int> a, vector<int> b){
    int i = 0;
    
    while(i<sortOnColumnIndex.size()){
        int target_index = sortOnColumnIndex[i];
        // cout<<a[target_index]<<" "<<b[target_index]<<endl;
        if(a[target_index] == b[target_index]){
            i++; 
            // cout<<"EQUALITY!"<<endl;
        }
        else{
            if(sortTypes[i] == "ASC"){
                return a[target_index] < b[target_index];
            }
            else if(sortTypes[i] == "DESC"){
                return a[target_index] > b[target_index];
            } 
        }
    }
    return true;
}
/**
 * @brief Construct a new Table:: Table object
 *
 */
Table::Table()
{
    logger.log("Table::Table");
}

/**
 * @brief Construct a new Table:: Table object used in the case where the data
 * file is available and LOAD command has been called. This command should be
 * followed by calling the load function;
 *
 * @param tableName 
 */
Table::Table(string tableName)
{
    logger.log("Table::Table");
    this->sourceFileName = "../data/" + tableName + ".csv";
    this->tableName = tableName;
}

/**
 * @brief Construct Matrix
 * 
 */
Table::Table(string tableName, bool trigger){
    logger.log("Matrix");
    this->sourceFileName = "../data/" + tableName + ".csv";
    this->tableName = tableName;
    this->matrixCheck = true;
}

/**
 * @brief Construct a new Table:: Table object used when an assignment command
 * is encountered. To create the table object both the table name and the
 * columns the table holds should be specified.
 *
 * @param tableName 
 * @param columns 
 */
Table::Table(string tableName, vector<string> columns)
{
    logger.log("Table::Table");
    this->sourceFileName = "../data/temp/" + tableName + ".csv";
    this->tableName = tableName;
    this->columns = columns;
    this->columnCount = columns.size();
    this->maxRowsPerBlock = (uint)((BLOCK_SIZE * 1000) / (sizeof(int) * columnCount));
    this->writeRow<string>(columns);
}

/**
 * @brief The load function is used when the LOAD command is encountered. It
 * reads data from the source file, splits it into blocks and updates table
 * statistics.
 *
 * @return true if the table has been successfully loaded 
 * @return false if an error occurred 
 */
bool Table::load()
{
    if(this->matrixCheck) {
        cout<<"ITS A MATRIX!!"<<endl;
        return false;
    }
    logger.log("Table::load");
    fstream fin(this->sourceFileName, ios::in);
    string line;
    if (getline(fin, line))
    {
        fin.close();
        if (this->extractColumnNames(line))
            if (this->blockify())
                return true;
    }
    fin.close();
    return false;
}

/**
 * @brief The load_matrix function is used when the LOAD MATRIX command is encountered. It
 * reads data from the source file, splits it into blocks and updates table
 * statistics.
 *
 * @return true if the MATRIX has been successfully loaded 
 * @return false if an error occurred 
 */
bool Table::load_Matrix()
{
    if(!this->matrixCheck) {
        cout<<"ITS NOT A MATRIX!!"<<endl;
        return false;
    }
    logger.log("Table::load");
    fstream fin(this->sourceFileName, ios::in);
    string line;
    if (getline(fin, line))
    {
        fin.close();
        this->extractColumnNamesAlt(line);
        if (this->blockify())
            return true;
    }
    fin.close();
    return false;
}


/**
 * @brief The Function is an alternative to the existing column reading function aimed to 
 * skip column reading but sets the remaining attributes of the class
 * 
 */
bool Table::extractColumnNamesAlt(string firstLine)
{
    logger.log("Table::extractCOlumnNamesALt() called");
    unordered_set<string> columnNames;
    string word;
    stringstream s(firstLine);
    int index = 0;
    while(getline(s,word,',')){
        string col = to_string(index);
        this->columns.emplace_back(col);
        index += 1;
    }

    this->columnCount = this->columns.size();
    this->maxRowsPerBlock = (uint)((BLOCK_SIZE * 1000) / (sizeof(int) * this->columnCount));
    return true;
}


/**
 * @brief Function extracts column names from the header line of the .csv data
 * file. 
 *
 * @param line 
 * @return true if column names successfully extracted (i.e. no column name
 * repeats)
 * @return false otherwise
 */
bool Table::extractColumnNames(string firstLine)
{
    logger.log("Table::extractColumnNames");
    unordered_set<string> columnNames;
    string word;
    stringstream s(firstLine);
    while (getline(s, word, ','))
    {
        word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
        if (columnNames.count(word))
            return false;
        columnNames.insert(word);
        this->columns.emplace_back(word);
    }
    this->columnCount = this->columns.size();
    this->maxRowsPerBlock = (uint)((BLOCK_SIZE * 1000) / (sizeof(int) * this->columnCount));
    return true;
}

/**
 * @brief This function splits all the rows and stores them in multiple files of
 * one block size. 
 *
 * @return true if successfully blockified
 * @return false otherwise
 */
bool Table::blockify()
{
    int access_count = 0;
    if(this->maxRowsPerBlock >= 1){
        logger.log("Table::blockify");
        ifstream fin(this->sourceFileName, ios::in);
        string line, word;
        vector<int> row(this->columnCount, 0);
        vector<vector<int>> rowsInPage(this->maxRowsPerBlock, row);
        int pageCounter = 0;
        unordered_set<int> dummy;
        dummy.clear();
        this->distinctValuesInColumns.assign(this->columnCount, dummy);
        this->distinctValuesPerColumnCount.assign(this->columnCount, 0);
        if(!this->matrixCheck)  getline(fin,line);
        while (getline(fin, line))
        {
            stringstream s(line);
            for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
            {
                if (!getline(s, word, ','))
                    return false;
                // cout<<"word is "<<word<<" line is "<<line<<endl;
                row[columnCounter] = stoi(word);
                rowsInPage[pageCounter][columnCounter] = row[columnCounter];
            }
            pageCounter++;
            this->updateStatistics(row);
            if (pageCounter == this->maxRowsPerBlock)
            {
                bufferManager.writePage(this->tableName, this->blockCount, rowsInPage, pageCounter);
                access_count+=1;
                this->blockCount++;
                this->rowsPerBlockCount.emplace_back(pageCounter);
                pageCounter = 0;
            }
        }
        
        if (pageCounter)
        {
            bufferManager.writePage(this->tableName, this->blockCount, rowsInPage, pageCounter);
            access_count+=1;
            this->blockCount++;
            this->rowsPerBlockCount.emplace_back(pageCounter);
            pageCounter = 0;
        }

        if (this->rowCount == 0)
            return false;
        this->distinctValuesInColumns.clear();
        cout<<"Number of blocks read: "<<0<<endl;
        cout<<"Number of blocks written: "<<access_count<<endl;
        cout<<"Number of blocks accessed: "<<access_count<<endl;
        
        cout<<this->rowCount<<endl;
        return true;
    }
    else{
        logger.log("Table:: special blockify");
        ifstream fin(this->sourceFileName, ios::in);
        string line, word;
        vector<int> row_contents;
        // to find out the number of pages required per row
        float page_size = 1000 * BLOCK_SIZE;
        float row_size = 4*this->columnCount;
        this->maxBlocksPerRow = ceil(row_size / page_size);
        int rowIndex = 0;
        while(getline(fin,line)) // for all rows
        {
            // for 1 row
            stringstream s(line);
            
            for(int i =0;i<this->maxBlocksPerRow; i++)
            {
                // inside each block
                int colsPerBlock = 1000*BLOCK_SIZE / 4;
                int columnCounter = 0;
                
                vector<int> row_per_block;

                while(columnCounter < colsPerBlock)
                {
                    if (!getline(s, word, ','))
                    break;

                    row_per_block.emplace_back(stoi(word));
                    row_contents.emplace_back(stoi(word));
                    columnCounter+=1;
                }
                
                bufferManager.writePage(this->tableName , rowIndex*maxBlocksPerRow + i, row_per_block, 1);
                access_count+=1;
                this->rowsPerBlockCount.emplace_back(row_per_block.size());
            }
            // this->updateStatistics(row_contents);
            rowIndex+=1;
        }
        logger.log("Finished blockify! special case");

        this->rowCount = rowIndex;
        cout<<"Number of blocks read: "<<0<<endl;
        cout<<"Number of blocks written: "<<access_count<<endl;
        cout<<"Number of blocks accessed: "<<access_count<<endl;
        return true;

    }
}

bool Table::extendedSort(vector<string> columnNames, vector<string> strategy){
    float RAM_LIMIT = 10;
    
    // idea is to first read blocks of data
    // which is alr stored in blocks so skip

    // now sort all the individual blocks
    
    int no_of_pages = ceil((float)rowCount/(float)maxRowsPerBlock);
    sortOnColumnIndex.clear();
    sortTypes.clear();

    map<string,int> columnIndices;
    for(int i=0;i<columnCount;i++){
        columnIndices[columns[i]] = i;
    }

    for(int i=0;i<columnNames.size();i++){
        sortOnColumnIndex.push_back(columnIndices[columnNames[i]]);
    }

    sortTypes  = strategy;

    
    // get the run size 
    int nR = RAM_LIMIT;
    
    for(int i=0;i<no_of_pages;i++){
        int offset = 0;
        vector<Page> run;
        vector<vector<int>> records;
        
        for(int offset=0;offset<RAM_LIMIT;offset++){
            
            if(i>= no_of_pages) break;
            auto curPage = bufferManager.getPage(this->tableName , i);
            auto rows = curPage.rows;
            curPage.rows.clear();
            run.push_back(curPage);
            // records.insert(records.end() , rows.begin(), rows.end()); //store the elements
            for(int it =0; it< curPage.rowCount; it++){
                records.push_back(rows[it]);
            }
            
            i++;
        }
        
        
        sort(records.begin(), records.end(), blockSortComparator ); 
        // for(auto x:records){
        //     for(auto y:x){
        //         cout<<y<<" ";
        //     }
        //     cout<<endl;
        // }
        
        for(int j=0; j<records.size(); j++){
            run[j/maxRowsPerBlock].rows.push_back(records[j]); //store the element
        }
        for(int pg = 0; pg< run.size(); pg++){
            run[pg].writePage();
        }
    
        i--;
    }

    // individual runs sorted 
    int dM = RAM_LIMIT-1;
    
    // now we need to sort  dM way runs
    // pair<vector<int>,pair<int,int>>
    // pair<vector<int>,pair<int,int>>
    priority_queue<QUEUE_TYPE , vector<QUEUE_TYPE> , Compare> pq; // for the heap 
    // <VECTOR<col_values> , <col_index, row_index>>
    
    for(int runSize = nR; runSize<this->blockCount; runSize = runSize*dM){
        int blockPerMergeCount = 0, writeIndex = 0;
        for(int pgNum = 0; pgNum < this->blockCount; pgNum+= runSize){ // jump across runs
            // now getting the current page
            auto curPage = bufferManager.getPage(this->tableName, pgNum);

            auto value = curPage.getRow(0);
            
            blockPerMergeCount+=1;
            pq.push({value, {pgNum, 0}});
            if(blockPerMergeCount == dM || ((pgNum+runSize) >= this->blockCount))
            {
                int pgCounter = 0;
                bool cond = true;
                vector<vector<int>> sortedWrites;
                while(cond){
                    QUEUE_TYPE p = pq.top();
                    pq.pop();
                    
                    Page topPage = bufferManager.getPage(this->tableName , p.second.first);
                    vector<int> topPageRow = topPage.getRow(p.second.second);

                    sortedWrites.push_back(topPageRow);
                    
                    // now to check if we need to reset the index
                    int newPage =-1, newRow = -1; bool cnd = false;
                    
                    if(p.second.second == topPage.rowCount -1){
                        // reset
                        newPage = p.second.first +1;
                        if(newPage >= this->blockCount){
                            newPage = 0; cnd = true;
                        }
                        newRow = 0;
                        
                    }
                    else{
                        // normal increment
                        newPage = p.second.first;
                        newRow = p.second.second+1;
                    }

                    // now we need to insert the next 
                    if( (cnd == false) &&  !(newPage == p.second.first+1 && (newPage%runSize ==0))){
                        Page nextPage = bufferManager.getPage(this->tableName, newPage);
                        // get  the record
                        auto val = nextPage.getRow(newRow);
                        
                        pq.push({val,{newPage, newRow}});
                    }
                    
                    pgCounter++;

                    if(pgCounter == this->maxRowsPerBlock){
                        // time to write the page
                        string namePgTable = "../data/temp/"+this->tableName + "_sub";
                        // append it to the temp
                        std::ofstream outfile;
                        outfile.open(namePgTable,ios::app);
                        for(auto x:sortedWrites) {
                            for(auto y:x) outfile<<y<<" ";
                            outfile<<endl;
                        }
                        outfile.close();
                        //reset pgcounter
                        pgCounter = 0;
                        sortedWrites.clear();
                    }

                    if(pq.empty()) cond=false;
                }

                if(pgCounter > 0){
                    // if anything left out before fully finishing (abnormal sizes)
                    string nameTable = "../data/temp/"+this->tableName+"_sub";
                    std::ofstream outfile;
                    outfile.open(nameTable,ios::app);
                    for(auto x:sortedWrites) {
                        for(auto y:x) outfile<<y<<" ";
                        outfile<<endl;
                    }
                    outfile.close();
                    sortedWrites.clear();
                    //reset pgcounter
                    pgCounter = 0;
                }

                blockPerMergeCount = 0; //reset
                
            }
        }
        
        if(convertToPages() == false)
        {
            return false;
        }
        
    }
    bufferManager.wipeDequeue();
    return true;

}
/**
 * @brief Given a row of values, this function will update the statistics it
 * stores i.e. it updates the number of rows that are present in the column and
 * the number of distinct values present in each column. These statistics are to
 * be used during optimisation.
 *
 * @param row 
 */
void Table::updateStatistics(vector<int> row)
{
    this->rowCount++;
    for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
    {
        if (!this->distinctValuesInColumns[columnCounter].count(row[columnCounter]))
        {
            this->distinctValuesInColumns[columnCounter].insert(row[columnCounter]);
            this->distinctValuesPerColumnCount[columnCounter]++;
        }
    }
}

/**
 * @brief Checks if the given column is present in this table.
 *
 * @param columnName 
 * @return true 
 * @return false 
 */
bool Table::isColumn(string columnName)
{
    logger.log("Table::isColumn");
    for (auto col : this->columns)
    {
        
        if (col == columnName)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Renames the column indicated by fromColumnName to toColumnName. It is
 * assumed that checks such as the existence of fromColumnName and the non prior
 * existence of toColumnName are done.
 *
 * @param fromColumnName 
 * @param toColumnName 
 */
void Table::renameColumn(string fromColumnName, string toColumnName)
{
    logger.log("Table::renameColumn");
    for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
    {
        if (columns[columnCounter] == fromColumnName)
        {
            columns[columnCounter] = toColumnName;
            break;
        }
    }
    return;
}

/**
 * @brief Function prints the first few rows of the table. If the table contains
 * more rows than PRINT_COUNT, exactly PRINT_COUNT rows are printed, else all
 * the rows are printed.
 *
 */
void Table::print()
{
    if(this->matrixCheck) {
        cout<<"ITS A MATRIX!!"<<endl;
        return;
    }
    logger.log("Table::print");
    // uint count = min((long long)PRINT_COUNT, this->rowCount);
    uint count = this->rowCount;

    //print headings
    this->writeRow(this->columns, cout);

    Cursor cursor(this->tableName, 0);
    
    vector<int> row;
    for (int rowCounter = 0; rowCounter < count; rowCounter++)
    {
        row = cursor.getNext();
        this->writeRow(row, cout);
    }
    printRowCount(this->rowCount);
}

/**
 * @brief Function prints the first few rows of the table. If the table contains
 * more rows than PRINT_COUNT, exactly PRINT_COUNT rows are printed, else all
 * the rows are printed.
 *
 */
void Table::print_matrix()
{
    int access_count = 0;
    if(!this->matrixCheck){
        cout<<"NOT A MATRIX !!"<<endl;
        return;
    }

   logger.log("Matrix printing");
   uint count = min((long long)PRINT_COUNT, this->rowCount);

    //print headings
    // this->writeRow(this->columns, cout);
    if(this->maxRowsPerBlock >= 1){
        Cursor cursor(this->tableName, 0);
        vector<int> row;
        for (int rowCounter = 0; rowCounter < count; rowCounter++)
        {
            logger.log("Entered print_matrix cursor for loop");
            row = cursor.getNext();
            this->writeRow(row, cout);
            cout<<endl;
            if(rowCounter%this->maxRowsPerBlock ==0) access_count+=1;
        }
    }
    else{
        Cursor cursor(this->tableName, 0);
        vector<int> row;
        for (int rowCounter = 0; rowCounter < count*this->maxBlocksPerRow; rowCounter++)
        {
            logger.log("Entered print_matrix cursor for loop");
            row = cursor.getNext();
            this->writeRow(row, cout);
            if(rowCounter % maxBlocksPerRow == (maxBlocksPerRow-1)) cout<<endl;
            else {cout<<","; access_count+=1;}
        }
    }
    cout<<"Number of blocks read: "<<access_count<<endl;
    cout<<"Number of blocks written: "<<0<<endl;
    cout<<"Number of blocks accessed: "<<access_count<<endl;
    printRowCount(this->rowCount);
}

/**
 * @brief This function returns one row of the table using the cursor object. It
 * returns an empty row is all rows have been read.
 *
 * @param cursor 
 * @return vector<int> 
 */
void Table::getNextPage(Cursor *cursor)
{
    logger.log("Table::getNext");

        if (cursor->pageIndex < this->blockCount - 1)
        {
            cursor->nextPage(cursor->pageIndex+1);
        }
}


/**
 * @brief called when EXPORT command is invoked to move source file to "data"
 * folder.
 *
 */
void Table::makePermanent()
{
    int access_count = 0;
    logger.log("Table::makePermanent");
    if(!this->isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
    string newSourceFile = "../data/" + this->tableName + ".csv";
    ofstream fout(newSourceFile, ios::out);

    //print headings
    if(!this->matrixCheck)
    this->writeRow(this->columns, fout);
    int count = this->rowCount;
    if(this->maxRowsPerBlock >= 1){
        Cursor cursor(this->tableName, 0);
        vector<int> row;
        for (int rowCounter = 0; rowCounter < count; rowCounter++)
        {
            logger.log("Entered print_matrix cursor for loop");
            row = cursor.getNext();
            this->writeRow(row, fout);
            fout<<endl;

            if(rowCounter%this->maxRowsPerBlock ==0) access_count+=1;
        }
    }
    else{
        Cursor cursor(this->tableName, 0);
        vector<int> row;
        for (int rowCounter = 0; rowCounter < count*this->maxBlocksPerRow; rowCounter++)
        {
            logger.log("Entered print_matrix cursor for loop");
            row = cursor.getNext();
            this->writeRow(row, fout);
            
            if(rowCounter % maxBlocksPerRow == (maxBlocksPerRow-1)) fout<<endl;
            else  {fout<<","; access_count+=1;}
        }
    }
    fout.close();
    cout<<"Number of blocks read: "<<access_count<<endl;
    cout<<"Number of blocks written: "<<0<<endl;
    cout<<"Number of blocks accessed: "<<access_count<<endl;
}

/**
 * @brief Function to check if table is already exported
 *
 * @return true if exported
 * @return false otherwise
 */
bool Table::isPermanent()
{
    logger.log("Table::isPermanent");
    if (this->sourceFileName == "../data/" + this->tableName + ".csv")
    return true;
    return false;
}

/**
 * @brief The unload function removes the table from the database by deleting
 * all temporary files created as part of this table
 *
 */
void Table::unload(){
    logger.log("Table::~unload");
    for (int pageCounter = 0; pageCounter < this->blockCount; pageCounter++)
        bufferManager.deleteFile(this->tableName, pageCounter);
    if (!isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
}

/**
 * @brief Function that returns a cursor that reads rows from this table
 * 
 * @return Cursor 
 */
Cursor Table::getCursor()
{
    logger.log("Table::getCursor");
    Cursor cursor(this->tableName, 0);
    return cursor;
}
/**
 * @brief Function that returns the index of column indicated by columnName
 * 
 * @param columnName 
 * @return int 
 */
int Table::getColumnIndex(string columnName)
{
    logger.log("Table::getColumnIndex");
    for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
    {
        if (this->columns[columnCounter] == columnName)
            return columnCounter;
    }
}


/**
 * @brief Function to transpose a matrix 
 * 
 * 
 * @return NOTHING
 */

void Table::transpose(){
    int read_count = 0, write_count = 0;
    logger.log("Transpose triggered");
    if(!this->matrixCheck)
    {
        cout<<"NOT A MATRIX TO TRANSPOSE"<<endl;
        return;
    }

    if(this->maxRowsPerBlock >= 1){
        for(int i =0 ; i< this->rowCount; i++){
            // start at (i,i+1)
            bufferManager.wipeDequeue();
            int iPage = i/this->maxRowsPerBlock;
            auto pageI = bufferManager.getPage(this->tableName,iPage);
            read_count+=1;

            for(int j = i+1; j< this->columnCount; j++){
                int jPage = j/this->maxRowsPerBlock;
                Page pageJ;
                if(j== i+1) pageJ = bufferManager.getPage(this->tableName, jPage);
                else pageJ = bufferManager.getPage(this->tableName,jPage, true);
                read_count+=1;
                if(iPage != jPage){
                    int iRow = i%this->maxRowsPerBlock;
                    int jRow = j%this->maxRowsPerBlock;

                
                    // swapping
                    auto pageIrowI = pageI.getRow(iRow);
                    auto pageJrowJ = pageJ.getRow(jRow);

                    //swap
                    int tmp = pageIrowI[j];
                    pageIrowI[j] = pageJrowJ[i];
                    pageJrowJ[i] = tmp;

                    pageI.changeRow(pageIrowI, iRow);
                    pageJ.changeRow(pageJrowJ, jRow);
                    
                    // write the new pages to file
                    pageI.writePage();
                    pageJ.writePage();
                    write_count+=2;
                }
                else{
                    logger.log("Same Page Swap Found! Special condn activated");
                    int iRow = i%this->maxRowsPerBlock;
                    int jRow = j%this->maxRowsPerBlock;

                    auto pageIrowI = pageI.getRow(iRow);
                    auto pageJrowJ = pageI.getRow(jRow);
                    

                    int tmp = pageIrowI[j];
                    pageIrowI[j] = pageJrowJ[i];
                    pageJrowJ[i] = tmp;

                    
                    pageI.changeRow(pageIrowI, iRow);
                    pageI.changeRow(pageJrowJ, jRow);
                    
                    
                    // write the new pages to file
                    pageI.writePage();
                    write_count+=1;
                    
                }
            }
        }
    }
    else
    {
        logger.log("Transpose on special condiiton of page sizes used!");
        for(int i =0;i< this->rowCount; i++){
            // starting at (i,i+1)
            for(int j=i+1;j<this->columnCount;j++){
                bufferManager.wipeDequeue();
                int iPage = i*this->maxBlocksPerRow;
                int jPage = j*this->maxBlocksPerRow;

                iPage += (j/250);
                jPage += (i/250);

                Page pageI = bufferManager.getPage(this->tableName , iPage);
                Page pageJ = bufferManager.getPage(this->tableName , jPage);
                read_count+=2;

                // get the rows
                auto pageIrow = pageI.getRow(0);
                auto pageJrow = pageJ.getRow(0);

                // indexing ~ j -> j%250
                // swap
                int tmp = pageIrow[j%250];
                pageIrow[j%250] = pageJrow[i%250];
                pageJrow[i%250] = tmp;
                
                //update changes
                pageI.changeRow(pageIrow, 0);
                pageJ.changeRow(pageJrow, 0);

                //write 
                pageI.writePage();
                pageJ.writePage();
                write_count+=2;
            }
        }
    }
    logger.log("Transpose Action finished");
    bufferManager.wipeDequeue();
    cout<<"Number of blocks read: "<<read_count<<endl;
    cout<<"Number of blocks written: "<<write_count<<endl;
    cout<<"Number of blocks accessed: "<<read_count+write_count<<endl;
    return;
}

/**
 * @brief Function to check if a matrix is symmetric
 * 
 * @return a boolean determining the symmetry. True = Symmetric ; False = Asymmetric
 * 
 */

bool Table::symmetry(){
    logger.log("Symmetry check for table initiated");
    int read_count = 0, write_count = 0;
    if(!this->matrixCheck)
    {
        cout<<"NOT A MATRIX TO TRANSPOSE"<<endl;
        return false;
    }
    bool flag = true;

    if(this->maxRowsPerBlock >= 1){
        for(int i =0 ; i< this->rowCount; i++){
            // start at (i,i+1)
            bufferManager.wipeDequeue();
            int iPage = i/this->maxRowsPerBlock;
            auto pageI = bufferManager.getPage(this->tableName,iPage);
            read_count +=1;

            for(int j = i+1; j< this->columnCount; j++){
                int jPage = j/this->maxRowsPerBlock;
                Page pageJ;
                if(j== i+1) pageJ = bufferManager.getPage(this->tableName, jPage);
                else pageJ = bufferManager.getPage(this->tableName,jPage, true);
                read_count +=1;

                if(iPage != jPage){
                    int iRow = i%this->maxRowsPerBlock;
                    int jRow = j%this->maxRowsPerBlock;

                
                    // get row
                    auto pageIrowI = pageI.getRow(iRow);
                    auto pageJrowJ = pageJ.getRow(jRow);

                    //check
                    flag = flag && (pageIrowI[j] == pageJrowJ[i]);

                }
                else{
                    
                    int iRow = i%this->maxRowsPerBlock;
                    int jRow = j%this->maxRowsPerBlock;

                    auto pageIrowI = pageI.getRow(iRow);
                    auto pageJrowJ = pageI.getRow(jRow);
                    

                    flag = flag && (pageIrowI[j] == pageJrowJ[i]);
                    
                }
            }
        }
    }
    else
    {
        logger.log("Transpose on special condiiton of page sizes used!");
        for(int i =0;i< this->rowCount; i++){
            // starting at (i,i+1)
            for(int j=i+1;j<this->columnCount;j++){
                bufferManager.wipeDequeue();
                int iPage = i*this->maxBlocksPerRow;
                int jPage = j*this->maxBlocksPerRow;

                iPage += (j/250);
                jPage += (i/250);

                Page pageI = bufferManager.getPage(this->tableName , iPage);
                Page pageJ = bufferManager.getPage(this->tableName , jPage);
                read_count +=2;

                // get the rows
                auto pageIrow = pageI.getRow(0);
                auto pageJrow = pageJ.getRow(0);

                // indexing ~ j -> j%250
                // swap
                flag = flag && (pageIrow[j%250] == pageJrow[i%250]);
            }
        }
    }
    bufferManager.wipeDequeue();
    cout<<"Number of blocks read: "<<read_count<<endl;
    cout<<"Number of blocks written: "<<write_count<<endl;
    cout<<"Number of blocks accessed: "<<read_count+write_count<<endl;
    return flag;
}

/**
 * @brief Function to calculate COMPUTE for a matrix given by
 * A - AT
 * 
 */

void Table::compute(){
    logger.log("Compute triggered");
    int read_count = 0, write_count =0;
    if(!this->matrixCheck)
    {
        cout<<"NOT A MATRIX TO COMPUTE"<<endl;
        return;
    }

    if(this->maxRowsPerBlock >= 1){
        for(int i =0 ; i< this->rowCount; i++){
            int check = 0;
            if(i==this->rowCount-1) check+=1;
            // start at (i,i+1)
            bufferManager.wipeDequeue();
            int iPage = i/this->maxRowsPerBlock;
            auto pageI = bufferManager.getPage(this->tableName,iPage);
            read_count+=1;
            

            for(int j = i; j< this->columnCount; j++){
                if(j==this->columnCount-1) check+=1;
                int jPage = j/this->maxRowsPerBlock;
                Page pageJ;
                if(j== i+1) pageJ = bufferManager.getPage(this->tableName, jPage);
                else pageJ = bufferManager.getPage(this->tableName,jPage, true);
                read_count+=1;

                if(iPage != jPage){
                    int iRow = i%this->maxRowsPerBlock;
                    int jRow = j%this->maxRowsPerBlock;

                
                    // swapping
                    auto pageIrowI = pageI.getRow(iRow);
                    auto pageJrowJ = pageJ.getRow(jRow);

                    //swap
                    int tmp = pageIrowI[j];
                    pageIrowI[j] -= pageJrowJ[i];
                    pageJrowJ[i] -= tmp;

                    pageI.changeRow(pageIrowI, iRow);
                    pageJ.changeRow(pageJrowJ, jRow);
                    
                    // write the new pages to file
                    pageI.writePage();
                    pageJ.writePage();
                    write_count+=2;
                }
                else{
                    logger.log("Same Page Swap Found! Special condn activated");
                    int iRow = i%this->maxRowsPerBlock;
                    int jRow = j%this->maxRowsPerBlock;

                    auto pageIrowI = pageI.getRow(iRow);
                    auto pageJrowJ = pageI.getRow(jRow);
                    
                    
                    int tmp = pageIrowI[j];

                    if(check==2) cout<<"BEfore"<<endl<<tmp<<" "<<pageJrowJ[i]<<endl;

                    pageIrowI[j] -= pageJrowJ[i];
                    pageJrowJ[i] -= tmp;

                    
                    if(check==2) cout<<"After"<<endl<<pageIrowI[j]<<" "<<pageJrowJ[i]<<endl;

                    pageI.changeRow(pageIrowI, iRow);
                    pageI.changeRow(pageJrowJ, jRow);
                    
                    if(check==2){
                        for(auto x:pageI.getRow(iRow)){
                            cout<<x<<' ';
                        }
                        cout<<endl;
                    }


                    // write the new pages to file
                    pageI.writePage();
                    write_count+=1;
                }
            }
        }
    }
    else
    {
        logger.log("Transpose on special condiiton of page sizes used!");
        for(int i =0;i< this->rowCount; i++){
            // starting at (i,i+1)
            for(int j=i;j<this->columnCount;j++){
                bufferManager.wipeDequeue();
                int iPage = i*this->maxBlocksPerRow;
                int jPage = j*this->maxBlocksPerRow;

                iPage += (j/250);
                jPage += (i/250);

                Page pageI = bufferManager.getPage(this->tableName , iPage);
                Page pageJ = bufferManager.getPage(this->tableName , jPage);
                read_count+=2;

                // get the rows
                auto pageIrow = pageI.getRow(0);
                auto pageJrow = pageJ.getRow(0);

                // indexing ~ j -> j%250
                // swap
                int tmp = pageIrow[j%250];
                pageIrow[j%250] -= pageJrow[i%250];
                pageJrow[i%250] -= tmp;
                
                //update changes
                pageI.changeRow(pageIrow, 0);
                pageJ.changeRow(pageJrow, 0);

                //write 
                pageI.writePage();
                pageJ.writePage();
                write_count+=2;
            }
        }
    }
    logger.log("Compute Action finished");
    bufferManager.wipeDequeue();
    cout<<"Number of blocks read: "<<read_count<<endl;
    cout<<"Number of blocks written: "<<write_count<<endl;
    cout<<"Number of blocks accessed: "<<read_count+write_count<<endl;
    return;
}

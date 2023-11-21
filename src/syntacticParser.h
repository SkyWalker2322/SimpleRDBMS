#include "tableCatalogue.h"

using namespace std;

enum QueryType
{
    CLEAR,
    CROSS,
    GROUP,
    ORDER,
    DISTINCT,
    EXPORT,
    EXPORT_M,
    INDEX,
    JOIN,
    LIST,
    LOAD,
    LOAD_M,
    PRINT,
    PRINT_M,
    PROJECTION,
    RENAME,
    RENAME_M,
    SELECTION,
    SORT,
    MERGESORT,
    SOURCE,
    TRANSPOSE,
    COMPUTE,
    CHECKSYMMETRY,
    UNDETERMINED
};

enum BinaryOperator
{
    LESS_THAN,
    GREATER_THAN,
    LEQ,
    GEQ,
    EQUAL,
    NOT_EQUAL,
    NO_BINOP_CLAUSE
};

enum SortingStrategy
{
    ASC,
    DESC,
    NO_SORT_CLAUSE
};

enum SelectType
{
    COLUMN,
    INT_LITERAL,
    NO_SELECT_CLAUSE
};

class ParsedQuery
{

public:
    QueryType queryType = UNDETERMINED;

    string clearRelationName = "";

    string crossResultRelationName = "";
    string crossFirstRelationName = "";
    string crossSecondRelationName = "";

    string groupResultRelationName = "";
    string groupInputRelationName = "";
    string groupSortColumnName = "";
    string groupFirstAggregateColumnName = "";
    string groupSecondAggregateColumnName = "";
    string groupBinaryOperator = "";
    string groupConditionValue = "";

    string orderResultRelationName = "";
    string orderconditionattribute = "";
    string ordercondition = "";
    string orderRelationName = "";

    string distinctResultRelationName = "";
    string distinctRelationName = "";

    string exportRelationName = "";

    IndexingStrategy indexingStrategy = NOTHING;
    string indexColumnName = "";
    string indexRelationName = "";

    string joinBinaryOperator = "";
    string joinResultRelationName = "";
    string joinFirstRelationName = "";
    string joinSecondRelationName = "";
    string joinFirstColumnName = "";
    string joinSecondColumnName = "";
    

    string loadRelationName = "";

    string printRelationName = "";

    string projectionResultRelationName = "";
    vector<string> projectionColumnList;
    string projectionRelationName = "";

    string renameFromColumnName = "";
    string renameToColumnName = "";
    string renameRelationName = "";

    SelectType selectType = NO_SELECT_CLAUSE;
    BinaryOperator selectionBinaryOperator = NO_BINOP_CLAUSE;
    string selectionResultRelationName = "";
    string selectionRelationName = "";
    string selectionFirstColumnName = "";
    string selectionSecondColumnName = "";
    int selectionIntLiteral = 0;

    SortingStrategy sortingStrategy = NO_SORT_CLAUSE;
    vector<SortingStrategy> sortingStrategies;
    string sortResultRelationName = "";
    string sortColumnName = "";
    vector<string> mergeSortColumns;
    string sortRelationName = "";

    string sourceFileName = "";

    string transposeTableName = "";

    ParsedQuery();
    void clear();
};

bool syntacticParse();
bool syntacticParseCLEAR();
bool syntacticParseCROSS();
bool syntacticParseGROUP();
bool syntacticParseORDER();
bool syntacticParseDISTINCT();
bool syntacticParseEXPORT();
bool syntacticParseEXPORTM();
bool syntacticParseINDEX();
bool syntacticParseJOIN();
bool syntacticParseLIST();
bool syntacticParseLOAD();
bool syntacticParseLOAD_M();
bool syntacticParsePRINT();
bool syntacticParsePRINT_M();
bool syntacticParsePROJECTION();
bool syntacticParseRENAME();
bool syntacticParseRENAME_M();
bool syntacticParseSELECTION();
bool syntacticParseSORT();
bool syntacticParseMERGESORT();
bool syntacticParseSOURCE();
bool syntacticParseTRANSPOSE();
bool syntacticParseSYMMETRY();
bool syntacticParseCOMPUTE();

bool isFileExists(string tableName);
bool isQueryFile(string fileName);

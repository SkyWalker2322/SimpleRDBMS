#include"global.h"

bool semanticParse(){
    logger.log("semanticParse");
    // cout<<parsedQuery.queryType<<endl;
    switch(parsedQuery.queryType){
        case CLEAR: return semanticParseCLEAR();
        case CROSS: return semanticParseCROSS();
        case DISTINCT: return semanticParseDISTINCT();
        case EXPORT: return semanticParseEXPORT();
        case EXPORT_M: return semanticParseEXPORT();
        case INDEX: return semanticParseINDEX();
        case JOIN: return semanticParseJOIN();
        case LIST: return semanticParseLIST();
        case LOAD: return semanticParseLOAD();
        case LOAD_M: return semanticParseLOAD();
        case PRINT: return semanticParsePRINT();
        case PRINT_M: return semanticParsePRINT();
        case PROJECTION: return semanticParsePROJECTION();
        case RENAME: return semanticParseRENAME();
        case RENAME_M: return semanticParseRENAME_M();
        case SELECTION: return semanticParseSELECTION();
        case SORT: return semanticParseSORT();
        case MERGESORT: return semanticParseMERGESORT();
        case SOURCE: return semanticParseSOURCE();
        case TRANSPOSE: return semanticParseTRANSPOSE();
        case CHECKSYMMETRY: return semanticParseSYMMETRY();
        case COMPUTE: return semanticParseCOMPUTE();
        case GROUP: return semanticParseGROUP();
        case ORDER: return semanticParseORDER();
        default: cout<<"SEMANTIC ERROR"<<endl;
    }

    return false;
}
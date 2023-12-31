#include"global.h"

void executeCommand(){

    switch(parsedQuery.queryType){
        case CLEAR: executeCLEAR(); break;
        case CROSS: executeCROSS(); break;
        case DISTINCT: executeDISTINCT(); break;
        case EXPORT: executeEXPORT(); break;
        case EXPORT_M: executeEXPORTM(); break;
        case INDEX: executeINDEX(); break;
        case JOIN: executeJOIN(); break;
        case LIST: executeLIST(); break;
        case LOAD: executeLOAD(); break;
        case LOAD_M: executeLOADM(); break;
        case PRINT: executePRINT(); break;
        case PRINT_M: executePRINTM(); break;
        case PROJECTION: executePROJECTION(); break;
        case RENAME: executeRENAME(); break;
        case RENAME_M: executeRENAMEM(); break;
        case SELECTION: executeSELECTION(); break;
        case SORT: executeSORT(); break;
        case MERGESORT: executeSORT(); break;
        case SOURCE: executeSOURCE(); break;
        case TRANSPOSE: executeTRANSPOSE(); break;
        case CHECKSYMMETRY: executeSYMMETRY(); break;
        case COMPUTE: executeCOMPUTE(); break;
        case GROUP: executeGROUP(); break;
        case ORDER: executeORDER(); break;
        default: cout<<"PARSING ERROR"<<endl;
    }

    return;
}

void printRowCount(int rowCount){
    cout<<"\n\nRow Count: "<<rowCount<<endl;
    return;
}
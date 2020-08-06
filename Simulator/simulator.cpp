#include <iostream>
#include <fstream>
#include <cstdlib>
#include "libraries.h"

using namespace std;

extern int reg[35];

fstream snapshot("snapshot.rpt", fstream::out);
int cycle = 0;
bool isHalt = false;

int main(int argc, char *argv[]){
    ifstream iimg("./iimage.bin", ios::in | ios::binary);
    ifstream dimg("./dimage.bin", ios::in | ios::binary);
    initReg();
    initMemory();
    loadIimage(&iimg);
    loadDimage(&dimg);

    int IMemorySize = 64;
    int DMemorySize = 32;
    int IMemoryPageSize =  8;
    int DMemoryPageSize = 16;
    int ICacheSize = 16;
    int ICacheBlockSize = 4;
    int ICacheSetAssociate = 4;
    int DCacheSize = 16;
    int DCacheBlockSize = 4;
    int DCacheSetAssociate = 1;
    
    if(argc == 2){
        IMemorySize = atoi(argv[1]);
    }
    else if(argc == 3){
        IMemorySize = atoi(argv[1]);
        DMemorySize = atoi(argv[2]);
    }
    else if(argc == 4){
        IMemorySize = atoi(argv[1]);
        DMemorySize = atoi(argv[2]);
        IMemoryPageSize = atoi(argv[3]);
    }
    else if(argc == 5){
        IMemorySize = atoi(argv[1]);
        DMemorySize = atoi(argv[2]);
        IMemoryPageSize = atoi(argv[3]);
        DMemoryPageSize = atoi(argv[4]);
    }
    else if(argc == 6){
        IMemorySize = atoi(argv[1]);
        DMemorySize = atoi(argv[2]);
        IMemoryPageSize = atoi(argv[3]);
        DMemoryPageSize = atoi(argv[4]);
        ICacheSize = atoi(argv[5]);
    }
    else if(argc == 7){
        IMemorySize = atoi(argv[1]);
        DMemorySize = atoi(argv[2]);
        IMemoryPageSize = atoi(argv[3]);
        DMemoryPageSize = atoi(argv[4]);
        ICacheSize = atoi(argv[5]);
        ICacheBlockSize = atoi(argv[6]);
    }
    else if(argc == 8){
        IMemorySize = atoi(argv[1]);
        DMemorySize = atoi(argv[2]);
        IMemoryPageSize = atoi(argv[3]);
        DMemoryPageSize = atoi(argv[4]);
        ICacheSize = atoi(argv[5]);
        ICacheBlockSize = atoi(argv[6]);
        ICacheSetAssociate = atoi(argv[7]);
    }
    else if(argc == 9){
        IMemorySize = atoi(argv[1]);
        DMemorySize = atoi(argv[2]);
        IMemoryPageSize = atoi(argv[3]);
        DMemoryPageSize = atoi(argv[4]);
        ICacheSize = atoi(argv[5]);
        ICacheBlockSize = atoi(argv[6]);
        ICacheSetAssociate = atoi(argv[7]);
        DCacheSize = atoi(argv[8]);
    }
    else if(argc == 10){
        IMemorySize = atoi(argv[1]);
        DMemorySize = atoi(argv[2]);
        IMemoryPageSize = atoi(argv[3]);
        DMemoryPageSize = atoi(argv[4]);
        ICacheSize = atoi(argv[5]);
        ICacheBlockSize = atoi(argv[6]);
        ICacheSetAssociate = atoi(argv[7]);
        DCacheSize = atoi(argv[8]);
        DCacheBlockSize = atoi(argv[9]);
    }
    else if(argc == 11){
        IMemorySize = atoi(argv[1]);
        DMemorySize = atoi(argv[2]);
        IMemoryPageSize = atoi(argv[3]);
        DMemoryPageSize = atoi(argv[4]);
        ICacheSize = atoi(argv[5]);
        ICacheBlockSize = atoi(argv[6]);
        ICacheSetAssociate = atoi(argv[7]);
        DCacheSize = atoi(argv[8]);
        DCacheBlockSize = atoi(argv[9]);
        DCacheSetAssociate = atoi(argv[10]);
    }

    InstrMemory(IMemorySize, IMemoryPageSize, ICacheSize, ICacheBlockSize, ICacheSetAssociate);
    DataMemory(DMemorySize, DMemoryPageSize, DCacheSize, DCacheBlockSize, DCacheSetAssociate);

    for(int i = 0; i < 34; i++)
        valueSet.insert(i);

    while(!isHalt){
    	showRegValue();
	    cycle++;
        ICMP(reg[PC]);
    	reg[PC] = reg[PC] + 4;
    	decode(insMemory[(reg[PC] / 4) - 1]);
    }
    printCMP();
    return 0;
}

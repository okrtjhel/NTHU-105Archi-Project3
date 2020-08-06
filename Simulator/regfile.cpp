#include <iostream>
#include <iomanip>
#include <fstream>
#include <set>
#include "libraries.h"

using namespace std;

extern int cycle;
extern fstream snapshot;
int reg[35];
set<int> valueSet;

void initReg(){
	for(int i = 0; i < 35; i++)
		reg[i] = 0;
}

void showRegValue(){
	snapshot << "cycle " << dec << cycle << endl;	
	for(set<int>::iterator it = valueSet.begin(); it != valueSet.end(); ++it){
		if(*it <= 31){
			snapshot << "$" << setfill('0') << setw(2) << dec << *it << ": 0x" << setfill('0') << setw(8) << hex << uppercase << reg[*it] << endl;
		}
		if(*it == HI){
			snapshot << "$HI: 0x" << setfill('0') << setw(8) << hex << uppercase << reg[HI] << endl;
		}
		if(*it == LO){
			snapshot << "$LO: 0x" << setfill('0') << setw(8) << hex << uppercase << reg[LO] << endl;
		}
	}
	snapshot << "PC: 0x" << setfill('0') << setw(8) << hex << uppercase << reg[PC] << endl;
	valueSet.clear();
	snapshot << endl << endl;
}

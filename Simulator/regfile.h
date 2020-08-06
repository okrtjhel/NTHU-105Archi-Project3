#ifndef _REGFILE_H
#define _REGFILE_H

#include <set>
#include <fstream>

#define SP 29
#define HI 32
#define LO 33
#define PC 34

using namespace std;

extern int reg[35];
extern set<int> valueSet;

extern void initReg();
extern void showRegValue();

#endif
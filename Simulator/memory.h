#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <fstream>

using namespace std;

extern int MemorySize;
extern unsigned insNum;
extern unsigned insMemory[256];
extern unsigned dataNum;
extern unsigned dataMemory[1024];

extern void initMemory();
extern unsigned readWord(ifstream*);
extern unsigned readByte(ifstream*);
extern void loadIimage(ifstream*);
extern void loadDimage(ifstream*);
extern int loadMemory(int, int);
extern void saveMemory(int, int, int);

#endif
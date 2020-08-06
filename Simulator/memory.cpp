#include <iostream>
#include <cstdlib>
#include <fstream>
#include "libraries.h"

using namespace std;

int MemorySize = 1024;
unsigned insNum;
unsigned insMemory[256];
unsigned dataNum;
unsigned dataMemory[1024];
extern bool isHalt;

void initMemory(){
	for(int i = 0; i < (MemorySize / 4); i++)
		insMemory[i] = 0;
	for(int i = 0; i < MemorySize; i++)
		dataMemory[i] = 0;
}

unsigned readWord(ifstream *img){
	unsigned value = 0;
	char c;
	img->get(c);
	value = value | (unsigned char)c;
	img->get(c);
	value = (value << 8) | (unsigned char)c;
	img->get(c);
	value = (value << 8) | (unsigned char)c;
	img->get(c);
	value = (value << 8) | (unsigned char)c;
	return value;
}

unsigned readByte(ifstream * img){
	unsigned value = 0;
	char c;
	img->get(c);
	value = value | (unsigned char)c;
	return value;
}

void loadIimage(ifstream *iimg){
	reg[PC] = readWord(iimg);
	insNum = readWord(iimg);
	for(int i = 0; i < insNum; i++){
		insMemory[(reg[PC] / 4) + i] = readWord(iimg);
	}
}

void loadDimage(ifstream* dimg){
	reg[SP] = readWord(dimg);
	dataNum = readWord(dimg) * 4;
	for(int i = 0; i < dataNum; i++){
		dataMemory[i] = readByte(dimg);
	}
}

int loadMemory(int loc, int byte){
	int data = 0;
	DCMP(loc);
	if(isHalt)
		return 0;
	switch(byte){
		case 1:
			data = dataMemory[loc];
			break;
		case 2:
			data = dataMemory[loc];
			data = (data << 8) + dataMemory[loc + 1];
			break;
		case 4:
			data = dataMemory[loc];
			data = (data << 8) + dataMemory[loc + 1];
			data = (data << 8) + dataMemory[loc + 2];
			data = (data << 8) + dataMemory[loc + 3];
			break;
		default:
			break;
	}
	return data;
}

void saveMemory(int loc, int byte, int value){
	unsigned uvalue = (unsigned)value;
	DCMP(loc);
	if(isHalt)
		return;
	switch(byte){
		case 1:
			dataMemory[loc] = (uvalue << 24) >> 24;
			break;
		case 2:
			dataMemory[loc] = (uvalue << 16) >> 24;
			dataMemory[loc + 1] = (uvalue << 24) >> 24;
			break;
		case 4:
			dataMemory[loc] = uvalue >> 24;
			dataMemory[loc + 1] = (uvalue << 8) >> 24;
			dataMemory[loc + 2] = (uvalue << 16) >> 24;
			dataMemory[loc + 3] = (uvalue << 24) >> 24;
			break;
		default:
			break;
	}
}

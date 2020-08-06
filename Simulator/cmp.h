#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <map>

using namespace std;
extern fstream report;
extern bool page_fault;

class TLB{
	int entryNum, page_size;
	bool *valid;
	int *PPN, *VPN;
	list<int> LRU;
	//<ppn,entry>
	map<int,int> table;

public:
	int hit, miss;
	TLB(){};
	TLB(int _page_size){
		page_size = _page_size;
		entryNum = (1024 / page_size) / 4;
		valid = new bool[entryNum];
		PPN = new int[entryNum];
		VPN = new int[entryNum];
		for(int i = 0; i < entryNum; ++i){
			valid[i] = false;
			PPN[i] = -1;
			VPN[i] = -1;
		}
		hit = 0;
		miss = 0;
	}

	int find_PPN(int vaddr){
		int vpn = vaddr / page_size;
		for(int i = 0; i < entryNum; ++i){
			if(valid[i] && (vpn == VPN[i])){
				return PPN[i];
			}
		}
		return -1;
	}

	void update(int vaddr, int ppn){
		//table
		if(table.find(ppn) != table.end()){
			valid[table[ppn]] = false;
		}

		int i;
		int vpn = vaddr / page_size;
		//if space exist
		for(i = 0; i < entryNum; ++i){
			if(valid[i] == false){
				valid[i] = true;
				VPN[i] = vpn;
				PPN[i] = ppn;
				table[ppn] = i;
				LRU.push_back(i);
				break;
			}
		}
		//if full
		if(i == entryNum){
			int lru = LRU.front();
			LRU.pop_front();
			VPN[lru] = vpn;
			PPN[lru] = ppn;
			LRU.push_back(lru);
		}
	}

	bool isHit(int vaddr){
		page_fault = false;
		int vpn = vaddr / page_size;
		for(int i = 0; i < entryNum; ++i){
			if(valid[i] && (VPN[i] == vpn)){
				auto lru = find(LRU.begin(), LRU.end(), i);
				LRU.erase(lru);
				LRU.push_back(i);
				return true;
			}
		}
		return false;
	}

	void print(){
//		report << "# hits: " << hit << endl;
//		report << "# misses: " << miss << endl << endl;
	}
};

class PTE{
	int entryNum, page_size;
	bool *valid;
	int *PPN;
	//<ppn,vpn>
	map<int,int> table;

public:
	int hit, miss;
	PTE(){};
	PTE(int _page_size){
		page_size = _page_size;
		entryNum = 1024 / page_size;
		valid = new bool[entryNum];
		PPN = new int[entryNum];
		for(int i = 0; i < entryNum; ++i){
			valid[i] = false;
			PPN[i] = -1;
		}
		hit = 0;
		miss = 0;
	}

	int find_PPN(int vaddr){
		int vpn = vaddr / page_size;
		if(valid[vpn]){
			return PPN[vpn];
		}
		return -1;
	}

	bool isHit(int vaddr){
		int vpn = vaddr / page_size;
		if(valid[vpn]){
			return true;
		}
		return false;
	}

	void update(int vaddr, int ppn){
		//table
		if(table.find(ppn) != table.end()){
			valid[table[ppn]] = false;
		}
		int vpn = vaddr / page_size;
		valid[vpn] = true;
		PPN[vpn] = ppn;
		table[ppn] = vpn;
	}
	void print(){
//		report << "# hits: " << hit << endl;
//		report << "# misses: " << miss << endl << endl;
	}
};

class Cache{
	int associate;
	int blockNum, setNum;
	int block_size, page_size;
	bool *MRU, *valid;
	int *tag, *PPN;

public:
	int hit, miss;
	Cache(){};
	Cache(int Cache_size, int _associate, int _block_size, int _page_size){
		associate = _associate;
		block_size = _block_size;
		page_size = _page_size;
		blockNum = Cache_size / block_size;
		setNum = blockNum / associate;
		MRU = new bool[blockNum];
		valid = new bool[blockNum];
		tag = new int[blockNum];
		PPN = new int[blockNum];
		for(int i = 0; i < blockNum; ++i){
			MRU[i]  = false;
			valid[i] = false;
			tag[i] = -1;
			PPN[i] = -1;
		}
		hit = 0;
		miss = 0;
	}

	void update(int paddr){
		int _ppn = paddr / page_size;
		int _tag = (paddr / block_size) / setNum;
		int _index = (paddr / block_size) % setNum;
		
		//page fault
		if(page_fault){
			for(int i = 0; i < blockNum; ++i){
				if(PPN[i] == _ppn){
					valid[i] = false;
					MRU[i] = false;
				}
			}
		}

		if(associate == 1){
			valid[_index] = true;
			MRU[_index] = true;
			tag[_index] = _tag;
			PPN[_index] = _ppn;
			if(isFull(_index)){
				resetMRU(_index, 0);
			}
			return;
		}
		else{
			for(int i = 0; i < associate; ++i){
				if(valid[_index * associate + i] == false){
					valid[_index * associate + i] = true;
					MRU[_index * associate + i] = true;
					tag[_index * associate + i] = _tag;
					PPN[_index * associate + i] = _ppn;
					if(isFull(_index)){
						resetMRU(_index, i);
					}
					return;
				}
			}
		}

		for(int i = 0; i < associate; ++i){
			if(MRU[_index * associate + i] == false){
				valid[_index * associate + i] = true;
				MRU[_index * associate + i] = true;
				tag[_index * associate + i] = _tag;
				PPN[_index * associate + i] = _ppn;
				if(isFull(_index)){
					resetMRU(_index, i);
				}
				return;
			}		
		}
	}

	bool isHit(int paddr){
		int _tag = (paddr / block_size) / setNum;
		int _index = (paddr / block_size) % setNum;
		for(int i = 0; i < associate; ++i){
			if(valid[_index * associate + i] && (_tag == tag[_index * associate + i])){
				MRU[_index * associate + i] = true;
				if(isFull(_index)){
					resetMRU(_index, i);
				}
				return true;
			}
		}
		return false;
	}

	bool isFull(int index){
		for(int i = 0; i < associate; ++i){
			if(MRU[index * associate + i] == false){
				return false;
			}
		}
		return true;
	}

	void resetMRU(int index, int i){
		for(int j = 0; j < associate; ++j){
			if(j != i){
				MRU[index * associate + j] = false;
			}
		}
	}

	void print(){
//		report << "# hits: " << hit << endl;
//		report << "# misses: " << miss << endl << endl;
	}
};

class Memory{
	bool *valid;
	list<int> LRU;

public:
	int page_size, pageNum;
	Memory(){};
	Memory(int Memory_size, int _page_size){
		page_size = _page_size;
		pageNum = Memory_size / page_size;
		valid = new bool [pageNum];
		for(int i = 0; i < pageNum; ++i){
			valid[i] = false;
		}
	}

	int swap(){
		page_fault = true;
		for(int i = 0; i < pageNum; ++i){
			if(valid[i] == false){
				valid[i] = true;
				LRU.push_back(i);
				return i;
			}
		}
		int lru = LRU.front();
		LRU.pop_front();
		LRU.push_back(lru);
		return lru;
	}	
};

extern Cache *ICache, *DCache;
extern TLB *ITLB, *DTLB;
extern PTE *IPTE, *DPTE;
extern Memory *IMemory, *DMemory;
void InstrMemory(int,int,int,int,int);
void ICMP(int);
void DataMemory(int,int,int,int,int);
void DCMP(int);
void printCMP();
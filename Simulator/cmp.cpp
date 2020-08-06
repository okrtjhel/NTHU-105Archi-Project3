#include "libraries.h"
#include <fstream>
#include <iostream>

using namespace std;

Cache *ICache, *DCache;
TLB *ITLB, *DTLB;
PTE *IPTE, *DPTE;
Memory *IMemory, *DMemory;
bool page_fault;
extern int cycle;
fstream report("report.rpt", fstream::out);

void InstrMemory(int Memory_size, int page_size, int Cache_size, int block_size, int associate){
	ICache = new Cache(Cache_size, associate, block_size, page_size);
	ITLB = new TLB(page_size);
	IPTE = new PTE(page_size);
	IMemory = new Memory(Memory_size, page_size);
}

void DataMemory(int Memory_size, int page_size, int Cache_size, int block_size, int associate){
	DCache = new Cache(Cache_size, associate, block_size, page_size);
	DTLB = new TLB(page_size);
	DPTE = new PTE(page_size);
	DMemory = new Memory(Memory_size, page_size);
}

void ICMP(int VA){
	int PPN, PA;
	if(ITLB->isHit(VA)){	//TLB hit
		ITLB->hit++;
		//cout << "cycle " << cycle << ": ITLB hit " << ITLB->hit << endl;
		PPN = ITLB->find_PPN(VA);
		//check Cache
		PA = (PPN * IMemory->page_size) + (VA % IMemory->page_size);
		if(ICache->isHit(PA)){	//Cache hit
			ICache->hit++;
			//cout << "cycle " << cycle << ": ICache hit " << ICache->hit << endl;
		}
		else{	//Cache miss
			ICache->miss++;
			//cout << "cycle " << cycle << ": ICache miss " << ICache->miss << endl;
			ICache->update(PA);
		}
	}
	else{	//TLB miss
		ITLB->miss++;
		//cout << "cycle " << cycle << ": ITLB miss " << ITLB->miss << endl;
		if(IPTE->isHit(VA)){	//PTE hit
			IPTE->hit++;
			//cout << "cycle " << cycle << ": IPTE hit " << IPTE->hit << endl;
			PPN = IPTE->find_PPN(VA);
			PA = (PPN * IMemory->page_size) + (VA % IMemory->page_size);
			ITLB->update(VA, PA);
			if(ICache->isHit(PA)){	//Cache hit
				ICache->hit++;
				//cout << "cycle " << cycle << ": ICache hit " << ICache->hit << endl;
			}
			else{	//Cache miss
				ICache->miss++;
				//cout << "cycle " << cycle << ": ICache miss " << ICache->miss << endl;
				ICache->update(PA);
			}
		}
		else{	//PTE miss
			IPTE->miss++;
			//cout << "cycle " << cycle << ": IPTE miss " << IPTE->miss << endl;
			PPN = IMemory->swap();
			IPTE->update(VA, PPN);
			ITLB->update(VA, PPN);
			PA = (PPN * IMemory->page_size) + (VA % IMemory->page_size);
			ICache->update(PA);
			ICache->miss++;
		}
	}
}

void DCMP(int VA){
	int PPN, PA;
	if(DTLB->isHit(VA)){	//TLB hit
		DTLB->hit++;
		//cout << "cycle " << cycle << ": DTLB hit " << DTLB->hit << endl;
		PPN = DTLB->find_PPN(VA);
		//check Cache
		PA = (PPN * DMemory->page_size) + (VA % DMemory->page_size);
		if(DCache->isHit(PA)){	//Cache hit
			DCache->hit++;
			//cout << "cycle " << cycle << ": DCache hit " << DCache->hit << endl;
		}
		else{	//Cache miss
			DCache->miss++;
			//cout << "cycle " << cycle << ": DCache miss " << DCache->miss << endl;
			DCache->update(PA);
		}
	}
	else{	//TLB miss
		DTLB->miss++;
		//cout << "cycle " << cycle << ": DTLB miss " << DTLB->miss << endl;
		if(DPTE->isHit(VA)){	//PTE hit
			DPTE->hit++;
			//cout << "cycle " << cycle << ": DPTE hit " << DPTE->hit << endl;
			PPN = DPTE->find_PPN(VA);
			PA = (PPN * DMemory->page_size) + (VA % DMemory->page_size);
			DTLB->update(VA, PA);
			if(DCache->isHit(PA)){	//Cache hit
				DCache->hit++;
				//cout << "cycle " << cycle << ": DCache hit " << DCache->hit << endl;
			}
			else{	//Cache miss
				DCache->miss++;
				//cout << "cycle " << cycle << ": DCache miss " << DCache->miss << endl;
				DCache->update(PA);
			}
		}
		else{	//PTE miss
			DPTE->miss++;
			//cout << "cycle " << cycle << ": DPTE miss " << DPTE->miss << endl;
			PPN = DMemory->swap();
			DPTE->update(VA, PPN);
			DTLB->update(VA, PPN);
			PA = (PPN * DMemory->page_size) + (VA % DMemory->page_size);
			DCache->update(PA);
			DCache->miss++;
		}
	}
}

void printCMP(){
	report << "ICache :" << endl;
    report << "# hits: " << ICache->hit << endl;
	report << "# misses: " << ICache->miss << endl << endl;

    report << "DCache :" << endl;
    report << "# hits: " << DCache->hit << endl;
	report << "# misses: " << DCache->miss << endl << endl;

    report << "ITLB :" << endl;
    report << "# hits: " << ITLB->hit << endl;
	report << "# misses: " << ITLB->miss << endl << endl;

    report << "DTLB :" << endl;
    report << "# hits: " << DTLB->hit << endl;
	report << "# misses: " << DTLB->miss << endl << endl;

    report << "IPageTable :" << endl;
    report << "# hits: " << IPTE->hit << endl;
	report << "# misses: " << IPTE->miss << endl << endl;

    report << "DPageTable :" << endl;
    report << "# hits: " << DPTE->hit << endl;
	report << "# misses: " << DPTE->miss << endl << endl;
}
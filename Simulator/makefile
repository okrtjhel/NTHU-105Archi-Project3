CC = g++ -std=c++14
OBJS = simulator.o memory.o regfile.o instruction.o cmp.o

CMP:${OBJS}
	${CC} -o CMP ${OBJS}
regfile.o:regfile.cpp libraries.h
	${CC} -g -c regfile.cpp
memory.o:memory.cpp libraries.h
	${CC} -g -c memory.cpp
instruction.o:instruction.cpp libraries.h
	${CC} -g -c instruction.cpp
simulator.o:simulator.cpp libraries.h
	${CC} -g -c simulator.cpp 
cmp.o:cmp.cpp libraries.h
	${CC} -g -c cmp.cpp
clean:
	rm -f CMP ${OBJS}
clean_others:
	rm -f snapshot.rpt _snapshot.rpt report.rpt _report.rpt iimage.bin dimage.bin
diff_snap:
	diff snapshot.rpt _snapshot.rpt -u
diff_report:
	diff report.rpt _report.rpt -u
TARGETS = analyse
FLAGS = -g -O3 -D__USE_XOPEN2K8

DEPEND = include/*.h

#TRPAlignment.o:	src/TRPAlignment.cc include/TRPAlignment.h
#				g++ -I$(ROOTSYS)/include -c  src/TRPAlignment.cc 

all: $(TARGETS)

info:
	@echo "ROOTSYS: $(ROOTSYS)"
	@echo "targets: $(TARGETS)"
	@echo "flags: $(FLAGS)"

clean:
	rm -rf analyse

$(TARGETS) : %: %.cc
	g++ -Wall -Woverloaded-virtual -Wconversion $(FLAGS) -I$(ROOTSYS)/include -L$(ROOTSYS)/lib `root-config --libs` -lMinuit -I. -ldl -pthread $< -o $@

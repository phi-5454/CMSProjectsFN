all: accepc coulomb

accepc: accepc.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		accepc.cc -o accepc

coulomb: coulomb.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		coulomb.cc -o coulomb

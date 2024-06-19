all: accepc coulomb atest

accepc: accepc.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		accepc.cc -o accepc

coulomb: coulomb.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		coulomb.cc -o coulomb

atest: atest.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		atest.cc -o atest


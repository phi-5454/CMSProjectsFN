all: accepc 

accepc: accepc.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		accepc.cc -o accepc

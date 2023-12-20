all: plot plot2

plot: plot.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		plot.cc -o plot

plot2: plot2.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		plot2.cc -o plot2



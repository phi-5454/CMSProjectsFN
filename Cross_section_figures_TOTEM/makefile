all: plot plot2 template_test

plot: plot.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		plot.cc -o plot

plot2: plot2.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		plot2.cc -o plot2

template_test:template_test.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		template_test.cc -o template_test

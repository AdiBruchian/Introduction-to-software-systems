CXX=g++
CCLINK=$(CXX)
CXXFLAGS=-g -Wall -std=c++11
OBJS=string.o ip.o port.o main.o
EXEC=firewall.exe
RM=$(OBJS) $(EXEC)

$(EXEC): main.o libfirewall.so libinput.so
	$(CCLINK) main.o -o $(EXEC) -L. -linput  -L. -lfirewall

libfirewall.so: string.o ip.o port.o
	$(CCLINK) -shared string.o ip.o port.o -o libfirewall.so


string.o: string.cpp string.h
	$(CXX) $(CXXFLAGS) -c -fpic string.cpp

ip.o: ip.cpp ip.h generic-field.h string.h
	$(CXX) $(CXXFLAGS) -c -fpic ip.cpp

port.o: port.cpp port.h generic-field.h string.h
	$(CXX) $(CXXFLAGS) -c -fpic port.cpp	

main.o: input.h string.h generic-field.h ip.h port.h main.cpp
	$(CXX) $(CXXFLAGS) -c -fpic main.cpp

clean:
	rm -rf *.o libfirewall.so *.exe

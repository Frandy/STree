
CC = g++-4.7

GCCDIR = /usr/local/gcc47/gcc-4.7
CPP_INCLUDE_PATH = $(GCCDIR)/include
CPP_LIBRARY_PATH = $(GCCDIR)/lib

CFLAGS = -Wall -std=c++0x -I $(CPP_INCLUDE_PATH)
LFLAGS = -std=c++0x -L $(CPP_LIBRARY_PATH) 


INCLUDE = -I .
		  
SOURCES = 	ngraph.cpp \
			nstree.cpp \
			egraph.cpp \
			estree.cpp \
			test.cpp

OBJECTS = $(SOURCES:.cpp=.o)

TARGET = stree

.cpp.o:
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE)

all:$(TARGET)

$(TARGET):$(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)
	
clean:
	rm -rf $(OBJECTS)
	rm -rf $(TARGET)
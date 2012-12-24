
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
			createGraph.cpp
			
TEST_MAIN = test.cpp
			
TEST_NORMAL_MAIN = testNormal.cpp

TEST_ORDER_MAIN = testOrder.cpp


OBJECTS = $(SOURCES:.cpp=.o)

TEST_MAIN_OBJECT = $(TEST_MAIN:.cpp=.o)

TEST_NORMAL_OBJECT = $(TEST_NORMAL_MAIN:.cpp=.o)

TEST_ORDER_OBJECT = $(TEST_ORDER_MAIN:.cpp=.o)

TARGET = stree

TEST_NORMAL = test_normal

TEST_ORDER = test_order

.cpp.o:
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE)

all:stree

stree:$(TEST_MAIN_OBJECT) $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)

test_normal:$(TEST_NORMAL_OBJECT) $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)
	
test_order:$(TEST_ORDER_OBJECT) $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)	
	
clean:
	rm -rf *.o
	rm -rf $(TARGET)
	rm -rf $(TEST_NORMAL)
	rm -rf $(TEST_ORDER)
	
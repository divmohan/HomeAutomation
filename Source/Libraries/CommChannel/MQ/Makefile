CC=g++ 
CFLAGS=-Wall -std=c++11  -c -Iinc/ -I../../../CommonIncludes -I/usr/local/include -L/usr/local/lib 
LDFLAGS=-Wall -std=c++11 -L/usr/local/lib -lrt
SOURCES=$(shell find . -maxdepth 2 -name "*.cpp")
OBJECTS=$(SOURCES:.cpp=.o)
LOADLIBES = -L./
MSGLIB=libmsgexchange.a
COPY_LIB_TO_PATH=../../UserLibraries/

OBJECTS=$(SOURCES:.cpp=.o)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@
 
$(MSGLIB): $(OBJECTS)
	ar rcs $(MSGLIB) $(OBJECTS)
	cp $(MSGLIB) $(COPY_LIB_TO_PATH)
	
clean:
	rm -f $(OBJECTS) $(MSGLIB) Makefile.bak

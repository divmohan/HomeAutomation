CC=g++ 
CFLAGS=-Wall -std=c++11  -c -Iinc/ -I../../CommonIncludes  -I/usr/include/python2.7/
LDFLAGS=-Wall -std=c++11 ../../Libraries/UserLibraries/libbaseapp.a ../../Libraries/UserLibraries/libmsgexchange.a -L/usr/local/lib -lrt -lpthread -lpython2.7
SOURCES=$(shell find . -maxdepth 2 -name "*.cpp")
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=NotificationApp

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o

run: 
	./$(EXECUTABLE)
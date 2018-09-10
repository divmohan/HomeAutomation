CC=g++ 
CFLAGS=-Wall -std=c++11  -c -Iinc/ -I../../CommonIncludes
LDFLAGS=-Wall -std=c++11 -lrt -L../../Libraries/UserLibraries/libmsgexchange.a
SOURCES=$(shell find . -maxdepth 2 -name "*.cpp")
OBJECTS=$(SOURCES:.cpp=.o)
BASEAPPLIB=libbaseapp.a   
COPY_LIB_TO_PATH=../../Libraries/UserLibraries/
OBJECTS=$(SOURCES:.cpp=.o)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@
 
$(BASEAPPLIB): $(OBJECTS)
	ar rcs $(BASEAPPLIB) $(OBJECTS)
	cp $(BASEAPPLIB) $(COPY_LIB_TO_PATH)

clean:
	rm -rf *.o

run: 
	./$(EXECUTABLE)
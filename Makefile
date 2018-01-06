CC = g++ -std=c++11
CFLAGS = -c -Wall
LDFLAGS = -L. -l:libcurlpp.a -lcurl
IFLAGS = -I./include/
SOURCES = bot.cpp json.hpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = bot

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(IFLAGS) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)

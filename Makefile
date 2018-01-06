CC = g++
CFLAGS = -c -Wall
LDFLAGS = -lcurlpp -lcurl -lm -lc
SOURCES = bot.cpp json.hpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = bot

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SOURCES = $(wildcard src/*.cpp)
TARGET = pr2

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run1: $(TARGET)
	./$(TARGET) 1
run2: $(TARGET)
	./$(TARGET) 2
run3: $(TARGET)
	./$(TARGET) 3
run4: $(TARGET)
	./$(TARGET) 4
run5: $(TARGET)
	./$(TARGET) 5
run6: $(TARGET)
	./$(TARGET) 6
run7: $(TARGET)
	./$(TARGET) 7

clean:
	rm -f $(TARGET)

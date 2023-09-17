CXX = g++
CXXFLAGS = -std=c++11 -Wall

TARGET = my_program

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean

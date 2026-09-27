CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -lpthread

TARGET = planificador
OBJS = main.o executor.o dag.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

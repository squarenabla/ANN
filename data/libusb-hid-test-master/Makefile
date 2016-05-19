CXXFLAGS = -O2
INCLUDES = -I /usr/include/libusb-1.0/
LIBS = -lusb-1.0
CXX = clang++

SRC_CPP = $(wildcard *.cpp)
OBJS = $(SRC_CPP:.cpp=.o)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES)  $^ -o $@ -c

all: $(OBJS)
	$(CXX) $(OBJS) $(LIBS) $(CXXFLAGS) $(INCLUDES) -o test

clean:
	rm *.o && rm test

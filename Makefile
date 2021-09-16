OPT = -O2
# Optimization option
## -O0: quick compilation, but the resulting program will be slow
## -O1: good trade-off between compilation time and execution speed
## -O2: produces programs which have almost all possible speedups, but compilation takes longer
## -O3: enables auto vectorisation when using gcc

EXTRA_OPT = -fwhole-program
## Uncomment the above line if you're compiling all source files into one program in a single hit

#DEBUG = -g
#Deccomment the above line to enable debugging, e.g. via gdb


EXTRA_FLAG = -Wall -Wno-sign-compare
# Ask for all availabel warnings at compile-time
# (except sign-compare warnings)

CXX_STD = -std=c++11
# Use C++11 standard

CXXFLAGS = $(CXX_STD) $(DEBUG) $(OPT) $(EXTRA_OPT) $(EXTRA_FLAG)


SRC = main.cpp random_wrapper.h
#list here all source and header files

TARGET = hard_rod

$(TARGET):	$(OBJS) $(SRC)
	$(CXX) -o $(TARGET) $(CXXFLAGS) $(SRC)


all:	$(TARGET)


clean:
	rm -f $(OBJS) $(TARGET)

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
INCLUDES = -I.

# List of algorithms
ALGORITHMS = gift_wrapping graham_scan monotone_chain

# Default target: build all algorithms
all: $(ALGORITHMS)

# Pattern rule for each algorithm
%: convexHull_implementations/%.cpp geo_prop.cpp
	$(CXX) $(CXXFLAGS) -o convexHull_implementations/$@ $^ $(INCLUDES)

clean:
	rm -f $(addprefix convexHull_implementations/,$(ALGORITHMS))

.PHONY: all clean
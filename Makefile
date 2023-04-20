# Set gcc as the C++ compiler
CXX=g++

# Enforce C++ 2011 as the language standard
CXXFLAGS=-std=c++11 -pedantic

# Enable all standard warnings, uninitialized variable warnings, 
# and treat all warnings as errors
CXXFLAGS+= -Wall -Wuninitialized -Werror

# Generate code with debugging information
# (for use with a debugger like gdb)
CXXFLAGS+= -g 


# Enable ASan (Address Sanitizer) 
# and UBSan (Undefined Behavior Sanitizer)
#
# NOTE: comment these temporarily if 
# your development environment is failing
# due to these settings - it is important that 
# you fix your environment at some point.
CXXFLAGS+= -fsanitize=address -fsanitize=undefined



# C++ source files to consider in compilation for all programs
COMMON_CPP_FILES=Color.cpp Image.cpp Script.cpp PNG.cpp XPM2.cpp 

# C++ header files to consider in compilation
HEADERS=${COMMON_CPP_FILES:.cpp=.hpp}


PROGRAMS=runscript test

all: $(PROGRAMS)


runscript:  $(HEADERS) $(COMMON_CPP_FILES) runscript.cpp
	$(CXX) -o runscript runscript.cpp $(COMMON_CPP_FILES) $(CXXFLAGS)

test:  $(HEADERS) $(COMMON_CPP_FILES) test.cpp
	$(CXX) -o test test.cpp $(COMMON_CPP_FILES) $(CXXFLAGS)

clean: 
	rm -fr *.dSYM $(PROGRAMS)

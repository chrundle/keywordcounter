#==========================================================
# Makefile: For compiling keywordcounter program
#==========================================================

default: keywordcounter

I = -IInclude

C = g++ -std=c++11 $(I)
VALGRIND_C = g++ -std=c++11 -g -O0 $(I)

INCLUDE = Include/keywordcounter.h

DEBUG_FLAGS = -DDBUG_PRINT -DDEBUG_MAIN

#----------------------------------------------------------
# OBJECTS
#----------------------------------------------------------

OBJ = keywordcounter.o

#----------------------------------------------------------

$(OBJ): $(INCLUDE)

#----------------------------------------------------------
# keywordcounter object file
#----------------------------------------------------------

keywordcounter: $(INCLUDE) Source/keywordcounter.cpp
	$(C) -o keywordcounter Source/keywordcounter.cpp

#----------------------------------------------------------
# keywordcounter object file with valgrind
#----------------------------------------------------------

use_valgrind: $(INCLUDE) Source/keywordcounter.cpp
	$(VALGRIND_C) -o keywordcounter Source/keywordcounter.cpp

#----------------------------------------------------------
# keywordcounter object file with debug info printing
#----------------------------------------------------------

debug: $(INCLUDE) Source/keywordcounter.cpp
	$(C) $(DEBUG_FLAGS) -o keywordcounter Source/keywordcounter.cpp

#----------------------------------------------------------
# keywordcounter object file with routine for ordered list 
#----------------------------------------------------------

generate_solution: $(INCLUDE) Source/keywordcounter.cpp
	$(C) -DGENERATE_SOLUTION -o keywordcounter Source/keywordcounter.cpp

#----------------------------------------------------------
# PURGE
#----------------------------------------------------------

purge: clean

#----------------------------------------------------------
# CLEAN
#----------------------------------------------------------

clean:
	- $(RM) *.o

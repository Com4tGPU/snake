# check os
ifeq '$(findstring ;,$(PATH))' ';'
    RM :=      del /f /q
    EXE_EXT := .exe
else
    RM :=      rm -f
    EXE_EXT := 
endif

CXX ?= c++

main$(EXE_EXT): main.cpp
	$(CXX) -Wall -Wextra main.cpp -o main

clean:
	$(RM) main$(EXE_EXT)

.PHONY: clean

# check os
ifeq '$(findstring ;,$(PATH))' ';'
    RM :=      del /f /q
    EXE_EXT := .exe
    DIR_SEP := \\
else
    RM :=      rm -f
    EXE_EXT := 
    DIR_SEP := /
endif

CXX ?= c++
TARGET = snake

$(TARGET)$(EXE_EXT): src$(DIR_SEP)main.cpp
	$(CXX) -Wall -Wextra src$(DIR_SEP)main.cpp -o $(TARGET)$(EXE_EXT)

clean:
	$(RM) $(TARGET)$(EXE_EXT)

.PHONY: clean

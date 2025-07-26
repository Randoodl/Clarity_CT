#Compiler
CXX = g++

#Relevant Directories
SOURCE_DIR = ./src
BUILD_DIR  = ./build
OBJ_DIR    = $(BUILD_DIR)/obj
OUTPUT     = $(BUILD_DIR)/Clarity_CT

#Compiler flags
CXXFLAGS = -std=c++20 -Wall -Werror -Wformat -O2 

#Linker options
LIBS = -lraylib

#Lists of sourcefile names
SOURCENAMES := $(wildcard $(SOURCE_DIR)/*.cpp)

#Derrived object file names
OBJECTNAMES := $(patsubst $(SOURCE_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCENAMES))

#object file linking
$(OUTPUT): $(OBJECTNAMES)
	$(CXX) -o $@ $^  $(LIBS) 

#object file generation
$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(OUTPUT)
	./$<

check: $(OUTPUT)
	valgrind --leak-check=full ./$<

clean:
ifneq ($(wildcard $(BUILD_DIR)),)
	@rm -rf $(BUILD_DIR)
endif

.PHONY: run check clean
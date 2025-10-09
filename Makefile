#General variables
CXX = g++
SOURCE_DIR = ./src
BUILD_DIR  = ./build
SOURCE_DIR = ./src
BUILD_DIR  = ./build
OBJ_DIR    = $(BUILD_DIR)/obj
OUTPUT = $(BUILD_DIR)/Clarity_CT

#OS checks for Windows or otherwise Linux and set libraries correctly
ifeq ($(OS), Windows_NT)
  PLATFORM = WINDOWS
  LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm -static
else
  PLATFORM = LINUX
  LIBS = -lraylib 
endif

#Compiler flags
CXXFLAGS = -std=c++20 -Wall -Werror -Wformat -O2 

#Lists of source file names
SOURCENAMES := $(wildcard $(SOURCE_DIR)/*.cpp)

#Derrived object file names
OBJECTNAMES := $(patsubst $(SOURCE_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCENAMES))

#Object file linking
$(OUTPUT): $(OBJECTNAMES)
	$(CXX) -o $@ $^  $(LIBS) 

#Object file generation
$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(OUTPUT)
ifeq ($(PLATFORM), WINDOWS)
	.\$<
else
	./$<
endif

clean:
	@rm -rf $(BUILD_DIR)

#For linux debugging purposes only
check: $(OUTPUT)
	valgrind -s --log-file=./build/log_"`date +%Y-%m-%d-%T`".txt --leak-check=full ./$<


.PHONY: run check clean




	
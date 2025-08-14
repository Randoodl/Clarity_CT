#Currently broken for Windows 

#OS checks for Windows or otherwise Linux
ifeq ($(OS), Windows_NT)
  CXX = g++
  LIBS = -lraylib -lgd32 -lwinmm
  SOURCE_DIR = .\src
  BUILD_DIR  = .\build
  OBJ_DIR    = $(BUILD_DIR)\obj
  OUTPUT = $(BUILD_DIR)\Clarity_CT.exe
  RM = rmdir -r
  MKDIR = @if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
  PLATFORM = WINDOWS
else
  CXX = g++
  LIBS = -lraylib 
  SOURCE_DIR = ./src
  BUILD_DIR  = ./build
  OBJ_DIR    = $(BUILD_DIR)/obj
  OUTPUT = $(BUILD_DIR)/Clarity_CT
  RM = rm -rf
  MKDIR = mkdir -p
  PLATFORM = LINUX
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
	@$(MKDIR) $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(OUTPUT)
ifeq ($(PLATFORM), WINDOWS)
	.\$<
else
	./$<
endif

#For debugging purposes only
check: $(OUTPUT)
	valgrind -s --log-file=./build/log_"`date +%Y-%m-%d-%T`".txt --leak-check=full ./$<

clean:
	@$(RM) $(BUILD_DIR)

.PHONY: run check clean
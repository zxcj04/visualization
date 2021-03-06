EXE = main
OBJ_DIR = obj
COLOR_DIR = color_template

SOURCES = $(filter-out test.cpp, $(wildcard *.cpp))

ifeq ($(findstring test, $(MAKECMDGOALS)), test)
	SOURCES = $(filter-out main.cpp, $(wildcard *.cpp))
endif

SOURCES += $(wildcard src/glad/*.c)
SOURCES += $(wildcard src/imgui/*.cpp)
SOURCES += $(wildcard src/implot/*.cpp)
SOURCES += $(wildcard src/*.cpp)

OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

CXXFLAGS = -std=c++17 -I./include -Wall

LIBS =

mkdir =
mkdir_color =
rm =

ifeq ($(findstring static, $(MAKECMDGOALS)), static)
	CXXFLAGS += -static
endif

ifeq ($(OS), Windows_NT)
	CXXFLAGS += -I./include/windows

	LIBS += -L./lib/windows -lglfw3 -lopengl32 -lgdi32 -limm32

	mkdir = if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	mkdir_color = if not exist $(COLOR_DIR) mkdir $(COLOR_DIR)

	rm = del /S /Q *.exe *.out imgui.ini transfer_function.txt & rmdir /S /Q $(OBJ_DIR)
else ifeq ($(findstring Microsoft, $(shell uname -a)), Microsoft)
	CXXFLAGS += -I./include/windows

	CXX = cmd.exe /C g++
	EXE = main.exe

	LIBS += -L./lib/windows -lglfw3 -lopengl32 -lgdi32 -limm32

	mkdir = mkdir -p $(OBJ_DIR)
	mkdir_color = mkdir -p $(COLOR_DIR)

	rm = rm -rf *.exe *.out imgui.ini $(OBJ_DIR) transfer_function.txt
else
	LIBS += $(shell pkg-config --static --libs glfw3)

	mkdir = mkdir -p $(OBJ_DIR) $(COLOR_DIR)
	mkdir_color = mkdir -p $(COLOR_DIR)

	rm = rm -rf main *.exe *.out imgui.ini $(OBJ_DIR) transfer_function.txt
endif

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: src/glad/%.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: src/imgui/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: src/implot/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all static test: create_directory $(EXE) run
	@echo Compile and Execute Success

create_directory:
	$(call mkdir)
	$(call mkdir_color)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

run:
	./$(EXE)

clean:
	$(call rm)
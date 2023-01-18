CHECK			?= none
COMPILER		?= clang++
DEBUGGER		?= lldb
OPTIMIZATION	?= -O0 -g
STANDARD		?= c++20
WARNINGS		?= -Wall -Wextra
CFLAGS			:= -std=$(STANDARD) $(OPTIMIZATION) $(WARNINGS) -Iinclude -Ijson/single_include
OUTPUT			?= interpreter
LDFLAGS			?= -pthread

CLOC_OPTIONS	:= --exclude-dir=.vscode
SOURCES			:= $(shell find src/*.cpp)
OBJECTS			:= $(SOURCES:.cpp=.o)

ifeq ($(CHECK), asan)
	COMPILER := $(COMPILER) -fsanitize=address -fno-common
else ifeq ($(CHECK), msan)
	COMPILER := $(COMPILER) -fsanitize=memory  -fno-common
endif

.PHONY: all clean count countbf declutter

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(COMPILER) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(COMPILER) $(CFLAGS) -c $< -o $@

test: $(OUTPUT)
	./$(OUTPUT) < js/assign.js

clean:
	rm -f $(OUTPUT) src/*.o src/**/*.o PVS-Studio.log report.tasks strace_out

tidy:
	clang-tidy src/*.cpp -- $(CFLAGS)

grind: $(OUTPUT)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --show-reachable=no ./$<

count:
	cloc . $(CLOC_OPTIONS)

countbf:
	cloc --by-file . $(CLOC_OPTIONS)

DEPFILE  = .dep
DEPTOKEN = "\# MAKEDEPENDS"
DEPFLAGS = -f $(DEPFILE) -s $(DEPTOKEN)

depend:
	@ echo $(DEPTOKEN) > $(DEPFILE)
	makedepend $(DEPFLAGS) -- $(COMPILER) $(CFLAGS) -- $(SOURCES) 2>/dev/null
	@ rm $(DEPFILE).bak

sinclude $(DEPFILE)

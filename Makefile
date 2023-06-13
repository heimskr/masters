CHECK           ?= none
COMPILER        ?= clang++
DEBUGGER        ?= lldb
OPTIMIZATION    ?= -O0 -g
STANDARD        ?= c++20
WARNINGS        ?= -Wall -Wextra
CFLAGS          := -std=$(STANDARD) $(OPTIMIZATION) $(WARNINGS) -Iinclude -Ijson/single_include
OUTPUT          ?= interpreter
LDFLAGS         ?= -pthread

CLOC_OPTIONS    := --exclude-dir=.vscode,node_modules,json,js --fullpath --not-match-f='^.\/((include|src)\/(flex|bison)\.(cpp|h|output)|.*\.js(on)?)$$'

LEXERCPP        := src/flex.cpp
PARSERCPP       := src/bison.cpp
PARSERHDR       := include/bison.h
LEXERSRC        := src/lexer.l
PARSERSRC       := src/parser.y

LEXFLAGS        := -Wno-sign-compare -Wno-register
BISONFLAGS      := --color=always

SOURCES         := $(shell find src/*.cpp) $(LEXERCPP) $(PARSERCPP)
OBJECTS         := $(SOURCES:.cpp=.o)

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

$(LEXERCPP): $(LEXERSRC) $(PARSERHDR)
	flex --prefix=js --outfile=$(LEXERCPP) $(LEXERSRC)

$(PARSERCPP) $(PARSERHDR): $(PARSERSRC)
	bison $(BISONFLAGS) --defines=$(PARSERHDR) --output=$(PARSERCPP) $(PARSERSRC)

counter:
	bison -Wcounterexamples $(BISONFLAGS) --defines=$(PARSERHDR) --output=$(PARSERCPP) $(PARSERSRC)

$(LEXERCPP:.cpp=.o): $(LEXERCPP)
	$(COMPILER) $(CFLAGS) $(LEXFLAGS) -c $< -o $@

$(PARSERCPP:.cpp=.o): $(PARSERCPP) $(PARSERHDR)
	$(COMPILER) $(CFLAGS) $(LEXFLAGS) -c $< -o $@

test: $(OUTPUT)
	./$(OUTPUT) < js/delete.js

define newline


endef

fulltest: $(OUTPUT)
	$(foreach file,$(wildcard js/*.js),@ printf "\x1b[35m$(file)\x1b[39m\n" $(newline) @ ./$(OUTPUT) < $(file) $(newline))

clean:
	rm -f $(OUTPUT) src/*.o src/**/*.o PVS-Studio.log report.tasks strace_out $(LEXERCPP) $(PARSERCPP) $(PARSERHDR)

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

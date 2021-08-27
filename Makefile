.SUFFIXES:
.SUFFIXES: .c .o .d .h .cpp .hpp

PROJECT_NAME := template

COMMON_FLAGS := -fexceptions
debug ?= false
ifeq (${debug}, true)
	COMMON_FLAGS += -g3
else
	COMMON_FLAGS += -O2 -D_FORTIFY_SOURCE=2 -DNDEBUG
endif

profile ?= false
ifeq (${profile}, true)
	COMMON_FLAGS += -pg
endif

sanitize ?= false
ifeq (${sanitize}, true)
	COMMON_FLAGS += -fsanitize=address -fsanitize=leak -fsanitize=undefined
endif

C_SOURCES := $(wildcard src/*.c)
CPP_SOURCES := $(wildcard src/*.cpp)
C_OBJECTS := $(patsubst %.c,%.o,${C_SOURCES})
CPP_OBJECTS := $(patsubst %.cpp,%.o,${CPP_SOURCES})
OBJECTS := ${CPP_OBJECTS} ${C_OBJECTS}
DEPENDENCIES := $(patsubst %.c,%.d,${C_SOURCES}) $(patsubst %.cpp,%.d,${CPP_SOURCES})

INCLUDE_FLAGS := -I./include -I./simple_dict/include
WARNING_FLAGS := -Wextra -Wall -Wshadow -Wdouble-promotion \
	-Wformat=2 -Wformat-truncation -fno-common -Wconversion -Warray-bounds \
	-Wtrampolines
CFLAGS += ${WARNING_FLAGS} ${INCLUDE_FLAGS} ${COMMON_FLAGS}
CPPFLAGS += ${CFLAGS}

PREFIX ?= /usr
export

.PHONY: clean tests coverage library objects install
library: ${OBJECTS} | lib
	ar -rc lib/lib${PROJECT_NAME}.a $^

objects: ${OBJECTS}

${C_OBJECTS}: %.o: %.c
	${CC} -Werror ${CFLAGS} -MMD -c $< -o $@

${CPP_OBJECTS}: %.o: %.cpp
	${CC} -Werror ${CPPFLAGS} -MMD -c $< -o $@

tests: ${OBJECTS}
	${MAKE} -C tests tests
	LSAN_OPTIONS=verbosity=1:log_threads=1 ./tests/tests

lib:
	mkdir lib

-include $(DEPENDENCIES)

clean:
	-rm -rf ${OBJECTS} ${DEPENDENCIES} lib
	-$(MAKE) -C tests clean

install:
	cp lib/* ${PREFIX}/lib
	cp include/* ${PREFIX}/include

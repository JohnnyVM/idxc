PROJECT_NAME := template

debug ?= false
ifeq (${debug}, true)
	COMMON_FLAGS := -g3 -fsanitize=address -fsanitize=leak -fsanitize=undefined
else
	COMMON_FLAGS := -O2 -D_FORTIFY_SOURCE=2 -DNDEBUG
endif

profile ?= false
ifeq (${profile}, true)
	COMMON_FLAGS += -pg
endif

SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst %.c,%.o,${SOURCES})
DEPENDENCIES := $(patsubst %.c,%.d,${SOURCES})

INCLUDE_FLAGS := -I./include -I./simple_dict/include
WARNING_FLAGS := -Wextra -Wall -Wshadow -Wdouble-promotion -Wpadded \
	-Wformat=2 -Wformat-truncation -fno-common -Wconversion -Warray-bounds \
	-Wtrampolines -fanalyzer
CFLAGS += ${WARNING_FLAGS} ${INCLUDE_FLAGS} ${COMMON_FLAGS}
export

.PHONY: clean tests coverage library
${OBJECTS}: %.o: %.c
	${CC} -Werror ${CFLAGS} -MMD -c $< -o $@

library: ${OBJECTS} | lib
	ar -rc lib/lib${PROJECT_NAME}.a $^

tests: ${OBJECTS}
	${MAKE} -C tests tests
	LSAN_OPTIONS=verbosity=1:log_threads=1 ./tests/tests

lib:
	mkdir lib

-include $(DEPENDENCIES)

clean:
	-rm -rf ${OBJECTS} ${DEPENDENCIES} lib
	-$(MAKE) -C tests clean

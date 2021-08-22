TEST_SOURCES := $(wildcard src/*.cpp)
TEST_OBJECTS := $(patsubst %.cpp,%.o,${TEST_SOURCES})
TEST_DEPENDENCIES := $(patsubst %.cpp,%.d,${TEST_SOURCES})

TEST_C_SOURCES := $(wildcard src/*.c)
TEST_C_OBJECTS := $(patsubst %.c,%.o,${TEST_C_SOURCES})
TEST_C_DEPENDENCIES := $(patsubst %.c,%.d,${TEST_C_SOURCES})

OBJECTS := $(patsubst %,../%,${OBJECTS}) ${TEST_OBJECTS} ${TEST_C_OBJECTS}

INCLUDE_FLAGS := -I../include -I$(CPPUTEST_HOME)/include
LD_FLAGS += -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt
CPPFLAGS += ${INCLUDE_FLAGS} ${COMMON_FLAGS}
CFLAGS += ${INCLUDE_FLAGS} ${COMMON_FLAGS}

tests: ${TEST_OBJECTS} ${TEST_C_OBJECTS}
	${CXX} ${CPPFLAGS} -MMD -o $@ ${OBJECTS} ${LD_FLAGS}

${TEST_OBJECTS}: %.o: %.cpp
	${CXX} ${CPPFLAGS} -MMD -c $< -o $@

${TEST_C_OBJECTS}: %.o: %.c
	${CC} ${CFLAGS} -MMD -c $< -o $@

-include $(TEST_DEPENDENCIES)

clean:
	-rm ${TEST_OBJECTS} ${TEST_DEPENDENCIES} ${TEST_C_OBJECTS} ${TEST_C_DEPENDENCIES} tests

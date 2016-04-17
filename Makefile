include Makefile.config

TGTS := main

EXES := ${TGTS}

.PHONY: clean

SRCS := $(wildcard *.cc)
OBJS := $(SRCS:.cc=.o)
DEPS := $(SRCS:.cc=.d)

ifeq ($(filter clean, ${MAKECMDGOALS}),)
include ${DEPS}
endif

clean:
	-rm -rf ${EXES} *.d *.o *.so *~ *.root *.png validation/

main: ${OBJS} 
	${CXX} ${CXXFLAGS} -o $@ ${OBJS} ${LDFLAGS}

${OBJS}: %.o: %.cc %.d
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o $@ $<
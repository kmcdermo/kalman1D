CPPFLAGS := $(shell root-config --cflags)
CXXFLAGS := -std=c++11 -g -O3
LDFLAGS  := $(shell root-config --libs)

TGTS := main

EXES := ${TGTS}

SRCDIR := src
OBJDIR := bin
DEPDIR := dep

SRCS := $(wildcard ${SRCDIR}/*.cc)
OBJS := $(addprefix ${OBJDIR}/,$(notdir $(SRCS:.cc=.o)))
DEPS := $(addprefix ${DEPDIR}/,$(notdir $(SRCS:.cc=.d)))

${OBJDIR}/%.o: ${SRCDIR}/%.cc ${DEPDIR}/%.d
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -o $@ -c $<

${DEPDIR}/%.d: ${SRCDIR}/%.cc
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -MM -MT $(patsubst ${SRCDIR}/%.cc,${OBJDIR}/%.o,$<) $< -MF $@

main: ${OBJS} 
	${CXX} ${CXXFLAGS} -o $@ ${OBJS} ${LDFLAGS}

${OBJS}: | ${OBJDIR}

${DEPS}: | ${DEPDIR}

${OBJDIR}:
	mkdir -p ${OBJDIR}

${DEPDIR}:
	mkdir -p ${DEPDIR}

# ROOT6
HEADDIR := interface
HEADERS := $(wildcard ${HEADDIR}/*.hh)

${SRCDIR}/dict.cc: ${HEADERS} 
	rootcling -f $@ -c -p $^

clean:
	-rm -f ${EXES} ${DEPDIR}/*.d ${OBJDIR}/*.o ${SRCDIR}/*.pcm ./*/*~
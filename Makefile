#Makefile for libshm

CXX	 = g++
LD	 = g++
AR	 = ar rc

CXX_STD = -std=c++0x
DEBUG_FLAG = -g
WARNING_FLAGS = -Wall -Wextra
CXX_MACROS = -DDEBUG
EXTRA_FLAGS = -Wfatal-errors
CXXFLAGS = $(CXX_STD) $(DEBUG_FLAG) $(WARNING_FLAGS) $(CXX_MACROS) $(EXTRA_FLAGS)
INCS = 

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
OUTPUT = libshm
TEMPFILE = *.o *.out core.* core

all: ${OUTPUT}

${OUTPUT}: $(OBJS)
	$(AR) $@ $(OBJS) $(EXTRA_LIB)

.cpp.o:
	${CXX} -c ${CXXFLAGS} ${INCS} $< -o $@

clean:
	-rm -f core $(OUTPUT) ${OBJS} $(TEMPFILE)



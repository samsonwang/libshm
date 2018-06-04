#Makefile for libshm

CXX	 = g++
LD	 = g++
AR	 = ar rc

CXX_STD = -std=c++0x
DEBUG_FLAG = -g
WARNING_FLAGS = -Wall -Wextra -pedantic \
			-Wswitch-default -Wundef -Wpointer-arith \
			-Woverloaded-virtual -Wnon-virtual-dtor \
			-Wno-unused-parameter -Wfloat-equal \
			-Wformat-security -Wformat-nonliteral \
			-Wuninitialized -Wshadow -Wlogical-op \
			-Wmissing-declarations -Winline

CXX_MACROS = -DLOGDEBUG
EXTRA_FLAGS = -Wfatal-errors
CXXFLAGS = $(CXX_STD) $(DEBUG_FLAG) $(WARNING_FLAGS) $(CXX_MACROS) \
		-ftemplate-depth-25 $(EXTRA_FLAGS)

INCS = -I$(XMLPATH) -I$(SYSLIBPATH) -I$(TOOLPATH) -I$(PROBASELPATH) \
		-I$(DEVLIBPATH) -I$(LOADDEVICEPATH)

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRCS))

TEMPFILE = *.o *.out core.* core

OUTPUT = libshm

all: ${OUTPUT}

${OUTPUT}: $(OBJS)
	$(AR) $@ $(OBJS) $(EXTRA_LIB)

# Rules for compiling source files to object files
.cpp.o:
	${CXX} -c ${CXXFLAGS} ${INCS} $< -o $@

clean:
	-rm -f core $(OUTPUT) ${OBJS} $(TEMPFILE)



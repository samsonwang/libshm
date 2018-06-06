
CXX	 = g++
LD	 = g++
AR	 = ar rc

CXX_STD = -std=c++0x
DEBUG_FLAG = -g
WARNING_FLAGS = -Wall -Wextra
CXX_MACROS = -DDEBUG
EXTRA_FLAGS = -Wfatal-errors
CXXFLAGS = $(CXX_STD) $(DEBUG_FLAG) $(WARNING_FLAGS) $(CXX_MACROS) \
		-ftemplate-depth-25 $(EXTRA_FLAGS)
INCS = -I../

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRCS))

TEMPFILE = *.o *.out core.* core

OUTPUT = shm_create.out shm_attach.out

all: ${OUTPUT}

%.out: %.cpp ../libshm
	$(CXX) $(CXXFLAGS) $(INCS) $< ../libshm -o $@

#${OUTPUT}: $(OBJS)
#	$(AR) $@ $(OBJS) $(EXTRA_LIB)

# Rules for compiling source files to object files
#.cpp.o:
#	${CXX} -c ${CXXFLAGS} ${INCS} $< -o $@

clean:
	-rm -f core $(OUTPUT) ${OBJS} $(TEMPFILE)



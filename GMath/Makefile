all: gmath.so

CXXFLAGS = -Wall  -Wno-comment -Werror -fPIC -I../GSystem
SRCS = $(wildcard *.cpp)

include ../build.mk
gmath.so: $(OBJS)
	$(LINK.cpp.so) $(OBJS) -o $@


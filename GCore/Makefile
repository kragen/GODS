all: gcore.so

CXXFLAGS = -Wall -Wno-unused-variable -Wno-unused-function -Werror \
	-fPIC -I../GSystem -I../GMath
SRCS = $(wildcard *.cpp)

include ../build.mk
gcore.so: $(OBJS)
	$(LINK.cpp.so) $(OBJS) -o $@


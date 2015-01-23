all: gsystem.so

CXXFLAGS = -Wall -Wno-unused-variable -Werror -fPIC
SRCS = $(wildcard *.cpp)

include ../build.mk
gsystem.so: $(OBJS)
	$(LINK.cpp.so) $(OBJS) -o $@


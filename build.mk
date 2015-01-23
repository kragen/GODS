# To be included in Makefiles for particular directories.
# Define SRCS and optionally CXXFLAGS; it will do dependency checking, give you `clean`,
# and give you OBJS and LINK.cpp.so

LINK.cpp.so = $(CXX) -shared $(LDFLAGS)

include $(SRCS:.cpp=.d)
OBJS = $(SRCS:.cpp=.o)
clean:
	$(RM) *.o *.so *.d

# Adapted from
# https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html
%.d: %.cpp Makefile ../build.mk
	$(RM) $@
	$(CPP) -MM $(CXXFLAGS) $< > $@

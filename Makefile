# Really simple, dumb, build script.

all:
	$(MAKE) -C GSystem
	$(MAKE) -C GMath
	$(MAKE) -C GCore

clean:
	$(MAKE) -C GSystem clean
	$(MAKE) -C GMath clean
	$(MAKE) -C GCore clean

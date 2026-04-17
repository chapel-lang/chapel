all:
	@#gcc may have different errors on different platforms, so just simulate an error
	@#gcc -c -fPIE hello.c
	@cat hello.c
	@echo "error: missing ';'"
	@exit 1

printchplflags:
	@$(info prereqs/some-c-lib/hello.h prereqs/some-c-lib/hello.o)

clean:
	rm -f hello.o
	

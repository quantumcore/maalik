COMPILER 				:=
ifeq ($(OS),Windows_NT)
	COMPILER += gcc
else
	COMPILER += i686-w64-mingw32-gcc
endif

reflective_default:
	$(COMPILER) ReflectiveDll.c ReflectiveLoader.c -c -m32
	$(COMPILER) ReflectiveDll.o ReflectiveLoader.o -o reflective.dll -shared -m32 -static
	
ncshell:
	$(COMPILER) ncshell.c ReflectiveLoader.c -c -m32
	$(COMPILER) ncshell.o ReflectiveLoader.o -o ncshell.dll -shared -m32 -static -lws2_32

test:
	$(COMPILER) test.c ReflectiveLoader.c -c -m32
	$(COMPILER) test.o ReflectiveLoader.o -o test.dll -shared -m32 -static	
COMPILER 				:=
ifeq ($(OS),Windows_NT)
	COMPILER += gcc
else
	COMPILER += i686-w64-mingw32-gcc
endif

CPCOMPILER 				:=
ifeq ($(OS),Windows_NT)
	CPCOMPILER += g++
else
	CPCOMPILER += i686-w64-mingw32-g++
endif

all: reflective_default ncshell elevate ChromeDump

reflective_default:
	$(COMPILER) ReflectiveDll.c ReflectiveLoader.c -c -m32
	$(COMPILER) ReflectiveDll.o ReflectiveLoader.o -o reflective.dll -shared -m32 -static
	
ncshell:
	$(COMPILER) ncshell.c ReflectiveLoader.c -c -m32
	$(COMPILER) ncshell.o ReflectiveLoader.o -o ncshell.dll -shared -m32 -static -lws2_32

elevate:
	$(COMPILER) elevate.c ReflectiveLoader.c -c -m32
	$(COMPILER) elevate.c ReflectiveLoader.o -o elevate.dll -shared -m32 -static	

ChromeDump:
	$(COMPILER) ChromePassDecryptor.c ReflectiveLoader.c sqlite/sqlite3.c -c -m32
	$(COMPILER) ChromePassDecryptor.c ReflectiveLoader.o sqlite3.o -o ChromeDump.dll -shared -m32 -static -lcrypt32

msf:
	$(COMPILER) msf.c ReflectiveLoader.c -c -m32
	$(COMPILER) msf.c ReflectiveLoader.o -o msf.dll -shared -m32 -static
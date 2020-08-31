reflective_default:
	gcc ReflectiveDll.c ReflectiveLoader.c -c -m32
	gcc ReflectiveDll.o ReflectiveLoader.o -o reflective.dll -shared -m32 -static

ncshell:
	gcc ncshell.c ReflectiveLoader.c -c -m32
	gcc ncshell.o ReflectiveLoader.o -o ncshell.dll -shared -m32 -static -lws2_32
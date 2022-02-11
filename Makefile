CC="C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\gcc.exe"
INCLUDE=
LIBS=-L./lib
FLAGS=-O3 -Wall -lcurlgo -llibcurl -m32

main:
	$(CC) main.c -o main $(INCLUDE) $(LIBS) $(FLAGS)
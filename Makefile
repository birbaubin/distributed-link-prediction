HELPERS_SRC=src/util/*.cpp
OBJ=*.o

output: helpers main
	g++ ${OBJ} -o output

main: 
	g++ -c src/main.cpp 

helpers:
	g++ -c ${HELPERS_SRC}

clean:
	rm *.o output

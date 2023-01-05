HELPERS_SRC=src/util/*.cpp
CRYPTO_SRC=src/util/crypto/*.cpp
TESTS_SRC=src/tests/*.cpp
NEW_PROTOCOL_SRC=src/new_dist_link_prediction/*.cpp
OBJ=*.o
OPENSSL_FLAGS=-I/opt/homebrew/Cellar/openssl@3/3.0.7/include -L/opt/homebrew/Cellar/openssl@3/3.0.7/lib -lssl -lcrypto
GMP_FLAGS= -lgmp

output: helpers crypto tests new_protocol main
	g++ ${OBJ} -o output ${OPENSSL_FLAGS} ${GMP_FLAGS}

main: 
	g++ -c src/main.cpp ${OPENSSL_FLAGS} ${GMP_FLAGS}

new_protocol:
	g++ -c ${NEW_PROTOCOL_SRC} ${OPENSSL_FLAGS} ${GMP_FLAGS}

helpers:
	g++ -c ${HELPERS_SRC}

tests:
	g++ -c ${TESTS_SRC} ${OPENSSL_FLAGS} ${GMP_FLAGS}

crypto:
	g++ -c ${CRYPTO_SRC} ${OPENSSL_FLAGS} ${GMP_FLAGS}


clean:
	rm *.o output

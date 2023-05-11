HELPERS_SRC=src/util/*.cpp
CRYPTO_SRC=src/util/crypto/*.cpp
TESTS_SRC=src/tests/*.cpp
NEW_PROTOCOL_SRC=src/new_dist_link_prediction/*.cpp
BASELINE_PROTOCOL_SRC=src/baseline_dist_link_prediction/*.cpp
OBJ=*.o
OPENSSL_INCLUDE=-I/opt/homebrew/Cellar/openssl@3/3.0.8/include
GMP_INCLUDE=-I/opt/homebrew/include
OPENSSL_FLAGS=-I/opt/homebrew/Cellar/openssl@3/3.0.8/include -L/opt/homebrew/Cellar/openssl@3/3.0.8/lib -lssl -lcrypto
GMP_FLAGS= -lgmp
BOOST_FLAGS=-I/opt/homebrew/Cellar/boost/1.80.0/include -L/opt/homebrew/Cellar/boost/1.80.0/lib -lboost_chrono -lboost_serialization
STDFLAG=-std=c++17



all: helpers inline
	g++ ${OBJ} -o output ${OPENSSL_FLAGS} ${STDFLAG}

inline: 
	g++ -c src/main.cpp ${OPENSSL_INCLUDE} ${STDFLAG}

helpers:
	g++ -c ${HELPERS_SRC} ${OPENSSL_INCLUDE} ${STDFLAG}


clean:
	rm *.o output

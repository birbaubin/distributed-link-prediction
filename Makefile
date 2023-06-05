CPP =/opt/homebrew/Cellar/llvm/16.0.4/bin/clang++
HELPERS_SRC=src/util/*.cpp
CRYPTO_SRC=src/util/crypto/*.cpp
TESTS_SRC=src/tests/*.cpp
#NEW_PROTOCOL_SRC=src/new_dist_link_prediction/InlineProtocole.cpp
NEW_PROTOCOL_SRC=src/new_dist_link_prediction/inline_new.cpp
BASELINE_PROTOCOL_SRC=src/baseline_dist_link_prediction/*.cpp
OBJ=*.o
OPENSSL_INCLUDE=-I/opt/homebrew/Cellar/openssl@3/3.0.8/include
GMP_INCLUDE=-I/opt/homebrew/include
OPENSSL_FLAGS=-I/opt/homebrew/Cellar/openssl@3/3.0.8/include -L/opt/homebrew/Cellar/openssl@3/3.0.8/lib -lssl -lcrypto
GMP_FLAGS= -lgmp
STDFLAG=-std=c++17
OMP_FLAG=-I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib



all: helpers new_protocol inline
	g++ ${OBJ} -o output ${OPENSSL_FLAGS} ${STDFLAG}

inline: 
	g++ -c ${OMP_FLAG} src/main.cpp ${OPENSSL_INCLUDE} ${STDFLAG}

new_protocol:
	g++ -c ${NEW_PROTOCOL_SRC} ${STDFLAG} ${OPENSSL_INCLUDE} ${GMP_INCLUDE} ${OMP_FLAG}

helpers:
	g++ -c ${HELPERS_SRC} ${OPENSSL_INCLUDE} ${STDFLAG}


clean:
	rm  *.o output

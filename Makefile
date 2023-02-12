HELPERS_SRC=src/util/*.cpp
CRYPTO_SRC=src/util/crypto/*.cpp
TESTS_SRC=src/tests/*.cpp
NEW_PROTOCOL_SRC=src/new_dist_link_prediction/*.cpp
BASELINE_PROTOCOL_SRC=src/baseline_dist_link_prediction/*.cpp
OBJ=*.o
OPENSSL_INCLUDE=-I/opt/homebrew/Cellar/openssl@3/3.0.7/include
GMP_INCLUDE=-I/opt/homebrew/include
OPENSSL_FLAGS=-I/opt/homebrew/Cellar/openssl@3/3.0.7/include -L/opt/homebrew/Cellar/openssl@3/3.0.7/lib -lssl -lcrypto
GMP_FLAGS= -lgmp
BOOST_FLAGS=-I/opt/homebrew/Cellar/boost/1.80.0/include -L/opt/homebrew/Cellar/boost/1.80.0/lib -lboost_chrono -lboost_serialization
STDFLAG=-std=c++17



all: helpers crypto tests baseline_protocol new_protocol inline
	g++ ${OBJ} -o output ${OPENSSL_FLAGS} ${GMP_FLAGS} ${BOOST_FLAGS} ${STDFLAG}

inline: 
	g++ -c src/main.cpp ${OPENSSL_INCLUDE} ${STDFLAG} ${GMP_INCLUDE}

multiprocess:
	g++ -c src/multiparty_main.cpp ${OPENSSL_FLAGS} ${BOOST_FLAGS} ${STDFLAG}


new_protocol:
	g++ -c ${NEW_PROTOCOL_SRC} ${STDFLAG} ${OPENSSL_INCLUDE}  ${GMP_INCLUDE}

baseline_protocol:
	g++ -c ${BASELINE_PROTOCOL_SRC} ${OPENSSL_INCLUDE} ${STDFLAG}


helpers:
	g++ -c ${HELPERS_SRC} ${STDFLAG}

tests:
	g++ -c ${TESTS_SRC} ${STDFLAG}

crypto:
	g++ -c ${CRYPTO_SRC} ${STDFLAG} -I/opt/homebrew/Cellar/openssl@3/3.0.7/include


clean:
	rm *.o output

CXX=clang++
INCLUDES=-Iincludes/ -Ilib/
CXXEXTRAS=-Wall -Wextra -Werror -pedantic -lcurl
CXXFLAGS=-std=c++20 -g -fstandalone-debug -lcurl

exec: bin/exec
art: bin/art
tests: bin/tests
ant: bin/Antonysplayground

bin/exec: ./src/driver.cc ./src/retriever.cc ./src/CurlObj.cc
	$(CXX) $(CXXFLAGS) $(CXXEXTRAS) $(INCLUDES) $^ -o $@

bin/art: ./src/artsplayground.cc ./src/retriever.cc ./src/CurlObj.cc
	$(CXX) $(CXXFLAGS) $(CXXEXTRAS) $(INCLUDES) $^ -o $@

bin/tests: ./tests/tests.cc obj/catch.o ./src/retriever.cc ./src/CurlObj.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

obj/catch.o: tests/catch.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $^ -o $@

.DEFAULT_GOAL := exec
.PHONY: clean exec tests ant

clean:
	rm -rf bin/* obj/*

test_serialize:
	g++ -g -Wall -pedantic -std=c++11 test.cpp -o ts2
	./ts2

test_kvstore:
	g++ -g -Wall -pedantic -std=c++11 test_kvstore.cpp

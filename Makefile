test_serialize:
	g++ -g -Wall -pedantic -std=c++11 test.cpp -o ts1
	./ts1

test_kvstore:
	g++ -g -Wall -pedantic -std=c++11 test_kvstore.cpp -o ts2
	./ts2

run:
	make test_serialize
	make test_kvstore
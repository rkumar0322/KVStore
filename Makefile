test_serialize:
	g++ -g -Wall -pedantic -std=c++11 test.cpp -o ts1
	valgrind --leak-check=full ./ts1
	./ts1

test_kvstore:
	g++ -g -Wall -pedantic -std=c++11 test_kvstore.cpp -o ts3
	valgrind --leak-check=full ./ts3
	./ts3

network1:
	g++ -g -Wall -pthread -pedantic -std=c++11 main.cpp -o ts5
	valgrind --leak-check=full ./ts5
	./ts5

network_local:
	g++ -g -Wall -pthread -pedantic -std=c++11 main.cpp -o ts4
	./ts4

no_valgrind:
	g++ -g -Wall -pedantic -std=c++11 test.cpp -o ts1
	g++ -g -Wall -pedantic -std=c++11 test_kvstore.cpp -o ts3
	./ts1
	./ts3


run:
	docker build -t cs4500:0.1 .
	- rm -rf ./part1/test-array
	- docker run -ti -v `pwd`:/test cs4500:0.1 bash -c "cd /test;make network1"

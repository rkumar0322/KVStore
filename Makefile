test_serialize:
	g++ -g -Wall -pedantic -std=c++11 test.cpp -o ts1
	valgrind --leak-check=full ./ts1
	./ts1

test_kvstore:
	g++ -g -Wall -pedantic -std=c++11 test_kvstore.cpp -o ts3
	valgrind --leak-check=full ./ts3
	./ts3
	
test_keyvalue:
	g++ -g -Wall -pedantic -std=c++11 test_keyvalue.cpp -o ts7
	valgrind --leak-check=full ./ts7
	./ts7

test_map:
	g++ -g -Wall -pedantic -std=c++11 test_map.cpp -o ts8
	valgrind --leak-check=full ./ts8
	./ts8

network1:
	g++ -g -Wall -pthread -pedantic -std=c++11 main.cpp -o ts5
	valgrind --leak-check=full ./ts5
	./ts5 -nodes 3

wc:
	g++ -g -Wall -pthread -pedantic -std=c++11 main.cpp -o ts6
	./ts6 -nodes 3 -pseudo true -app wc -file 100k.txt

RANDOM:
	g++ -g -Wall -pthread -pedantic -std=c++11 main.cpp -o ts4
	./ts4 -nodes 3 -pseudo true -app wc -port 3780 -index 0 -masterip 127.0.0.1 -masterport 3780 -ip 127.0.0.1

demo:
	g++ -g -Wall -pthread -pedantic -std=c++11 main.cpp -o ts4
	./ts4 -nodes 3 -pseudo true -app demo -port 3780 -index 1 -masterip 127.0.0.1 -masterport 3780 -ip 127.0.0.7

no_valgrind:
	g++ -g -Wall -pedantic -std=c++11 test.cpp -o ts1
	g++ -g -Wall -pedantic -std=c++11 test_kvstore.cpp -o ts3
	g++ -g -Wall -pedantic -std=c++11 test_keyvalue.cpp -o ts7
	g++ -g -Wall -pedantic -std=c++11 test_map.cpp -o ts8
	./ts1
	./ts3
	./ts7
	./ts8


run:
	docker build -t cs4500:0.1 .
	- rm -rf ./part1/test-array
	- docker run -ti -v `pwd`:/test cs4500:0.1 bash -c "cd /test;make test_serialize;make test_kvstore;"

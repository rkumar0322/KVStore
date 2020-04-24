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
	./ts5 -nodes 3

network2:
	g++ -g -Wall -pthread -pedantic -std=c++11 main.cpp -o ts6
	valgrind valgrind --leak-check=full ./ts6 -nodes 3 -pseudo true -app wc -file 100k.txt

network_local:
	g++ -g -Wall -pthread -pedantic -std=c++11 main.cpp -o ts4
	./ts4 -nodes 3 -pseudo false -app demo -port 3780 -index 0 -masterip 127.0.0.1 -masterport 3780 -ip 127.0.0.1

network_client:
	g++ -g -Wall -pthread -pedantic -std=c++11 main.cpp -o ts4
	./ts4 -nodes 3 -pseudo false -app demo -port 3780 -index 1 -masterip 127.0.0.1 -masterport 3780 -ip 127.0.0.7

no_valgrind:
	g++ -g -Wall -pedantic -std=c++11 test.cpp -o ts1
	g++ -g -Wall -pedantic -std=c++11 test_kvstore.cpp -o ts3
	./ts1
	./ts3


run:
	docker build -t cs4500:0.1 .
	- rm -rf ./part1/test-array
	- docker run -ti -v `pwd`:/test cs4500:0.1 bash -c "cd /test;make network2"

# Introduction 
eau2 system is a distributed key-value store which stores key-value pairs in a give node. A Node is simply ONE instance of the system. in place. Eau2 can have multiple instances runing, and theyh 
Every node in the networks keeps track of its own key value stores, and they are able to get access to other nodes inside the application. 


# Architecture 
## Application 
- The application is the user-level interface which is used for reading, storing, retrieving and writing data. 
- We created 3 subclasses for testing application:  Trivial, Demo, and WordCount. 
- Trivial is for testing writing and storing Dataframe inside application
- Demo is for testing an application running with multiple threads: which we create 3 kinds of nodes: producer, counter and summarizer.
- Reader and Writer is for visiting rows to read and write the text file.
- Wordcount is an example application which prints the list of different words and their counts from a text file.
- We didn't reach m5 so the linus application is not available as well as the real networking stuffs (we do it by threads).
## DataFrame
- The dataframe is designed for storing data in table format to be easily used by users 


## Helpers 
- Has necessary helper classess 
- array.h: class for representing an array of objects 
- serial.h: class for serializing and deserializing objects   
## KDStore
- The KDStore is designed for storing data of every node inside the application.
    - The KDStore is internally represented as a Map. The KVStore has 3 methods overall that are relevant, and all of them take DataFrames as Input. 
    - Get: Get simply retrieves a value. If a key is not on the current node on the system, the key then is sent through to the corresponding node, retrieved, and sent back to the original place. 
    - WaitAndGet: This is like get, but the catch is that waitandget does not end until a value is reached for the key that it is wating on. 
    - Put simply stores the data into the appropriate portions of the store. If the data lives on another node, it simply sends the data to the appropriate node for retrieval. 
- keyvalue.h: class for key and value. 
- map.h: class for storing key-value pairs.
- newmap.h: class for SIMap used for reader.
## network 
- The network is designed for users to communicate on a network layer, by sending different messages.  
- network_ifc.h: network interface
- network_ip.h: basic implementation of network.
   - This part was unfortunately not implemented, altghough the code does live in this repositovy. 
- network_pseudo: network implementation where a thread  represents a singular node which has a KVStore. 
    - The Thread 
- message.h: class for different network messages.
- thread.h: class for self-implemented thread and lock.
    - The Thread class is widely used in order to manage the KDStore. This is also used in the network Implementation. 
    The general format of this includes that idea that there is a producer thread which calls the KV run method. This handles any incoming communication with other nodes in the system, and acts like a secondary user of the data in situations where it needs to provide informaiton back to other nodes. 


# Implementation 
## Dataframe
- The Dataframe class is the representation of sorer data in a table format. It contains a list of Column to keep track of its data, and a schema which defines the structure of data table. Dataframe unable users to get data with a specifc coordinate and create new dataframe using several different methods.
The components of a dataframe indlude a column, a schema and row which are needed to build a dataframe.
    - The Dataframe is represented as an array of columns. 
    - A column is represented as a collection of chunks. A chunk is a simple array wrapper. This representation allows for a distributed way of computing different parts of a dataframe instead of traversing through one dataframe. 
 
## KDStore
- A Key is a String which represents the name key-value pairs under each node.
- A Value is a char* which represents the char array representation of a Dataframe after being serialized.
- A Map is a self-defined class which stores two lists of Object* with the same size. Every key is corresponding to the value with the same index in their lists. 
- A Kdstore stores the key-value pairs by being as a child class of Map. Users are able to access values and insert key-values pairs. 
## Serialize/Deserialize
- Serial.h has our self-defined classes of Serializer and Deserialzer.
- Serializer unable users to serialize any primitive types and their corresponding arrays and store their char* representation into its buffer field. 
- Deserializer unable users to input char* and deserialize it into the actual of its corresponding type.
- Both serializer and deserializer has an index field which keeps track of the current address in the buffer and updates every time when serializing/deserializing.
## Distribution of key-value store
- Our distribution of key-value store can be done with multiple threads.
- In our kdstore.h, we have locks corresponding to get, waitandGet, put methods and a general_lock for the current node which the kdstore belongs to. These locks prevent multiple threads from accessing the same key-value pair at the same time.  


## Use Cases
- To create a DataFrame inside an application, assign it as the value of a key, and insert the keyvalue pair into a KDStore:
    
    size_t SZ = 1000*1000;
    
    double* vals = new double[SZ];
    
    double sum = 0;
    
    KDStore* kv = new KDStore();
    
    for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
    
    String* s = new String("triv");
    
    Key* key = new Key(s, 0);
    
    DataFrame* df = DataFrame::fromArray(key, kv, SZ, vals);
    
## To Run the Application:

In the command line, to run WordCount for the specifications that we were able to use, please type the following: 

make wc


In the command line, to run WordCount for the specifications that we were able to use, please type the following: 

make wc

In the command line, to run Demo for the specifications that we were able to use, please type the following: 

make demo

To call your own application, please use the following monikers: 

```g++ -g -Wall -pthread -pedantic -std=c++11 main.cpp -o eauw```

```./ts6 -nodes 3 -pseudo true -app wc -file 100k.txt```
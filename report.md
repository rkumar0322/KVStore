# Introduction 
eau2 system is a distributed key-value store which stores key-value pairs. Every node in the networks keeps track of its own key value stores, and they are able to get access to other nodes inside the application. 

# Architecture 
## Application 
- The application is the user-level interface which is used for reading, storing, retrieving and writing data. 
- We created 2 subclasses for testing application:  Trivial && Demo 
- Trivial is for testing writing and storing Dataframe inside application
- Demo is for testing an application running with multiple threads: which we create 3 kinds of nodes: producer, counter and summarizer.
- Reader and Writer is for visiting rows to read and write the text file.
- Wordcount is an example application which prints the list of different words and their counts from a text file.
- We didn't reach m5 so the linus application is not available as well as the real networking stuffs (we do it by threads).
## DataFrame
- The dataframe is designed for storing data in table format to be easily used by users 
- contains classess column, schema and row which are needed to build a dataframe.
## Helpers 
- Has necessary helper classess 
- array.h: class for representing an array of objects 
- serial.h: class for serializing and deserializing objects
## KDStore
- The KDStore is designed for storing data of every node inside the application.
- keyvalue.h: class for key and value. 
- map.h: class for storing key-value pairs.
- newmap.h: class for SIMap used for reader.
## network 
- The network is designed for users to communicate on a network layer, by sending different messages.  
- network_ifc.h: network interface
- network_ip.h: basic implementation of network.
- network_pseudo: network implementation containing multiple threads and locks.
- message.h: class for different network messages.
- thread.h: class for self-implemented thread and lock.


# Implementation 
## Dataframe
- The Dataframe class is the representation of sorer data in a table format. It contains a list of Column to keep track of its data, and a schema which defines the structure of data table. Dataframe unable users to get data with a specifc coordinate and create new dataframe using several different methods. 
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


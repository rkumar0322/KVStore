# Introduction 
eau2 system is a distributed key-value store which stores key-value pairs. Every node in the networks keeps track of its own key value stores, and they are able to get access to other nodes inside the application. 

# Architecture 
- The application class is the user-level interface which will used for reading, storing, retrieving and writing data. 

# Implementation 
## Dataframe
- The Dataframe class is the representation of sorer data in a table format. It contains a list of Column to keep track of its data, and a schema which defines the structure of data table. Dataframe unable users to get data with a specifc coordinate and create new dataframe using several different methods. 

## Key-Value store
- A Key is a String which represents the name key-value pairs under each node.
- A Value is a char* which represents the char array representation of a Dataframe after being serialized.
- A Map is a self-defined class which stores a list of Keys and a list of Values with the same size. Every key is corresponding to the value with the same index in their lists. 
- A Kdstore stores the key-value pairs by having a map as its field. Users are able to access values and insert key-values pairs. 

## Serialize/Deserialize
- Serial.h has our self-defined classes of Serializer and Deserialzer.
- Serializer unable users to serialize any primitive types and their corresponding arrays and store their char* representation into its buffer field. 
- Deserializer unable users to input char* and deserialize it into the actual of its corresponding type.
- Both serializer and deserializer has an index field which keeps track of the current address in the buffer and updates every time when serializing/deserializing.

## Use Cases
- To create a DataFrame, assign it as the value of a key, and insert the keyvalue pair into a KDStore:
    
    size_t SZ = 1000*1000;
    
    double* vals = new double[SZ];
    
    double sum = 0;
    
    KDStore* kv = new KDStore();
    
    for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
    
    String* s = new String("triv");
    
    Key* key = new Key(s, 0);
    
    DataFrame* df = DataFrame::fromArray(key, kv, SZ, vals);


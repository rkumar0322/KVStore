#include "helpers/helper.h"
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>


class Args : public Object {
public:
    char* file = nullptr;
    bool pseudo = false;
    size_t num_nodes = 0;
    size_t rows_per_chunk = 2000;
    size_t index = 0;
    size_t port = 0;
    char* master_ip;
    size_t master_port;
    char* app;
    char* ip_addr_str;

    Args() {}

    /**
     * The Arguments to be read in.
     * @param argc the arguments
     * @param argv
     */
    void parse(int argc, char** argv) {
        for (int i = 1; i < argc;i++) {
            char* a = argv[i++];
            assert(i < argc);
            char* n = argv[i];
            printf("%s\n",n);
            printf("%s\n",a);
            if (strcmp(a,"-file") == 0) {
                file = n;
            } else if (strcmp(a,"-pseudo") == 0) {
                pseudo = strcmp(n,"true") == 0;
            } else if (strcmp(a,"-nodes") == 0) {
                num_nodes = atol(n);
            } else if (strcmp(a,"-index") == 0) {
                index = atol(n);
            }else if (strcmp(a,"-port") == 0) {
                port = atol(n);
            } else if (strcmp(a,"-masterip") == 0) {
                master_ip = n;
            } else if (strcmp(a,"-app")==0) {
                app = n;
            } else if (strcmp(a,"-masterport") == 0) {
                master_port = atol(n);
            } else if (strcmp(a,"-rowsperchunk") == 0) {
                rows_per_chunk = atol(n);
            } else if (strcmp(a,"-ip") == 0) {
                ip_addr_str = n;
            }else {
                //FATAL_ERROR("Unknown command line argument: "<<a<<" "<<n);
                printf("GETS HERE SOMEHOW\n");
                exit(1);
            }
        }
    }
};

extern Args arg;
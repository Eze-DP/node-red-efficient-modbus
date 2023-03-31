#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <modbus.h>
#include <unistd.h>

char* lookup_host(const char *host, char* addrstr){
    struct addrinfo hints, *res, *result;
    int errcode;
    void *ptr;

    memset (&hints, 0, sizeof (hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags |= AI_CANONNAME;

    getaddrinfo(host, NULL, &hints, &result);

    res = result;

    while (res){
        inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 100);

        switch (res->ai_family){
            case AF_INET:
                ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
                break;
            case AF_INET6:
                ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
                break;
        }
        inet_ntop(res->ai_family, ptr, addrstr, 100);
        res = res->ai_next;
    }

    freeaddrinfo(result);
    return addrstr;
}

int main(int argc, char *argv[]) {
    modbus_t *mb;

    for(int k = 0; k < (argc-1); k = k + 4){
        int port = atoi(argv[k+2]);
        int start = atoi(argv[k+3]);
        int quantity = atoi(argv[k+4]);
        uint16_t tab_reg[quantity];
        char addrstr[100];
        int rstat;
        int block = 0;

        for(int x = 0; (x * 120) < quantity; x = x + 1){
            if((quantity - (x * 120)) > 120){
                block = 120;  
            } else {
                block = (quantity - (x * 120));
            }

            mb = modbus_new_tcp(lookup_host((char*)argv[k+1], addrstr), port);
            //modbus_set_response_timeout(mb, 2, 0);
            modbus_connect(mb);
            rstat = modbus_read_registers(mb, (start+(120*x)), block, tab_reg);
            modbus_close(mb);
            modbus_free(mb);

            if(rstat!= -1){
                for(int i = 0; i < block; i = i + 1){
                    printf("%d " , tab_reg[i]);
                }
            }
        }
        printf("/");
    }
}


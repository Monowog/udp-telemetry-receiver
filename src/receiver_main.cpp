#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include "udprx/packet.hpp"

#define PORTNUM 8080
#define BACKLOG 10

int main() {
    printf("receiver: starting\n");

    int sockfd;

    //Create the UDP socket

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP socket created with descriptor: %d\n", sockfd);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORTNUM); 
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    char buf[1024];

    struct sockaddr_storage sender_addr;
    socklen_t addr_size = sizeof sender_addr;
    while(true){
        int recv_bytes = recvfrom(sockfd, &buf, sizeof(buf), 0, (struct sockaddr*)&sender_addr, &addr_size);
        if(recv_bytes != -1){
            printf("Just read [%d] bytes from sender\n", recv_bytes);
            std::array<uint8_t, WIRE_SIZE> data;

            memcpy(&data, &buf, sizeof(data));

            Packet packet = deserialize(data);

            printf("Packet information: \n");
            printf("magic: %u\n", packet.magic);
            printf("version: %u\n", packet.version);
            printf("sensor type: %u\n", packet.sensor_type);
            printf("sequence number: %u\n", packet.sequence_number);
            printf("timestamp: %u\n", packet.timestamp);
            printf("values: ");
            for(int i = 0; i <= 2; i++){
                printf("%f [%d], ", packet.values[i], (i+1));
            }
            printf("\nchecksum: %u\n", packet.checksum);

            break;
        }
    }

    close(sockfd);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "udprx/checksum.hpp"
#include "udprx/packet.hpp"

#define PORTNUM 8080

int main() {
    Packet packet = {0}; //example packet
    packet.magic = 0x4226;
    packet.version = 0x01;
    packet.sensor_type = 0x00;
    packet.sequence_number = 0x00000001;
    packet.timestamp = 0x00000283;
    packet.values[0] = 294.007;
    packet.values[1] = 93829.029;
    packet.values[2] = 5589.029;
    packet.checksum = 0x00000000;

    printf("sender: starting\n");

    int sockfd;

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP socket created with descriptor: %d\n", sockfd);

    struct sockaddr_in recv_addr = {0};
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(PORTNUM); 
    inet_pton(AF_INET, "127.0.0.1", &recv_addr.sin_addr);

    std::array<uint8_t, WIRE_SIZE> data = serialize(packet);

    sendto(sockfd, &data, sizeof(data), 0, (struct sockaddr*)&recv_addr, sizeof(recv_addr));

    close(sockfd);

    return 0;
}
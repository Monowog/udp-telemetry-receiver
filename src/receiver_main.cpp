#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <optional>
#include "udprx/packet.hpp"
#include "udprx/checksum.hpp"

#define PORTNUM 8080
#define BACKLOG 10
#define MAGICNUM 16934

std::optional<Packet> try_parse_packet(const std::array<uint8_t, WIRE_SIZE>& data){
    std::optional<Packet> packet = deserialize(data);

    if(packet->magic != MAGICNUM){
        printf("Invalid magic number. Received: [%04x], Correct: [%04x]\n", packet->magic, MAGICNUM);
        return std::nullopt;
    }

    uint32_t checksum = crc32(&data[0], 24);
    if(packet->checksum != checksum){
        printf("Invalid checksum. Received: [%08x], Calculated: [%08x]\n", packet->checksum, checksum);
        return std::nullopt;
    }
    
    printf("Packet parsed successfully\n");
    return packet;
}

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
        if(recv_bytes == WIRE_SIZE){
            printf("Successfully read [%d] bytes from sender\n", recv_bytes);
            std::array<uint8_t, WIRE_SIZE> data;

            memcpy(&data, &buf, sizeof(data)); //copy data into array

            std::optional<Packet> packet = try_parse_packet(data); //turn array into packet

            if(!packet){
                printf("Invalid packet received\n\n");
                continue;
            }

            printf("Packet information: \n");
            printf("magic: %u\n", packet->magic);
            printf("version: %u\n", packet->version);
            printf("sensor type: %u\n", packet->sensor_type);
            printf("sequence number: %u\n", packet->sequence_number);
            printf("timestamp: %u\n", packet->timestamp);
            printf("values: ");
            for(int i = 0; i <= 2; i++){
                printf("%f [%d], ", packet->values[i], (i+1));
            }
            printf("\nchecksum: %08X\n\n", packet->checksum);
        } else if (recv_bytes != -1){
            printf("Read [%d] bytes from sender, rejecting...\n\n", recv_bytes);
        }
    }

    close(sockfd);
    return 0;
}
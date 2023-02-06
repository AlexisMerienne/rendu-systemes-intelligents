#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define HOST_IP "127.0.1.1"
#define PORT_NO 8889
#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUF_SIZE];

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error: socket creation failed");
        exit(1);
    }

    // Configure server address
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT_NO);

    // Convert the host IP from string to binary
    if (inet_pton(AF_INET, HOST_IP, &server_address.sin_addr) <= 0) {
        perror("Error: invalid host IP address");
        exit(1);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("Error: connect to server failed");
        exit(1);
    }

    // Send data to the server
    bzero(buffer, BUF_SIZE);
    strcpy(buffer, "Hello, Server!");
    int n = write(client_socket, buffer, strlen(buffer));
    if (n < 0) {
        perror("Error: write to server failed");
        exit(1);
    }

    // Close the socket
    close(client_socket);
    return 0;
}
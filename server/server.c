#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main() {
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("Couldn't create server socket");
    return -1;
  }
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(7000);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (bind(server_socket, (struct sockaddr*)&serv_addr, sizeof serv_addr) < 0) {
    perror("Couldn't bind");
    return -1;
  }

  listen(server_socket, 0);

  while (1) {
    struct sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&client_len); 

    char buffer[50];
    read(client_socket, buffer, sizeof buffer);

    char welcome[50] = "Welcome to server ";
    char buffer2[150];
    strcpy(buffer2, welcome);
    strcat(buffer2, buffer);
    send(client_socket, buffer2, 150, 0);
  }

  close(server_socket);

  return 0;
}

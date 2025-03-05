#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define MAX_USERNAME_LENGTH 50
#define MAX_MESSAGE_LENGTH 250
#define MAX_USERS_LENGTH 30

int clientSockets[MAX_USERS_LENGTH];

void addClient(int* client_socket) {
  for (int i=0; i < MAX_USERS_LENGTH; i++) {
    if (clientSockets[i] == NULL) {
      clientSockets[i] = *client_socket;
      break;
    }
  }
}

void sendToAllClients(char* message, char* username) {
  char buffer[MAX_MESSAGE_LENGTH + MAX_USERNAME_LENGTH + 5];
  strcpy(buffer, username);
  strcat(buffer, ": ");
  strcat(buffer, message);

  for (int i = 0; i < MAX_USERS_LENGTH; i++) {
    if (clientSockets[i] != NULL) {
      send(clientSockets[i], buffer, sizeof buffer, 0);
      printf("%s\n", buffer);
    }
  }

  memset(buffer, 0, sizeof buffer);
}
 
void* handleClient(void* arg) {
  int client_socket = (int)arg;
  pid_t pid = getpid();
  printf("Created thread of PID: %d\n", (int)pid);
  addClient(&client_socket);

  char username[MAX_USERNAME_LENGTH];
  read(client_socket, username, sizeof username);

  char welcome[50] = "Welcome to server ";
  char buffer2[150];
  strcpy(buffer2, welcome);
  strcat(buffer2, username);
  send(client_socket, buffer2, 150, 0);

  memset(buffer2, 0, sizeof buffer2);
  memset(welcome, 0, sizeof welcome);

  while (1) {
    char message[MAX_MESSAGE_LENGTH];
    read(client_socket, message, sizeof message);
    sendToAllClients(message, username);
    memset(message, 0, sizeof message);
  }
}

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

    pthread_t client_thread;
    pthread_create(&client_thread, NULL, handleClient, (void*)client_socket);
  }

  close(server_socket);

  return 0;
}

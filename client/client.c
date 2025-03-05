#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <error.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#include "ui.h"

#define MAX_USERNAME_LENGTH 50
#define MAX_MESSAGE_LENGTH 250
#define MAX_MESSAGE_NUMBER 250

char messages[MAX_MESSAGE_NUMBER][MAX_MESSAGE_LENGTH];
int messages_length;

void* receive_messages(void* arg) {
  int client_socket = (int)arg;
  char buffer[MAX_MESSAGE_LENGTH + MAX_USERNAME_LENGTH];

  while (1) {
    read(client_socket, buffer, sizeof buffer);

    for (int i=0; i<MAX_MESSAGE_NUMBER; i++) {
      if (messages[i] == NULL) {
        strncpy(messages[i], buffer, sizeof buffer);
        messages_length++;
        break;
      } 
    }

    show_chat_message(messages, messages_length);
    memset(buffer, 0, sizeof buffer);
  }
}

int main() {

  char username[MAX_USERNAME_LENGTH];
  init_username_window(username);

  char ip[100];
  init_server_connect_window(ip);


  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(7000);
  //serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  inet_aton(ip, &serv_addr.sin_addr);

  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket < 0) {
    perror("Couldn't create socket");
    return -1;
  }

  if (connect(client_socket, (struct sockaddr*)&serv_addr, sizeof serv_addr) < 0) {
    perror("Couldn't connect to server");
    return -1;
  }

  send(client_socket, username, sizeof username, 0);

  char message[MAX_MESSAGE_LENGTH];
  init_chat(message);

  while (1) {
    pthread_t receive_messages_thread;
    pthread_create(&receive_messages_thread, NULL, receive_messages, (void*)client_socket);

    char buffer[MAX_MESSAGE_LENGTH] = {0};
    handle_input(buffer, &client_socket);
    send(client_socket, buffer, sizeof buffer, 0);
    memset(buffer, 0, sizeof buffer);
  }

  close(client_socket);

  return 0;
}

#include <curses.h>
#include <string.h>

#define WINDOW_WIDTH 34
#define WINDOW_HEIGHT 7
#define CHAT_PADDING 5

#define MAX_MESSAGES_NUMBER 250
#define MAX_MESSAGE_LENGTH 250
WINDOW *chat; 
WINDOW *input; 

void init_username_window(char* buffer) {
  initscr();
  noecho();
  cbreak();

  int height, width, starty, startx;
  height = WINDOW_HEIGHT;
  width = WINDOW_WIDTH;
  starty = (LINES - height) / 2;
  startx = (COLS - width) / 2;

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);

  WINDOW *win = newwin(height, width, starty, startx);
  wbkgd(win, COLOR_PAIR(1));
  box(win, 0, 0);
  refresh();

  char prompt[] = "Enter your username:";
  mvwprintw(win, 3, (width - strlen(prompt)) / 2, "%s", prompt);
  wrefresh(win);

  echo();
  nocbreak();

  char username[50];
  wattron(win, A_BOLD);
  mvwgetnstr(win, 4, 2, username, 29);
  strncpy(buffer, username, sizeof(username));
  wattroff(win, A_BOLD);


  werase(win);
  delwin(win);
  endwin();
}

void init_server_connect_window(char* server_ip) {
  initscr();
  noecho();
  cbreak();

  int height, width, starty, startx;
  height = WINDOW_HEIGHT;
  width = WINDOW_WIDTH;
  starty = (LINES - height) / 2;
  startx = (COLS - width) / 2;

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);

  WINDOW *win = newwin(height, width, starty, startx);
  wbkgd(win, COLOR_PAIR(1));
  box(win, 0, 0);
  refresh();

  char prompt[] = "Enter Server IP Address:";
  mvwprintw(win, 3, (width - strlen(prompt)) / 2, "%s", prompt);
  wrefresh(win);

  echo();
  nocbreak();

  char ip[100];
  wattron(win, A_BOLD);
  mvwgetnstr(win, 4, 2, ip, 29);
  strncpy(server_ip, ip, sizeof(ip));
  wattroff(win, A_BOLD);

  werase(win);
  delwin(win);
  endwin();
}

void init_chat(char* message) {
  clear();
  refresh();

  initscr();
  noecho();
  cbreak();

  int max_y, max_x;

  getmaxyx(stdscr, max_y, max_y);

  /*
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  */

  int chatHeight, chatWidth, chatStartY, chatStartX;
  chatHeight = LINES - 6;
  chatWidth = COLS - 4;
  chatStartY = 1;
  chatStartX = 2;

  int inputHeight, inputWidth, inputStartY, inputStartX;
  inputHeight = 5;
  inputWidth = COLS - 4;
  inputStartY = chatHeight + 1;
  inputStartX = 2;

  chat = newwin(chatHeight, chatWidth, chatStartY, chatStartX);
  // WINDOW *online = newwin(height, width, starty, startx);
  input = newwin(inputHeight, inputWidth, inputStartY, inputStartX);

  box(chat, 0, 0);
  box(input, 0, 0);

  mvwprintw(chat, 0, 2, "CHAT");
  mvwprintw(input, 0, 2, "INPUT");

  scrollok(chat, TRUE);

  wrefresh(chat);
  wrefresh(input);

  getch();

  /*
  delwin(chat);
  delwin(input);
  endwin();
*/
}

void refresh_chat_windows() {
  wrefresh(chat);
  wrefresh(input);
}

void show_chat_message(char buffers[][250], int buffers_length) {
  wclear(chat);
  box(chat, 0, 0);
  int chatHeight = LINES - 6;
  int line = chatHeight - 1; 
  for (int i=0; i < buffers_length; i++) {
    mvwprintw(chat, 0, 2, "CHAT");
    mvwprintw(chat, line, 1, "%s", buffers[i]);
    wrefresh(chat);
    line--;
  }
  wrefresh(chat);
}

void handle_input(char* buffer, int* client_socket) {
  int input_pos = 0;

  while (1) {
    box(input, 0, 0);

    mvwprintw(input, 0, 2, "INPUT");
    wrefresh(input);

    int ch = wgetch(input);
    if (ch == '\n') {
      input_pos = 0;
      wclear(input);
      break;
    } else if (ch == 127 || ch == KEY_BACKSPACE) {
      if (input_pos > 0) {
        buffer[--input_pos] = '\0';
        wclear(input);
      }
    } else if (input_pos < MAX_MESSAGE_LENGTH - 1) {
      buffer[input_pos++] = ch; 
    }

    mvwprintw(input, 1, 3, "%s", buffer);
    wrefresh(input);
  }
}
  

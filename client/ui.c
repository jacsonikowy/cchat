#include <curses.h>
#include <string.h>

#define WINDOW_WIDTH 34
#define WINDOW_HEIGHT 7
#define CHAT_PADDING 5

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

  WINDOW *chat = newwin(chatHeight, chatWidth, chatStartY, chatStartX);
  // WINDOW *online = newwin(height, width, starty, startx);
  WINDOW *input = newwin(inputHeight, inputWidth, inputStartY, inputStartX);

  box(chat, 0, 0);
  box(input, 0, 0);

  mvwprintw(chat, 0, 2, "CHAT");
  mvwprintw(input, 0, 2, "INPUT");

  scrollok(chat, TRUE);

  wrefresh(chat);
  wrefresh(input);

  getch();

  delwin(chat);
  delwin(input);
  endwin();
  
}

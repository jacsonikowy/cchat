#include <curses.h>
#include <string.h>

#define WINDOW_WIDTH 34
#define WINDOW_HEIGHT 7

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

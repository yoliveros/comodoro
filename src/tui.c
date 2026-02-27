#include "tui.h"
#include "base.h"
#include <asm-generic/ioctls.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#define T_SECONDS 60

struct termios orig_termios;

void tui_print(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  fflush(stdout);
}

void tui_move_cursor(i32 row, i32 col) {
  printf("\033[%d;%dH", row, col);
  fflush(stdout);
}

void tui_clear_screen(void) {
  printf("\033[2J");
  fflush(stdout);
}

void tui_hide_cursor(void) {
  printf("\033[?25l");
  fflush(stdout);
}

void tui_show_cursor(void) {
  printf("\033[?25h");
  fflush(stdout);
}

void tui_disable_input(void) {
  tcgetattr(STDIN_FILENO, &orig_termios);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCIFLUSH, &raw);
}

void tui_enable_input(void) {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

b32 tui_handle_input(void) {
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);

  struct timeval tv = {T_SECONDS, 0};

  if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0) {
    char c;
    read(STDIN_FILENO, &c, 1);
    switch (c) {
    case 'q':
      tui_show_cursor();
      tui_enable_input();
      tui_clear_screen();
      return FALSE;
      break;
    }
  }

  return TRUE;
}

void tui_progress_bar(bar_args args) {
  tui_move_cursor(args.rows - 2, args.cols);

  i32 filled = (i32)(args.width * args.progress);
  if (filled > args.width)
    filled = args.width;

  args.remaining = args.remaining / 60;
  args.duration = args.duration / 60;

  if (args.working)
    tui_print("Working: %dm / %dm\n", args.remaining, args.duration);
  else
    tui_print("Resting: %dm / %dm\n", args.remaining, args.duration);

  for (i32 row = 0; row < 3; row++) {
    tui_move_cursor(args.rows + row, args.cols);
    for (i32 i = 0; i < args.width; i++) {
      printf("%s\u2591\033[0m", FG_MAGENTA);
    }

    tui_move_cursor(args.rows + row, args.cols);
    for (i32 i = 0; i < args.width; i++) {
      // printf("%d, %d\n", filled, args.width);
      if (i < filled) {
        // printf("%s\u2588\033[0m", BG_MAGENTA);
        printf("%s \033[0m", BG_MAGENTA);
      }
    }
  }

  fflush(stdout);
}

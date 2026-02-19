#include "tui.h"
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

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

void tui_handle_input(void) {}

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

  tui_move_cursor(args.rows, args.cols);
  for (i32 i = 0; i < args.width; i++) {
    // printf("%s \033[0m", BG_BAR);
    printf("%s \033[0m", BG_BAR);
    // printf("%s \033[0m", BG_BAR);
  }

  tui_move_cursor(args.rows, args.cols);

  for (i32 i = 0; i < args.width; i++) {
    if (i < filled) {
      printf("%s \033[0m", BG_MAGENTA);
      // printf("%s \033[0m", BG_MAGENTA);
    }
  }

  fflush(stdout);
}

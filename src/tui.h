#pragma once

#include "base.h"

typedef struct {
  i32 rows;
  i32 cols;
  i32 width;
  f32 progress;
  b32 working;
  i32 remaining;
  i32 duration;
} bar_args;

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define ITALIC "\033[4m"

#define FG_DEFAULT "\033[39m"
#define FG_BLACK "\033[30m"
#define FG_RED "\033[31m"
#define FG_GREEN "\033[32m"
#define FG_MAGENTA "\033[35m"
#define FG_CYAN "\033[36m"
#define FG_WHITE "\033[37m"

#define BG_DEFAULT "\033[49m"
#define BG_BAR "\033[100m"
#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_MAGENTA "\033[45m"
#define BG_WHITE "\033[47m"

void tui_print(const char *fmt, ...);
void tui_move_cursor(i32 row, i32 col);
void tui_clear_screen(void);
void tui_hide_cursor(void);
void tui_show_cursor(void);
void tui_handle_input(void);
void tui_progress_bar(bar_args args);

#include "base.h"
#include "tui.h"
#include <locale.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#define T_SECONDS 60

volatile sig_atomic_t resized = 0;

void handle_resize(i32 sig) { resized = 1; }

int main(void) {
  setlocale(LC_ALL, "");
  tui_disable_input();
  i32 work_min = 50 * 60;
  i32 rest_min = 10 * 60;
  i32 duration = 0;

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  i32 cols = w.ws_col;
  i32 rows = w.ws_row;
  i32 bar_width = cols - 50;
  i32 padding_x = (cols - bar_width) / 2;

  tui_hide_cursor();
  tui_clear_screen();

  b32 working = TRUE;

  signal(SIGWINCH, handle_resize);

  while (1) {
    if (working)
      duration = work_min;
    else
      duration = rest_min;

    time_t start = time(NULL);
    time_t end = start + duration;

    for (time_t now = time(NULL); now < end; now = time(NULL)) {
      b32 check_input = tui_handle_input();
      if (!check_input)
        return 0;

      f32 progress = (f32)(now - start) / (f32)duration;
      i32 remaining = (i32)(end - now);

      bar_args args = {
          .rows = rows / 2,
          .cols = padding_x,
          .width = bar_width,
          .progress = progress,
          .working = working,
          .remaining = remaining,
          .duration = duration,
      };

      tui_progress_bar(args);

      sleep(T_SECONDS);
      if (resized) {
        ioctl(STDIN_FILENO, TIOCGWINSZ, &w);
        cols = w.ws_col;
        rows = w.ws_row;
        bar_width = cols - 50;
        padding_x = (cols - bar_width) / 2;
        resized = 0;
        tui_clear_screen();
      }
    }

    working = !working;
  }

  tui_show_cursor();
  tui_enable_input();
  return 0;
}

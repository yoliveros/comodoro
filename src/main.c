#include "base.h"
#include "tui.h"
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#define T_SECONDS 60

int main(void) {
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

  // time_t now = time(NULL);

  b32 working = TRUE;

  while (1) {
    if (working)
      duration = work_min;
    else
      duration = rest_min;

    time_t start = time(NULL);
    time_t end = start + duration;

    for (time_t now = time(NULL); now <= end; now = time(NULL)) {
      f32 progress = (f32)(now - start) / (f32)duration;
      i32 remaining = (i32)(end - now);

      // tui_move_cursor(rows / 2 - 2, padding_x / 2 + 50);
      bar_args args = {
          .rows = rows / 2,
          .cols = padding_x,
          .width = bar_width,
          .progress = progress,
          .working = working,
          .remaining = remaining,
          .duration = duration
      };

      tui_progress_bar(args);

      // tui_print("\n");

      sleep(60);
    }

    working = !working;
  }

  // tui_move_cursor(6, 1);
  // tui_print("Done!\n");

  tui_show_cursor();
  return 0;
}

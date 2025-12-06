#ifndef TUI_H
#define TUI_H

#include "monitor.h"

// TUI 초기화 및 종료
void init_tui();
void close_tui();

// 화면 출력
void draw_header(float cpu, long used, long total);
void draw_process_list(ProcessInfo *plist, int count, int start_index, int selected);

#endif

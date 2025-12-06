#include "tui.h"
#include <ncurses.h>   // 화면 출력, 색상, 커서 제어

void init_tui() {
    initscr();      // ncurses 화면 
    noecho();       // 입력한 글자 화면에 표시하지 않는 기능
    cbreak();       // 입력 즉시 처리
    curs_set(0);    // 커서 숨김
    keypad(stdscr, TRUE);  // 방향키 사용 
}

/* ncurses 종료 */
void close_tui() {
    endwin();
}

/* 화면 상단 CPU/메모리 표시 */
void draw_header(float cpu, long used, long total) {
    mvprintw(0, 0, "============================================================");
    mvprintw(1, 0, "                      My-Top System Monitor                 ");
    mvprintw(2, 0, "============================================================");
    mvprintw(3, 0, " CPU: %.2f %%    MEM: %ld MB / %ld MB (%.2f %%)",
             cpu, used, total, (used * 100.0) / total);
    mvprintw(4, 0, "------------------------------------------------------------");
}

/* 프로세스 목록 */
void draw_process_list(ProcessInfo *plist, int count, int start_index, int selected) {

    mvprintw(5, 0, " PID       CPU       MEM(KB)     STATE     NAME ");
    mvprintw(6, 0, "------------------------------------------------------------");

    int max_display = 15;

    for (int i = 0; i < max_display && (start_index + i) < count; i++) {
        int idx = start_index + i;

        if (i == selected) {
            attron(A_REVERSE); // 선택 강조
        }

        mvprintw(7 + i, 0,
                 "%-8d %-8lu %-10ld %-8c %-20s",
                 plist[idx].pid,
                 plist[idx].utime + plist[idx].stime, // CPU 시간 합
                 plist[idx].rss * 4, 
                 plist[idx].state,
                 plist[idx].name);

        if (i == selected) {
            attroff(A_REVERSE);
        }
    }

    mvprintw(23, 0,
        " ↑/↓:스크롤  ←/→:선택  F1/F2/F3:정렬  k:KILL s:STOP c:CONT q:종료 ");
}


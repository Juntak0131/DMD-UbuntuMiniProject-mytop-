#include "monitor.h"
#include "tui.h"
#include <unistd.h> // sleep()
#include <signal.h> // kill(), SIGSTOP, SIGKILL
#include <stdlib.h>
#include <ncurses.h> // 키 입력 처리

/* PID 정렬 */
int compare_pid(const void *a, const void *b) {
    return ((ProcessInfo*)a)->pid - ((ProcessInfo*)b)->pid;
}

/* CPU 정렬 */
int compare_cpu(const void *a, const void *b) {
    long aa = ((ProcessInfo*)a)->utime + ((ProcessInfo*)a)->stime;
    long bb = ((ProcessInfo*)b)->utime + ((ProcessInfo*)b)->stime;
    return bb - aa;
}

/* 메모리 정렬 */
int compare_mem(const void *a, const void *b) {
    return ((ProcessInfo*)b)->rss - ((ProcessInfo*)a)->rss;
}

int main() {
    init_tui();

    int start = 0;
    int selected = 0;
    int sort_mode = 0;

    while (1) {

        float cpu = get_cpu_usage();
        long total, used;
        get_memory_info(&total, &used);

        ProcessInfo plist[300];
        int count = get_process_list(plist, 300);

        if (sort_mode == 0) qsort(plist, count, sizeof(ProcessInfo), compare_pid);
        if (sort_mode == 1) qsort(plist, count, sizeof(ProcessInfo), compare_cpu);
        if (sort_mode == 2) qsort(plist, count, sizeof(ProcessInfo), compare_mem);

        clear();
        draw_header(cpu, used, total);
        draw_process_list(plist, count, start, selected);
        refresh();

        timeout(0);
        int ch = getch();

        if (ch == 'q') break;
				/* 스크롤 */
        if (ch == KEY_DOWN && start < count - 1) start++;
        if (ch == KEY_UP && start > 0) start--;

				/* 선택 이동 */
        if (ch == KEY_RIGHT && selected < 14) selected++;
        if (ch == KEY_LEFT && selected > 0) selected--;

				/* 정렬 */
        if (ch == KEY_F(1)) sort_mode = 0;
        if (ch == KEY_F(2)) sort_mode = 1;
        if (ch == KEY_F(3)) sort_mode = 2;

				/* 프로세스 제어 */
        int idx = start + selected;
        if (idx < count) {
            int pid = plist[idx].pid;

            if (ch == 'k') kill(pid, SIGKILL);
            if (ch == 's') kill(pid, SIGSTOP);
            if (ch == 'c') kill(pid, SIGCONT);
        }

        sleep(1);
    }

    close_tui();
    return 0;
}


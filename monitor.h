#ifndef MONITOR_H
#define MONITOR_H

/* 프로세스 정보 저장용 구조체 */
typedef struct {
    int pid;                // 프로세스 ID
    char name[256];         // 프로세스 이름
    char state;             // 실행 상태 
    unsigned long utime;    // 사용자 모드 CPU 시간
    unsigned long stime;    // 커널 모드 CPU 시간
    long rss;               // 메모리 사용량
} ProcessInfo;

/* CPU 사용률 */
float get_cpu_usage();

/* 메모리 정보 */
void get_memory_info(long *total, long *used);

/* 프로세스 목록 */
int get_process_list(ProcessInfo *plist, int max_count);

#endif


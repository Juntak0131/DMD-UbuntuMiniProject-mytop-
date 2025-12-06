#include "monitor.h"
#include <stdio.h> // fopen(), fscanf(), fclose()
#include <stdlib.h>  
#include <string.h> // strcmp, strlen 문자열 처리 코드
#include <dirent.h> // opendir(), readdir()
#include <unistd.h> // 시스템 함수( ex : getpid() )

float get_cpu_usage() {
    FILE *fp;
    static unsigned long long prev_total = 0, prev_idle = 0;

    unsigned long long user,nice,system,idle,iowait,irq,softirq,steal;
    unsigned long long total;

    fp = fopen("/proc/stat", "r");
    if (!fp) return 0.0;

    int ret = fscanf(fp,
        "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
        &user,&nice,&system,&idle,&iowait,&irq,&softirq,&steal
    );
    fclose(fp);

    if (ret != 8) return 0.0;

    total = user + nice + system + idle + iowait + irq + softirq + steal;

    if (prev_total == 0) {
        prev_total = total;
        prev_idle  = idle;
        return 0.0;
    }

    float usage =
        (float)((total - prev_total) - (idle - prev_idle)) /
        (total - prev_total) * 100.0f;

    prev_total = total;
    prev_idle  = idle;

    return usage;
}

/* 메모리 정보 */
void get_memory_info(long *total, long *used) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return;

    char key[64];
    long mem_total = 0, mem_available = 0;

    while (fscanf(fp, "%63s", key) != EOF) {

        if (!strcmp(key, "MemTotal:")) {
            if (fscanf(fp, "%ld", &mem_total) != 1) break;
        }
        else if (!strcmp(key, "MemAvailable:")) {
            if (fscanf(fp, "%ld", &mem_available) != 1) break;
        }

        if (mem_total && mem_available) break;
    }

    fclose(fp);

    *total = mem_total / 1024;            // KB → MB
    *used  = (mem_total - mem_available) / 1024;
}

/* 프로세스 목록 */
int get_process_list(ProcessInfo *plist, int max_count) {

    DIR *dp = opendir("/proc");
    if (!dp) return 0;

    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(dp)) != NULL && count < max_count) {
        if (entry->d_name[0] < '0' || entry->d_name[0] > '9')
            continue;

        char stat_path[512], statm_path[512];

        snprintf(stat_path, sizeof(stat_path), "/proc/%s/stat", entry->d_name);
        snprintf(statm_path, sizeof(statm_path), "/proc/%s/statm", entry->d_name);

        FILE *fp = fopen(stat_path, "r");
        if (!fp) continue;

        ProcessInfo p;

        int ret = fscanf(fp,
            "%d (%255[^)]) %c "
            "%*d %*d %*d %*d %*d %*d %*d %*d "
            "%lu %lu",
            &p.pid, p.name, &p.state,
            &p.utime, &p.stime
        );
        fclose(fp);

        if (ret != 5) continue;

        FILE *fm = fopen(statm_path, "r");
        if (fm) {
            long skip;
            if (fscanf(fm, "%ld %ld", &skip, &p.rss) != 2)
                p.rss = 0;
            fclose(fm);
        }

        plist[count++] = p;
    }

    closedir(dp);
    return count;
}


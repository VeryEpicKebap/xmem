#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMINFO_FILE "/proc/meminfo"

long parse(const char *line, const char *key) {
    if (strncmp(line, key, strlen(key)) == 0) {
        long value;
        sscanf(line + strlen(key), " %ld", &value);
        return value;
    }
    return -1;
}

int main() {
    FILE *file = fopen(MEMINFO_FILE, "r");
    if (!file) {
        perror("failed to open /proc/meminfo");
        return 1;
    }
    char line[256];
    long mem_total = -1;
    long mem_available = -1;
    while (fgets(line, sizeof(line), file)) {
        if (mem_total == -1)
            mem_total = parse_meminfo_line(line, "MemTotal:");
        else if (mem_available == -1)
            mem_available = parse_meminfo_line(line, "MemAvailable:");      
        if (mem_total != -1 && mem_available != -1)
            break;
    }
    fclose(file);
    if (mem_total == -1 || mem_available == -1) {
        fprintf(stderr, "failed to read MemTotal or MemAvailable\n");
        return 1;
    }
    long used_kb = mem_total - mem_available;
    printf("total: %ld MB | used: %ld MB | free: %ld MB\n",
           mem_total / 1024, used_kb / 1024, mem_available / 1024);

    return 0;
}

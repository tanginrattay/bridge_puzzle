#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include "optimize.h"
#include "backtracking.h"

int map[51][51][6];
int trigger = 0;

void printmap(int n, int m) {
    printf("##################################\n");
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (map[i][j][1]) {
                printf("| ");
            } else {
                printf("  ");
            }
        }
        putchar('\n');
        for (int j = 1; j <= m; j++) {
            if (map[i][j][0] > 0) {
                printf("%d", map[i][j][0]);
            } else {
                printf(" ", map[i][j][0]);
            }
            if (map[i][j][2]) {
                printf("-");
            } else {
                printf(" ");
            }
        }
        putchar('\n');
    }
    printf("##################################\n");
}

int main () {
    int W;
    if (scanf("%d", &W) != 1) {
        fprintf(stderr, "请输入重复次数 W\n");
        return 1;
    }
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) {
        fprintf(stderr, "输入错误，请输入 n m\n");
        return 1;
    }

    int original_map[51][51][6];
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int tmp;
            scanf("%d", &tmp);
            original_map[i][j][0] = original_map[i][j][5] = tmp;
            // keep other fields zeroed
            original_map[i][j][1] = original_map[i][j][2] = original_map[i][j][3] = original_map[i][j][4] = 0;
        }
    }

    // print initial map once (optional)
    // copy original -> map for initial display
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            for (int k = 0; k < 6; k++)
                map[i][j][k] = original_map[i][j][k];
    printmap(n, m);

    // allocate reusable list buffer (max cells 51*51)
    int max_cells = 51 * 51;
    int **list = (int **)malloc(max_cells * sizeof(int *));
    for (int i = 0; i < max_cells; i++) {
        list[i] = (int *)malloc(2 * sizeof(int));
    }

    // start high-resolution timing for W runs
#ifdef _WIN32
    LARGE_INTEGER t_start, t_end, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t_start);
#else
    struct timeval t_start, t_end;
    gettimeofday(&t_start, NULL);
#endif

    for (int run = 0; run < W; run++) {
        // restore map from original
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                for (int k = 0; k < 6; k++) {
                    map[i][j][k] = original_map[i][j][k];
                }
            }
        }
        trigger = 0;

        // run optimization and backtracking as in single run
        optimize(n, m);

        int list_size = 0;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (map[i][j][5] > 0) {
                    list[list_size][0] = i;
                    list[list_size][1] = j;
                    list_size++;
                }
            }
        }

        back_tracking(n, m, list, list_size, 0);
        // if trigger indicates early solution, still continue runs to accumulate timing as requested
    }

    // stop timing and compute elapsed ms
#ifdef _WIN32
    QueryPerformanceCounter(&t_end);
    double elapsed_ms_d = (double)(t_end.QuadPart - t_start.QuadPart) * 1000.0 / (double)freq.QuadPart;
#else
    gettimeofday(&t_end, NULL);
    long sec = t_end.tv_sec - t_start.tv_sec;
    long usec = t_end.tv_usec - t_start.tv_usec;
    long long elapsed_ms_ll = sec * 1000LL + usec / 1000LL;
    double elapsed_ms_d = (double)elapsed_ms_ll;
#endif

    long long total_ms = (long long)(elapsed_ms_d + 0.5);
    int hours = (int)(total_ms / 3600000LL);
    int minutes = (int)((total_ms % 3600000LL) / 60000LL);
    int seconds = (int)((total_ms % 60000LL) / 1000LL);
    int milliseconds = (int)(total_ms % 1000LL);

    // print final map (result from last run)
    printmap(n, m);

    printf("用时:%02d:%02d:%02d.%03d\n", hours, minutes, seconds, milliseconds);

    // free list
    for (int i = 0; i < max_cells; i++) free(list[i]);
    free(list);

    return 0;
}
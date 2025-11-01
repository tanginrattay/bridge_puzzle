#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
    int n, m;
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int tmp;
            scanf("%d", &tmp);
            map[i][j][0] = map[i][j][5] = tmp;
        }
    } 

    // start timing after input is read
    time_t t_start = time(NULL);

    printmap(n, m);
    
    optimize(n, m);

    printmap(n, m);

    int **list = (int **)malloc(2601 * sizeof(int *));
    for (int i = 0; i < 2601; i++) {
        list[i] = (int *)malloc(2 * sizeof(int));
    }
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

    printmap(n, m);

    // stop timing and print elapsed time (hh:mm:ss)
    time_t t_end = time(NULL);
    int elapsed = (int)difftime(t_end, t_start);
    int hours = elapsed / 3600;
    int minutes = (elapsed % 3600) / 60;
    int seconds = elapsed % 60;
    printf("time:%02d:%02d:%02d\n", hours, minutes, seconds);

    getchar();
    getchar();
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
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
            if (map[i][j][5] == 1) {
                list[list_size][0] = i;
                list[list_size][1] = j;
                list_size++;
            }
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (map[i][j][5] == 2) {
                list[list_size][0] = i;
                list[list_size][1] = j;
                list_size++;
            }
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (map[i][j][5] == 3) {
                list[list_size][0] = i;
                list[list_size][1] = j;
                list_size++;
            }
        }
    }

    back_tracking(n, m, list, list_size, 0);

    printmap(n, m);

    getchar();
    getchar();
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "optimize.h"
#include "backtracking.h"

int map[50][50][6];

void printmap(int n, int m) {
    printf("##################################\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (map[i][j][1]) {
                printf("| ");
            } else {
                printf("  ");
            }
        }
        putchar('\n');
        for (int j = 0; j < m; j++) {
            if (map[i][j][0]) {
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
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int tmp;
            scanf("%d", &tmp);
            map[i][j][0] = map[i][j][5] = tmp;
        }
    } 

    printmap(n, m);
    
    optimize(n, m);

    printmap(n, m);

    // int **list = (int **)malloc(2500 * sizeof(int *));
    // for (int i = 0; i < 2500; i++) {
    //     list[i] = (int *)malloc(2 * sizeof(int));
    // }
    // int list_size = 0;
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < m; j++) {
    //         if (map[i][j][5] > 0) {
    //             list[list_size][0] = i;
    //             list[list_size][1] = j;
    //             list_size++;
    //         }
    //     }
    // }

    // int trigger = 0;

    // trigger = back_tracking(n, m, list, list_size);

    // printmap(n, m);

    getchar();
    getchar();
    return 0;
}
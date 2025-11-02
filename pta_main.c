#include <stdio.h>
#include <stdlib.h>
#include "optimize.h"
#include "backtracking.h"

int map[51][51][6];
int trigger = 0;

void print_solution(int n, int m) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (map[i][j][0] > 0) {
                printf("%d %d %d %d %d %d\n", 
                    i, j,
                    map[i][j][1], // up
                    map[i][j][3], // down
                    map[i][j][4], // left
                    map[i][j][2]  // right
                );
            }
        }
    }
}

int main () {
    int n, m;
    scanf("%d %d", &n, &m);

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int tmp;
            scanf("%d", &tmp);
            map[i][j][0] = map[i][j][5] = tmp;
            map[i][j][1] = map[i][j][2] = map[i][j][3] = map[i][j][4] = 0;
        }
    }

    optimize(n, m);

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
    if (trigger) {
        print_solution(n, m);
    } else {
        printf("No solution\n");
    }
    for (int i = 0; i < 2601; i++) free(list[i]);
    free(list);

    return 0;
}
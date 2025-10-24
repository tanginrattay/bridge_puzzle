#include <stdio.h>
#include <stdlib.h>
#include "optimize.h"
#include "backtracking.h"

int map[50][50][6];

void printmap(int n, int m) {
    printf("##################################");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (map[i][j][3]) {
                printf("| ");
            } else {
                printf("  ");
            }
        }
        putchar('\n');
        for (int j = 0; j < m; j++) {
            if (map[i][j][2]) {
                printf("%d-", map[i][j][0]);
            } else {
                printf("%d ", map[i][j][0]);
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
    
    find_determined_by_path_search(n, m);

    printmap(n, m);
    getchar();
    getchar();
    return 0;
}
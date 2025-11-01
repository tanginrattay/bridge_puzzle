#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> // 引入 bool 类型

int main() {
    int n, m;

    while(1) {
        if (scanf("%d %d", &n, &m) != 2) {
            fprintf(stderr, "输入错误，请输入两个整数 n 和 m\n");
            return 1;
        }

        // Check for exit condition
        if (n == 0 && m == 0) {
            break;
        }

        if (n <= 1 || n > 50 || m <= 1 || m > 50) {
            fprintf(stderr, "n 和 m 必须大于 1 且小于等于 50\n");
            continue;  // Skip to next iteration instead of exiting
        }

        srand(time(NULL));

        // 1. 创建内部的 "解决方案" 布局
        // horizontal[i][j] 表示 (i,j) 和 (i, j+1) 之间是否有连接
        bool horizontal[n][m - 1];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m - 1; j++) {
                horizontal[i][j] = (rand() % 2 == 1);
            }
        }

        // vertical[i][j] 表示 (i,j) 和 (i+1, j) 之间是否有连接
        bool vertical[n - 1][m];
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < m; j++) {
                vertical[i][j] = (rand() % 2 == 1);
            }
        }

        // 2. 准备输出的 "题目" 矩阵
        int problem_grid[n][m];

        // 3. 遍历所有节点，计算度数并决定是否 "擦除"
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                
                bool has_left = (j > 0) && horizontal[i][j - 1];
                bool has_right = (j < m - 1) && horizontal[i][j];
                bool has_up = (i > 0) && vertical[i - 1][j];
                bool has_down = (i < n - 1) && vertical[i][j];

                int degree = (int)has_left + (int)has_right + (int)has_up + (int)has_down;

                // 检查是否为 "直通" 节点
                bool is_horizontal_passthrough = (degree == 2 && has_left && has_right && !has_up && !has_down);
                bool is_vertical_passthrough = (degree == 2 && !has_left && !has_right && has_up && has_down);

                if (is_horizontal_passthrough || is_vertical_passthrough) {
                    // 这是一个 "直通" 节点
                    // 我们可以随机将其 "擦除" (变为0)
                    // 设置一个概率，例如 70% 的概率变为 0
                    if (rand() % 10 < 7) { 
                        problem_grid[i][j] = 0;
                    } else {
                        problem_grid[i][j] = 2; // 保留为连接器
                    }
                } else {
                    // 这是 0, 1, 3, 4 度节点，或者是 2 度的 "拐角"
                    // 它们必须是连接器 (或 0 度)
                    problem_grid[i][j] = degree;
                }
            }
        }

        // 4. 打印输出
        printf("%d %d\n", n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                printf("%d", problem_grid[i][j]);
                if (j < m - 1) {
                    printf(" ");
                }
            }
            printf("\n");
        }
    }

    return 0;
}
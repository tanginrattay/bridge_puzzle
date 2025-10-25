#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


void generate_test_case(int n, int m, int density) {
    // 设置随机种子
    srand(time(NULL));
    
    printf("%d %d\n", n, m);
    
    // 创建网格
    int grid[n][m];
    
    // 初始化网格，大部分为0（无连接器）
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // 根据密度决定是否放置连接器
            if (rand() % 100 < density) {
                // 放置1-4度的连接器
                grid[i][j] = (rand() % 4) + 1;
            } else {
                grid[i][j] = 0;
            }
        }
    }
    
    // 确保至少有一些连接器
    int connector_count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] > 0) connector_count++;
        }
    }
    
    // 如果没有连接器，随机放置几个
    if (connector_count == 0) {
        for (int k = 0; k < 3; k++) {
            int i = rand() % n;
            int j = rand() % m;
            grid[i][j] = (rand() % 4) + 1;
        }
    }
    
    // 输出网格
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d", grid[i][j]);
            if (j < m-1) printf(" ");
        }
        printf("\n");
    }
}



int main() {
    generate_test_case(50, 50, 30); // 50x50网格，30%密度
    return 0;
}
// 使用示例

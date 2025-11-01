//确定类型：
//1：往1个方向试探，4种情况
//2：往2个方向试探，6种情况
//3：往3个方向试探，4种情况
//剪枝1.遇到交叉线
//剪枝2.没有目标接口
//0表示原来状态，5表示还剩下的接口数
//find_up/down/left/right用于寻找下一个连接器,并且返回其坐标，同时将路途的点设置为-1，表示不可通行


//问题1：0行0列开始
//问题2：map[i][j][0]=-1

int find_down(int n,int m,int x,int y){
    for(int i = x+1;i<=n;i++){
        if(map[i][y][0]>0){
            if(map[i][y][5] == 0){
                return 0;
            }
            for(int k=x+1;k<i;k++){
                map[k][y][1] = 1;
                map[k][y][3] = 1; 
            }
            return i;
        }
        else if(map[i][y][0] == -1){
            return 0;
        }
        else{
            map[i][y][0] = -1;
        }
    }
    return 0;
};

int find_right(int n,int m,int x,int y){
    for(int j = y+1;j<=m;j++){
        if(map[x][j][0]>0){
            if(map[x][j][5] == 0){
                return 0;
            }
            for(int k=y+1;k<j;k++){
                map[x][k][2] = 1;
                map[x][k][4] = 1; 
            }
            return j;
        }
        else if(map[x][j][0] == -1){
            return 0;
        }
        else{
            map[x][j][0] = -1;
        }
    }
    return 0;
};
//注意还有清除-1标记的函数
void clear_right(int n,int m,int x,int y){
    for(int j = y+1;j<=m;j++){
        if(map[x][j][0]>0){
            return;
        }
        else if(map[x][j][0] == -1){
            map[x][j][0] = 0;
            map[x][j][2] = 0;
            map[x][j][4] = 0;
        }
        else{
            return;
        }
    }
    return;

};
void clear_down(int n,int m,int x,int y){
    for(int i = x+1;i<=n;i++){
        if(map[i][y][0]>0){
            return;
        }
        else if(map[i][y][0] == -1){
            map[i][y][0] = 0;
            map[i][y][1] = 0;
            map[i][y][3] = 0;
        }
        else{
            return;
        }
    }
    return;

};
extern int trigger;
 void back_tracking(int n,int m,int** c,int c_size,int index){
        //终点，num==c_size
        if(index == c_size){
                trigger = 1;
                return;
        }
        //非终点就要判断是否存在接口
        int x = c[index][0];
        int y = c[index][1];
        if(map[x][y][5] == 0){
            back_tracking(n,m,c,c_size,index+1);
            return;
        }
        //若存在接口，则进行试探
        else{
            int rem = map[x][y][5];
            //优先处理剩余接口数为3的节点，强力剪枝
            if(rem == 3){
                return;
            }
            //剩余接口数为2或1的节点
            //剩余2个接口
            else if(rem == 2){
                int right = find_right(n,m,x,y);
                int down = find_down(n,m,x,y);
                if(right && down){
                    map[x][y][2] = 1;
                    map[x][right][4] = 1;
                    map[x][y][3] = 1;
                    map[down][y][1] = 1;
                    map[x][y][5] -= 2;
                    map[x][right][5]--;
                    map[down][y][5]--;
                    back_tracking(n,m,c,c_size,index+1);
                    if(trigger == 1) return;
                    map[x][y][2] = 0;
                    map[x][right][4] = 0;
                    map[x][y][3] = 0;
                    map[down][y][1] = 0;
                    map[x][y][5] += 2;
                    map[x][right][5]++;
                    map[down][y][5]++;
                }
                //清除标记
                    clear_right(n,m,x,y);
                    clear_down(n,m,x,y);
            }
            //剩余1个接口
            else{
                int right = find_right(n,m,x,y);
                if(right){
                    map[x][y][2] = 1;
                    map[x][right][4] = 1;
                    map[x][y][5]--;
                    map[x][right][5]--;
                    back_tracking(n,m,c,c_size,index+1);
                    if(trigger == 1) return;
                    map[x][y][2] = 0;
                    map[x][right][4] = 0;
                    map[x][y][5]++;
                    map[x][right][5]++; 
                }
                //清除标记
                    clear_right(n,m,x,y);
                int down = find_down(n,m,x,y);
                if(down){
                    map[x][y][3] = 1;
                    map[down][y][1] = 1;
                    map[x][y][5]--;
                    map[down][y][5]--;
                    back_tracking(n,m,c,c_size,index+1);
                    if(trigger == 1) return;
                    map[x][y][3] = 0;
                    map[down][y][1] = 0;
                    map[x][y][5]++;
                    map[down][y][5]++;
                }
                //清除标记
                    clear_down(n,m,x,y);
            }
            return;   
        }
 }
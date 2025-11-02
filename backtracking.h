// Determine types:
// 1: probe in 1 direction, 4 cases
// 2: probe in 2 directions, 6 cases
// 3: probe in 3 directions, 4 cases
// Pruning 1: encounter crossing lines
// Pruning 2: no target interface
// 0 indicates original state, 5 indicates remaining interface count
// find_up/down/left/right are used to find the next connector, return its coordinates, and set the path points to -1 to indicate impassable


// Issue 1: starts at row 0 column 0
// Issue 2: map[i][j][0] = -1
int find_down(int n,int m,int x,int y){
    for(int i = x+1;i<=n;i++){
        if(map[i][y][0]>0){
            if(map[i][y][5] == 0){
                return 0;
            }
            for(int k=x+1;k<i;k++){
                map[k][y][1] = 1;
                map[k][y][3] = 1; 
                map[k][y][0] = -1;   
            }
            return i;
        }
        else if(map[i][y][0] == -1){
            return 0;
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
                map[x][k][0] = -1;
            }
            return j;
        }
        else if(map[x][j][0] == -1){
            return 0;
        }
    }
    return 0;
};
// Note there are also functions to clear -1 marks
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
        // Terminal: index == c_size
        if(index == c_size){
                trigger = 1;
                return;
        }
        // If not terminal, check whether interfaces exist
        int x = c[index][0];
        int y = c[index][1];
        if(map[x][y][5] == 0){
            back_tracking(n,m,c,c_size,index+1);
            return;
        }
        // If interfaces exist, perform probing
        else{
            int rem = map[x][y][5];
            // Prioritize nodes with 3 remaining interfaces, strong pruning
            if(rem == 3){
                return;
            }
            // Nodes with 2 or 1 remaining interfaces
            // Remaining 2 interfaces
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
                 // Clear marks
                    clear_right(n,m,x,y);
                    clear_down(n,m,x,y);
            }
            // Remaining 1 interface
            else if(rem == 1){
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
                    // Clear marks
                    
                }
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
                    // Clear marks
                    
                }
                clear_down(n,m,x,y);
            }
            return;   
        }
 }
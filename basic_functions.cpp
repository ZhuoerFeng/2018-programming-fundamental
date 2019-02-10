#include "point.h"
#include "loadbmp.h"
#include <windows.h>

vector<char> record_solution;

// BMP类，主要支持图像操作
class BMP {
public:
    int w, h;
    unsigned char* pixels;

    BMP(int _w = 0, int _h = 0) {
        w = _w; h = _h;
        pixels = (unsigned char*)malloc(w * h * 3);
        memset(pixels, 0, w * h * 3);
    }

    BMP(const char* filename) {
        bmpDecode(filename, &pixels, &w, &h, 3);
    }

    ~BMP() {
        free(pixels);
    }

    void save(const char* filename) {
        bmpEncode(filename, pixels, w, h, 3);
    }

    unsigned char* pixel(int i, int j) {
        return pixels + (i * w + j) * 3;
    }
};

int k = 0;

// 函数功能：交换
void mySwap(int* a, int* b)
{
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}

// 函数功能：读入一个二维数组，将其作为当前状态
Point* readMatrix(int a[][100], Point* size)
{
    printf("read begins.\n");
    int m = size->x;
    int n = size->y;
    printf("the point number is (%d, %d)", m, n);
    for (int i = 0; i < m; i ++)   
        for (int j = 0; j < n; j ++)
            scanf("%d", &a[i][j]);
    printf("read ends.\n");
    return size;
}

// 函数功能：打印当前状态，并且判断是否复原
Point* printMatrix(int a[][100], Point* size)
{
    system("cls");
    printf("=========================================================================================\n");
    printf("=========================================================================================\n");
    int m = size->x;
    int n = size->y;
    for (int i = 0; i < m; i ++)
    {
        for (int j = 0; j < n; j ++)
            printf("%d ", a[i][j]); 
        printf("\n");       
    }   

//    printf("print ends.\n");
    return size;
}

// 随机生成可解的状态，有可能不可解
// generate a matrix based on random numbers, with a -1 space in the end
int generateRandomMatrix(int a[][100], Point* size)
{
    int m = size->x;
    int n = size->y;
    int sum = m * n; // total numbers to generate

    srand(unsigned(std::time(0)));
    vector<int> temp;
    for (int i = 0; i < sum - 1; i ++)
        temp.push_back(i);
    random_shuffle(temp.begin(), temp.end() - 1); // generate random numbers

    int reverse = 0;
    //O(n^2)
    for (int i = 0; i < sum - 1; i ++)
        for (int j = 0; j < i; j ++)
            if (temp[j] > temp[i])
                reverse ++;

    for (int i = 0; i < m; i ++)
        for (int j = 0; j < n ; j ++)
        {
            if (i < m - 1 || j < n - 1)
                a[i][j] = temp[i * n + j];
        }
            

    a[m - 1][n - 1] = -1; // the space should = -1
    //consider revise
    Point* space = new Point;
    space->x = m - 1;
    space->y = n - 1;
    return reverse; // return the coordinate of space
}

// 生成可解的随机矩阵，如果当前状态不可解，那么就重新生成直至可解
int generateSolvableRandomMatrix(int a[][100], Point* size)
{
    int m = size->x;
    int n = size->y;
    int sum = m * n; // total numbers to generate

    srand(unsigned(std::time(0)));
    vector<int> temp;
    for (int i = 0; i < sum - 1; i ++)
        temp.push_back(i);
    random_shuffle(temp.begin(), temp.end() - 1); // generate random numbers

    int reverse = 0;
    //O(n^2)
    for (int i = 0; i < sum - 1; i ++)
        for (int j = 0; j < i; j ++)
            if (temp[j] > temp[i])
                reverse ++;
    while (reverse % 2 != 0)
    {
        random_shuffle(temp.begin(), temp.end() - 1); 
        reverse = 0;
        for (int i = 0; i < sum - 1; i ++)
            for (int j = 0; j < i; j ++)
                if (temp[j] > temp[i])
                    reverse ++;        
    }

    for (int i = 0; i < m; i ++)
        for (int j = 0; j < n ; j ++)
        {
            if (i < m - 1 || j < n - 1)
                a[i][j] = temp[i * n + j];
        }
            
    a[m - 1][n - 1] = -1; // the space should = -1
    //consider revise
    Point* space = new Point;
    space->x = m - 1;
    space->y = n - 1;
    return reverse; // return the coordinate of space
}

// 根据当前矩阵状态判断是否可解
// to judge a matrix has been recovered or not
void judgeMatrixRecovered(int a[][100], Point* point) 
{
    int m = point->x;
    int n = point->y;
    bool recover = true;
     // if there are any unrecovered, there must be at least twos.
    for (int i = 0; i < m; i ++)
        for (int j = 0; j < n; j ++)
            if (a[i][j] != (i * n + j) && !(i == m - 1 && j == n - 1))
            {
                recover = false;
                //printf("The un point is (%d, %d), it shall be %d.\n", i, j, (i * n + j));
                break;
            }
    if (recover)
        printf("Recovered!\n"), printf("You have finished the puzzle!\n"), exit(0);
    else 
        printf("Unrecovered!\n");            
}

// basic operation: w, move space from down to up
void MoveSpaceFromDownToUp_w(int a[][100], Point* space)
{
    
    if (space->x == 0)
        return;
    mySwap(&a[space->x][space->y], &a[(space->x) - 1][space->y]);
    space->x --;
}

// basic operation: w, move space from right to left
void MoveSpaceFromRightToLeft_a(int a[][100], Point* space)
{

    if (space->y == 0)
        return;
    mySwap(&a[space->x][space->y], &a[(space->x)][space->y -1]);
    
    space->y --;
}

void MoveSpaceFromUpToDown_s(int a[][100], Point* space, Point* size)
{

    int bottom = size->x - 1;
    if (space->x >= bottom)
        return;
    mySwap(&a[space->x][space->y], &a[(space->x + 1)][space->y]);
    space->x ++;
}

void MoveSpaceFromLeftToRight_d(int a[][100], Point* space, Point* size)
{

    int broad = size->y - 1;
    if (space->y >= broad)
        return;
    mySwap(&a[space->x][space->y], &a[(space->x)][space->y + 1]);
    space->y ++;
}

// 记录移动状态，基于规则交换生成初始矩阵
//###############################################################################################################################3
Point* generateRecordRandomMatrix(int a[][100], Point* size, int difficulty)
{

    printf("in generate record randoms\n");
    srand(unsigned(std::time(0)));    
    
    int m = size->x;
    int n = size->y;
    int sum = m * n; // total numbers to generate      
    int times = (2 << (difficulty * 2));
    printf("I have shifted %d times\n", times);
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            a[i][j] = i * n + j;
    
    a[m - 1][n - 1] = -1;
    Point* current = new Point;
    current->x = m - 1;
    current->y = n - 1;
    
    while (0 < times --)
    {
        int __move = rand() % 4;
    //    printf(" __ move = %d\n", __move);
        // 0 == a    1 == d    2 == w    3 == s
        switch (__move) 
        {
            case 0 :
            {
                if (current->y == 0)
                    break;
                record_solution.push_back('a');
                MoveSpaceFromRightToLeft_a(a, current);
                break;
            }
            case 1 :
            {
                if (current->y == n - 1)
                    break;
                record_solution.push_back('d');
                MoveSpaceFromLeftToRight_d(a, current, size);
                break;
            }
            case 2 :
            {
                if (current->x == 0)
                    break;
                record_solution.push_back('w');
                MoveSpaceFromDownToUp_w(a, current);
                break;
            }
            case 3 : 
            {
                if (current->x == m - 1)
                    break;
                record_solution.push_back('s');
                MoveSpaceFromUpToDown_s(a, current, size);
                break;
            }
        }
    }
            
    printMatrix(a, size);
    int steps = record_solution.size();
/*    printf("Move directions: ");
    for (int i = 0; i < steps; ++i)
        printf("%c ", record_solution[i]);
    printf("\n");
    */
    return current;
}

//################################################################################################################################3
// 保存初始记录，以便还原
char* saveRecordAtTheBeginning(int a[][100], int record[][10000], Point* space, Point* size, int cnt)
{

    char* name = new char[50];
    name[0] = 'd';
    name[1] = 'e';
    name[2] = 'm';
    name[3] = 'o';
    name[4] = '\0';
    printf("read\n");
    int m = size->x;
    int n = size->y;   
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
        {
            int cur = i * n + j;
            record[cnt][cur] = a[i][j];
        }
    printf("save\n");
    return name; 
}


// 保存当前存档进度
char* saveRecord(int a[][100], int record[][10000], Point* space, Point* size, int cnt)
{
    system("cls");
    printf("=========================================================================================\n");
    printf("=========================================================================================\n");
    printf("Now You begin to save a picture\n");    
    char* name = new char[50];
    printf("please input your file name:  ");
    scanf("%s", name);
    printf("read\n");
    int m = size->x;
    int n = size->y;   
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
        {
            int cur = i * n + j;
            record[cnt][cur] = a[i][j];
        }
    printf("save\n");
    return name; 
}

// 读取进度
Point* readRecord(int a[][100], int record[][10000], Point* space, Point* size, int cnt)
{
    printf("=========================================================================================\n");
    printf("=========================================================================================\n"); 
    printf("Now you have read a record!\n");    
    Point* current = new Point;
    int m = size->x;
    int n = size->y;
    int capacity = m * n;
    // x = i % n
    // y = i / n
    for (int i = 0; i < capacity; ++i)
    {
        int cur_x = i % n;
        int cur_y = i / n;
        a[cur_y][cur_x] = record[cnt][i];
        if (a[cur_y][cur_x] == -1)
        {
            current->x = cur_y;
            current->y = cur_x;
        }
    }    
    return current;
}

//************************************************************************************************************************
// 周恩贤算法：找到目标位置在哪里，并且返回
Point* findTargetPosition(int a[][100], Point* size, Point* start, int target)
{
    int m = size->x;
    int n = size->y;    

    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            if (a[i][j] == target)
                {
                    Point* aim = new Point;
                    aim->x = i;
                    aim->y = j;
                    return aim;
                }
    printf("Finding Failure!\n");
    return start; // robust
}

// 最基本的操作：左移
void fromRightsideToLeftside(int a[][100], Point* space, Point* size) // 低级情况
{
    MoveSpaceFromUpToDown_s(a, space, size);
    MoveSpaceFromRightToLeft_a(a, space);
    MoveSpaceFromRightToLeft_a(a, space);
    MoveSpaceFromDownToUp_w(a, space);
}

// 最基本的操作：右移
void fromLeftsideToRightside(int a[][100], Point* space, Point* size) // 低级情况
{
    MoveSpaceFromUpToDown_s(a, space, size);
    MoveSpaceFromLeftToRight_d(a, space, size);
    MoveSpaceFromLeftToRight_d(a, space, size);
    MoveSpaceFromDownToUp_w(a, space);
}

// 最基本的操作：上移
void fromDownsideToUpside(int a[][100], Point* space, Point* size)
{
    MoveSpaceFromLeftToRight_d(a, space, size);
    MoveSpaceFromDownToUp_w(a, space);
    MoveSpaceFromDownToUp_w(a, space);
    MoveSpaceFromRightToLeft_a(a, space);
}

//*************************************************************************************
// 回到右下角
void setBackToRightDownCorner(int a[][100], Point* size, Point* space)
{
    int m = size->x;
    int n = size->y;

    while (space->x < m - 1)
        MoveSpaceFromUpToDown_s(a, space, size);
    while (space->y < n - 1)
        MoveSpaceFromLeftToRight_d(a, space, size);

}

// 前往目标未复原函数的左侧

void gotoTargetLeft(int a[][100], Point* size, Point* space, Point* target)
{
    int m = size->x;
    int n = size->y;

    // counting distance
    int value = a[target->x][target->y];
    int ideal_x = value / n;
    int ideal_y = value % n;

    int distance_x = space->x - target->x; // 竖着的
    int distance_y = space->y - target->y; // 横着的

    // 保证该点的目标并不在最后两行
    if (target->x == m - 1) // 站在最底下
    {
        if (target->y == 0)  // 猥琐占位在最左边
        {
            if (ideal_y == 0) //　直接可以执行往上移的操作了
            {
                MoveSpaceFromDownToUp_w(a, space);
                for (int i = 0; i < distance_y; ++i)
                    MoveSpaceFromRightToLeft_a(a, space);
                MoveSpaceFromUpToDown_s(a, space, size);
                -- target->x;
                return;                
            }
            else   // 你不需要往上移，那么我就不客气了
            {
                for (int i = 0; i < distance_y; ++i)
                    MoveSpaceFromRightToLeft_a(a, space);
                
                return;                
            }
        }
        else if (target->y == n - 1 && ideal_y == n - 1) // 站在最右边
        {
            for (int i = 0; i < distance_x - 1; ++i )
                MoveSpaceFromDownToUp_w(a, space);
            return;
        }
        else // 正常占位，但是还是在最底下
        {
            MoveSpaceFromDownToUp_w(a, space);
            for (int i = 0; i <= distance_y; ++i)
                MoveSpaceFromRightToLeft_a(a, space);
            
            MoveSpaceFromUpToDown_s(a, space, size);
            ++ target->y;     // 需要交换
            return;               
        }  
    }
    else if (target->y == 0) // 站在最左边，但不在最底下
    {
        if (ideal_y == 0) // 这种情况直接可以往上移了
        {
            for (int i = 0; i < distance_y; ++i)
                MoveSpaceFromRightToLeft_a(a, space);
            
            for (int i = 0; i < distance_x - 1; ++ i)
                MoveSpaceFromDownToUp_w(a, space);

            return;
        }
        else // 但还是要交换
        {
            for (int i = 0; i < distance_x; ++i)
                MoveSpaceFromDownToUp_w(a, space);

            for (int i = 0; i < distance_y; ++i)
                MoveSpaceFromRightToLeft_a(a, space);

            ++ target->y;  // 需要交换
            return ;
        }

    } // 没有其他幺蛾子了，站在中间地区
    else 
    {
        for (int i = 0; i <= distance_y; ++i)
            MoveSpaceFromRightToLeft_a(a, space);

        for (int i = 0; i < distance_x; ++i)
            MoveSpaceFromDownToUp_w(a, space);
        
        return;
    }

}

// 保证位于 gototargetleft之后才调用

void plainMoveToLeft(int a[][100], Point* size, Point* space)
{
    MoveSpaceFromLeftToRight_d(a, space, size);
    MoveSpaceFromUpToDown_s(a, space, size);
    MoveSpaceFromRightToLeft_a(a, space);
    MoveSpaceFromRightToLeft_a(a, space);
    MoveSpaceFromDownToUp_w(a, space);
}

void plainMoveToRight(int a[][100], Point* size, Point* space)
{
    MoveSpaceFromUpToDown_s(a, space, size);
    MoveSpaceFromLeftToRight_d(a, space, size);
    MoveSpaceFromLeftToRight_d(a, space, size);
    MoveSpaceFromDownToUp_w(a, space);
    MoveSpaceFromRightToLeft_a(a, space);
}

// 保证不再后两行用， 应该返回-1在该格子下面的状态。 预期：移到目标的正下
void leftClear(int a[][100], Point* size, Point* space, Point* target)
{
    if (target->y == 0) return;
    if (target->x = space->x + 1 && target->y == space->y) return;

    int m = size->x;
    int n = size->y;
    printf("target position ist: (%d, %d)\n", target->x, target->y);
    int value = a[target->x][target->y];
    int ideal_x = value / n;
    int ideal_y = value % n;
    printf("value on target position is %d\n", value);
    int intend_move_x = target->x - ideal_x;
    int intend_move_y = target->y - ideal_y;
    printMatrix(a, size);
    printf("ideal position ist: (%d, %d)\n", ideal_x, ideal_y);
    printf("I am reday to move!\n");                           // 在左边上，在中间，在后边
    if (intend_move_y == 0) // 已经完成
    {
        printf("finished!\n");
        
    }
    else if (ideal_y == 0) // 移动最左边那个情况
    {
        printf("impossible move\n");
        for (int i = 0; i < intend_move_y - 1; ++i)
            plainMoveToLeft(a, size, space);

        printMatrix(a, size);

        MoveSpaceFromLeftToRight_d(a, space, size);
        MoveSpaceFromUpToDown_s(a, space, size);
        MoveSpaceFromRightToLeft_a(a, space);
        printf("OK\n");
        printMatrix(a, size);
    }
    else if (intend_move_y > 0)// 其他移位情况
    {
        printf("normal move, y > 0\n");
        for (int i = 0; i < intend_move_y; ++i)
            plainMoveToLeft(a, size, space);
        
        MoveSpaceFromUpToDown_s(a, space, size);
        MoveSpaceFromLeftToRight_d(a, space, size);
    }
    else if (intend_move_y < 0)// 其他移位情况
    {
        printf("normal move, y < 0\n");
        for (int i = 0; i < - intend_move_y; ++i)
            plainMoveToRight(a, size, space);
        
        MoveSpaceFromUpToDown_s(a, space, size);
        MoveSpaceFromLeftToRight_d(a, space, size);
    }    

    printf("left clear!\n");
}

// Not used
void getBackToLeft(int a[][100], Point* size, Point* space, Point* target)
{
    int m = size->x;
    int n = size->y;
    int value = a[target->x][target->y];
    int aim_x = value / n;
    int aim_y = value % n;

    int distance_x = target->x - aim_x;
    int distance_y = target->y - aim_y;
    printf("the distance from value %d to position is x: %d\ty:%d\n", value, aim_x, aim_y);

    for (int i = 0; i < distance_y - 1; ++i)
    {
        MoveSpaceFromLeftToRight_d(a, space, size);
        fromRightsideToLeftside(a, space, size);
    }
    MoveSpaceFromLeftToRight_d(a, space, size);
}

//########################################################################################################################33
// output a picture 输出图片 需要提供输出图片的文件名和读入图片的文件名
void updatePicture(int a[][100], Point* size, const char* filename)
{
    int m = size->x;
    int n = size->y;
    char* input_name = new char[50];
    printf("please enter the BMP you want to input.\n");
    scanf("%s", input_name);

    BMP b = BMP(input_name);

    printf("great\n");

    printf("b.h: %d\tb.w:  %d\n", b.h, b.w);
    BMP c = BMP(b.w, b.h);

    for (int i = 0; i < m; i ++) for (int j = 0; j < n; j ++)
    {
        int intendh = b.h / m;
        int intendw = b.w / n;
        int currenth = i * intendh;
        int currentw = j * intendw;

        if (a[i][j] == -1)
        {
            for (int _i = 0; _i < intendh; ++ _i) for (int _j = 0 ; _j < intendw; ++ _j)
            {
                unsigned char* reverse = c.pixel(currenth + _i, currentw + _j); // 起始坐标点
                reverse[0] = 255, reverse[1] = 255, reverse[2] = 255;        
                             
            } 
            continue;                 
        }
        //5, 4     19 -> 3  -> 4
        int realw = a[i][j] % n;
        int realh = a[i][j] / n;
                                                     
        int temp_h = realh * intendh;
        int temp_w = realw * intendw;
    //    printf("[%d] (%d, %d)\n", a[i][j], realh, realw);
        for (int _i = 0; _i < intendh; ++ _i) for (int _j = 0 ; _j < intendw; ++ _j)
        {
            unsigned char* reverse = c.pixel(currenth + _i, currentw + _j); // 起始坐标点
            unsigned char* temp = b.pixel(temp_h + _i, temp_w + _j);  // 真实坐标点
            reverse[0] = temp[0], reverse[1] = temp[1], reverse[2] = temp[2];  //           
        }               
    }

    for (int i = 0; i < b.h; i += (b.h / m)) for (int j = 0; j < b.w; ++j)
    {
        for (int k = i - 1; k <= i + 1; ++ k)
        if (k >= 0 && k < b.h)
        {
            unsigned char* reverse = c.pixel(k, j);
            reverse[0] = 0, reverse[1] = 0, reverse[2] = 0;
        }
    }

    for (int i = 0; i < b.h; i ++) for (int j = 0; j < b.w; j += (b.w / n))
    {
        for (int k = j - 1; k <= j + 1; ++ k)
        if (k >= 0 && k < b.w)
        {
            unsigned char* reverse = c.pixel(i, k);
            reverse[0] = 0, reverse[1] = 0, reverse[2] = 0;
        }

        if (i > (b.h / m * (m - 1)) && (j > b.w / n * (n - 1)))
        {
            unsigned char* temp = c.pixel(i, j);
            temp[0] = 255, temp[1] = 255, temp[2] = 255;
        }
    }    

    system("cls");
    printf("=========================================================================================\n");
    printf("=========================================================================================\n");
    c.save(filename);

    printf("Now You have saved a bmp picutre in name: %s\n", filename);

}

vector<char> solution; // 记录当前走过的状态

// 深度优先搜索找出算法
bool dfs(int g, int a[][100], Point* size, Point* space) // g代表之前走过的路径曼哈顿距离
{
    if (g > 1) {
        char u = solution[g - 1];
        char v = solution[g - 2];
        if (u == 'W' && v == 'S') return false; // 防止前后两步走的一样
        if (u == 'A' && v == 'D') return false; // 防止前后两步走的一样
    }
    int h = 0; // 距离目标状态的距离
    int m = size->x;
    int n = size->y;
    // calculate h
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (a[i][j] == -1)
                continue;
            int value = a[i][j];
            int value_x = a[i][j] / n;
            int value_y = a[i][j] % n;
            h += abs(value_x - i) + abs(value_y - j); // 计算当前状态下，h的变化
        }
    }
    // printf("H = %d\n", h);
    if (h == 0) // 已经解出来了
    {
        printf("Solved!\n");
        printMatrix(a, size);
        return true;
    }

    int f = g + h; // f代表真正的heuristic函数

    if (f > k) return false;
    if (space->x > 0)  // 深度优先搜索
    {
        solution.push_back('W');
        MoveSpaceFromDownToUp_w(a,space);
        if (dfs(g + 1, a, size, space)) return true;
        solution.pop_back();
        MoveSpaceFromUpToDown_s(a, space, size);
    }

    if (space->y > 0) 
    {
        solution.push_back('A');
        MoveSpaceFromRightToLeft_a(a,space);
        if (dfs(g + 1, a, size, space)) return true;
        solution.pop_back();
        MoveSpaceFromLeftToRight_d(a, space, size);
    }

    if (space->y < n - 1) 
    {
        solution.push_back('D');
        MoveSpaceFromLeftToRight_d(a, space, size);
        if (dfs(g + 1, a, size, space)) return true;
        solution.pop_back();
        MoveSpaceFromRightToLeft_a(a, space);
    }    

    if (space->x < m - 1) 
    {
        solution.push_back('S');
        MoveSpaceFromUpToDown_s(a, space, size);
        if (dfs(g + 1, a, size, space)) return true;
        solution.pop_back();
        MoveSpaceFromDownToUp_w(a, space);
    }     
    
    return false;
}


//###########################################################################################33


// 主循环函数
void mainLoop(int a[][100], Point* space, Point* size)
{
    system("cls");
    printf("=========================================================================================\n");
    printf("=========================================================================================\n");     
    printf("Now you have entered In Main loop\n\n");

    int record[20][10000];
    vector <char*> memo; // 存档名
    int count = 0;  // 存档序号 
    // pre-save
    char* temp = saveRecordAtTheBeginning(a, record, space, size, count); // 读入存档名
    memo.push_back(temp); // 读入
    count = memo.size();  // 存档大小
    printf("size: %d\n", count); // 
    for (int i = 0; i < count; ++i)
    {
        printf("[%d] %s\n", i, memo[i]);
    }  
    //  

    char current;  // 
    cin >> current;
    while (current != 'p')  // P 退出
    {
        // functions
        switch (current)
        {
            case 'w' :
            {
                if (space->x >= 0)
                    record_solution.push_back('w');
                printf("Move W!\n");
                MoveSpaceFromDownToUp_w(a, space);
                size = printMatrix(a, size);
                judgeMatrixRecovered(a, size); 
                break;
            }
            case 'a' :
            {
                printf("Move A!\n");
                if (space->y > 0)
                    record_solution.push_back('a');
                MoveSpaceFromRightToLeft_a(a, space);
                size = printMatrix(a, size);
                judgeMatrixRecovered(a, size);                 
                break;
            }
            case 's' :
            {
                printf("Move S!\n");
                if (space->x < size->x - 1)
                    record_solution.push_back('s');
                MoveSpaceFromUpToDown_s (a, space, size);
                size = printMatrix(a, size);
                judgeMatrixRecovered(a, size); 
                break;
            }
            case 'd' : 
            {
                printf("Move D!\n");
                if (space->y < size->y - 1)
                    record_solution.push_back('d');
                MoveSpaceFromLeftToRight_d (a, space, size);
                size = printMatrix(a, size);
                judgeMatrixRecovered(a, size); 
                break;
            }
            case 'c' : // test if finished  C 判断当前状态是否结束
            {
                judgeMatrixRecovered(a, size);
                break;
            }
            case 'i' : // save record  I 存取一个进度
            {
                char* temp = saveRecord(a, record, space, size, count); // 读进来有问题
                memo.push_back(temp);
                count = memo.size();
                printf("size: %d\n", count);
                for (int i = 0; i < count; ++i)
                {
                    printf("[%d] %s\n", i, memo[i]);
                }
                
                break;
            }
            case 'o' : // read record  O 读取一个进度
            {
                system("cls");
                printf("=========================================================================================\n");
                printf("=========================================================================================\n");                 
                printf("please enter your record name:");
                char* temp = new char[50];
                scanf("%s", temp);
                bool valid = false;
                for (int i = 0; i < count; ++i)
                {
                    if (*memo[i] == *temp)
                    {
                        delete space;
                        
                        space = readRecord(a, record, space, size, i);
                        valid = true;
                        
                        printf("match\n");
                        break;
                    }
                    printf("Comparison:%s\t%s\n", memo[i], temp);
                }
                if (! valid)
                    printf("Invalid Record Name! Please retry!\n");
                delete temp;
                break;
            }
            case 'r' : // 输入 R 回到开头
            {
                system("cls");
                printf("=========================================================================================\n");
                printf("=========================================================================================\n");                
                printf("Now you have returned to the beginning!\n");
                delete space;
                
                space = readRecord(a, record, space, size, 0);
                break;
            }
            case 'f' : // automatic reduce  DFS 搜索算法
            {   
                for (k = 1; ; ++k)
                    if ( dfs(0, a, size, space) )  break;
                /*
                Point* target = findTargetPosition(a, size, space, 6);
                printf("target: (%d,%d)\n", target->x, target->y);
                gotoTargetLeft(a, size, space, target);
                delete target;
                
                size = printMatrix(a, size);     
                break;
                */
               break;
            }   
            case 'g' : // save image 存储图像
            {
                char* filename = new char[50];
                printf("Print Mode: please enter your filename:");
                scanf("%s", filename);
                updatePicture(a, size, filename);
                break;
            }
            case 'F' : // 一个失败的尝试，周恩贤算法
            {
                Point* target = findTargetPosition(a, size, space, 6);
                if (target->x == 1 && target->y == 0)   break;
                printf("index: (%d, %d)\n", target->x, target->y);
                setBackToRightDownCorner(a, size, space);
                printf("index: (%d, %d)\n", target->x, target->y);
                gotoTargetLeft(a, size, space, target);
                printf("index: (%d, %d)\n", target->x, target->y);
                printMatrix(a, size);
                leftClear(a, size, space, target);
                printf("index: (%d, %d)\n", target->x, target->y);
                printMatrix(a, size);
                break;
            }
            case 'Z' : // 冯卓尔算法
            {
                int steps = record_solution.size();
                printf("Do you want to see the screen? Input 1 to see, 0 to skip!\n");
                int mode = 0;
                scanf("%d", &mode);
                if (mode == 1)
                {
                    for (int i = steps - 1; i >= 0; --i)
                    {
                        switch (record_solution[i])
                        {
                            case 'a' : 
                            {
                                MoveSpaceFromLeftToRight_d(a, space, size);
                                record_solution.push_back('a');
                                break;
                            }
                            case 'w' :
                            {
                                MoveSpaceFromUpToDown_s(a, space, size);
                                record_solution.push_back('w');
                                break;
                            }
                            case 's' :
                            {
                                MoveSpaceFromDownToUp_w(a, space);
                                record_solution.push_back('s');
                                break;
                            }
                            case 'd' :
                            {
                                MoveSpaceFromRightToLeft_a(a, space);
                                record_solution.push_back('d');
                                break;
                            }
                        }
                        printMatrix(a, size);   
                    }
                    printMatrix(a, size); 
                    printf("Puzzled Solved!\n");           
                    for (int i = 0; i < 1000000; ++i);
                    exit(1);   
                }
                else if (mode == 0) // 仅仅输出如何还原
                {
                    printf("Your reduction steps costs %d Times!\n", record_solution.size());
                    printf("Now see how You can solve it!\n");
                    for (int i = steps - 1; i >= 0; --i)
                    {
                        printf("%c ", record_solution[i]);
                    }
                    printf("\n");
                }
                break;

            }
            case 'Q' : // 需要多少步才能还原
            {
                system("cls");
                printf("=========================================================================================\n");
                printf("=========================================================================================\n"); 
                printf("now the vector space: %d\n", record_solution.size());                
            }
        }
        cin >> current;
    }
}
#include "point.h"

int main()
{
    Point* size_test = new Point;
    int test[100][100];
    int standard[100][100];
    printf("=================================================================================================================\n");
    printf("=================================================================================================================\n");    
    printf("Welcome to the Final Puzzle by Andrew Feng!\n");
    printf("I provided 2 different mode: one is based on the dfs to solve the puzzle, and the other mode is based on regulation-based artificial intelligence\n");
    printf("But first you had to split the picture up!\n");
    printf("please enter two integers instrcuting me to split it up\n");
    printf("For example, 10 20 implies it has 10 rows and 20 columns!\n");
    printf("Now please enter (M, N): ");

    scanf("%d%d", &(size_test->x), &(size_test->y));
    printf("the test node is: (%d, %d)\n", size_test->x, size_test->y);    
    printf("In mode 1, you can apply Andrew Feng Solution to the game, but in mode 2, you cannot. However, the efficiency is improved by doing so.\n");
    printf("[1] Andrew Feng Solution.\n[2] Normal A* solution.\n");
    printf("Please select a mode: \n");            
    int mode = 0;
    scanf("%d", &mode);
    
    if (mode == 1)
    {
        printf("=========================================================================================\n");
        printf("=========================================================================================\n");
        printf("We provide 8 difficult ranks:\n");
        printf("[1]Primary\n[2]Junior\n[3]Senior\n[4]Undergraduate\n[5]Master\n[6]Doctor\n");
        printf("[7]Associate researcher\n[8]Professor\n");
        printf("enter one number to select and continue: ");
        int choose_diffi; // 选择难度
        cin >> choose_diffi;
        if (choose_diffi >= 0 && choose_diffi <= 8)
        {
            Point* space = generateRecordRandomMatrix(standard, size_test, choose_diffi);  
            mainLoop(standard, space, size_test);
        }
    }
    else if (mode == 2) // 正常状态，无法使用冯卓尔算法，但是会提升效率
    {

        int reverse = generateSolvableRandomMatrix(test, size_test);

        if (reverse % 2 == 0)
            printf("This is Computable!\n");
        else    
            printf("This is incomputable!\n");

        Point* space_test = new Point; // space means the coordinate of the gap, (i, j)
        space_test->x = size_test->x - 1;
        space_test->y = size_test->y - 1;
        size_test = printMatrix(test, size_test);
        
        mainLoop(test, space_test, size_test);
        size_test = printMatrix(test, size_test);         

    }
                              

    
    return 0;
}
#pragma once
// mit liscence
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <ctime>
#include <windows.h>
#include <stdlib.h> //


using namespace std;

struct Point {
    int x;
    int y;
};

//basic functions
void mySwap(int* a, int* b);

Point* readMatrix(int a[][100], Point* size);
Point* printMatrix(int a[][100], Point* size);
int generateRandomMatrix(int a[][100], Point* point); // consider revise, return reverse num
int generateSolvableRandomMatrix(int a[][100], Point* point);

Point* generateRecordRandomMatrix(int a[][100], Point* size, int difficulty);

void judgeMatrixRecovered(int a[][100], Point* point); // consider revise

void MoveSpaceFromDownToUp_w(int a[][100], Point* space); // please note about the board conditions
void MoveSpaceFromUpToDown_s(int a[][100], Point* space, Point* size);
void MoveSpaceFromRightToLeft_a(int a[][100], Point* space);
void MoveSpaceFromLeftToRight_d(int a[][100], Point* space, Point* size);

void mainLoop(int a[][100], Point* space, Point* size);

// save and read
char* saveRecordAtTheBeginning(int a[][100], int record[][10000], Point* space, Point* size, int cnt);
char* saveRecord(int a[][100], int record[][10000], Point* space, Point* size, int n);
Point* readRecord(int a[][100], int record[][10000], Point* space, Point* size, int n);

//
void solve(); // the most difficult part of this work


// zex
Point* findTargetPosition(int a[][100], Point* size, Point* start, int target);
void gotoTargetLeft(int a[][100], Point* size, Point* space, Point* target);
void setBackToRightDownCorner(int a[][100], Point* size, Point* space);

void plainMoveToLeft(int a[][100], Point* size, Point* space);
void plainMoveToRight(int a[][100], Point* size, Point* space);
void leftClear(int a[][100], Point* size, Point* space, Point* target);

void gotoTargetDown(int a[][100], Point* size, Point* space, Point* target);


void fromRightsideToLeftside(int a[][100], Point* space, Point* size);
void getBackToLeft(int a[][100], Point* size, Point* space, Point* target);

void moveUp(int a[][100], Point* space, Point* size);
void fromDownsideToUpside(int a[][100], Point* space, Point* size);

void fromLeftsideToRightside(int a[][100], Point* space, Point* size);


void cornerUpRight();
void cornerDownLeft();
////////////////

// read pic

void updatePicture(int a[][100], Point* size, const char* filename);

//
bool dfs(int g, int a[][100], Point* size, Point* space);

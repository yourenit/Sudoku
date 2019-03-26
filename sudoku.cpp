#include "sudoku.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <random>

std::random_device r;
std::default_random_engine e1(r());
std::uniform_int_distribution<int> uniform_dist(0, 8);

Sudoku::Sudoku()
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            data[i][j] = 0;
        }
    }
}


Sudoku::~Sudoku()
{
}

//生成随机数组
int* Sudoku::random()
{
    int *randomInt;
    randomInt = (int*)malloc(sizeof(int) *9 );
//     std::random_device r;
//     std::default_random_engine e1(r());
//     std::uniform_int_distribution<int> uniform_dist(0, 8);           //导致数独重置失败（局面不变）,与文件开头冲突
    for (int i = 0; i < 9; i++) {
        randomInt[i] = i+1;
    }
    for (int j = 0; j < 20; j++) {
        int t = uniform_dist(e1);
        int temp = randomInt[0];
        randomInt[0] = randomInt[t];
        randomInt[t] = temp;
    }
    return randomInt;
}

//将生成的随机数组放入局面的第一行
void Sudoku::setRandomInArray()
{
    int *value_random = random();
    for (int i = 0; i < 9; i++) {
        this->data[0][i] = value_random[i];
    }
}

//复制矩阵
void Sudoku::copyArray(int srcArray[9][9], int dstArray[9][9])
{
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            dstArray[i][j] = srcArray[i][j];
        }
    }
}

// bool Sudoku::findFirstEmpty(int & x, int & y)
// {
//     for (int i = 0; i < 9; i++) {
//         for (int j = 0; j < 9; j++) {
//             if (data[i][j] == 0) {
//                 x = i; y = j;
//                 return true;
//             }
//         }
//     }
//     return false;
// }

//判断规则，数字k是否已经存在行，列或者坐标(i,j)所在的九宫格里
bool Sudoku::tryfill(int i, int j, int k)
{
    if (this->data[i][j] != 0) {
        return false;
    }
    //判断列中是否具有数字k
    for (int row = 0; row < 9; row++) {
        if (this->data[row][j] == k) {
            return false;
        }
    }
    //判断行中是否具有数字k
    for (int col = 0; col < 9; col++) {
        if (this->data[i][col] == k) {
            return false;
        }
    }
    //判断数字k是否在坐标(i,j)所在的九宫格内
    int up1 = (i / 3) * 3 + 3;
    int up2 = (j / 3) * 3 + 3;
    for (int p = up1 - 3; p < up1; ++p) {
        for (int q = up2 - 3; q < up2; ++q) {
            if (this->data[p][q] == k) {
                return false;
            }
        }
    }
    this->data[i][j] = k;
    return true;
}
//迭代到下一个坐标 (8, 8)下一个是(9, 9); (0, 0)上一个是(8, 8）
void Sudoku::next(int & x, int & y)
{
    ++y;
    x += (y / 9);
    if (x < 9) {
        y %= 9;
    }
}

//上一个坐标
void Sudoku::previous(int & x, int & y)
{
    --y;
    if (y < 0) {
        x -= 1;
        x < 0 ? x = 8 : x;
        y = 8;
    }
}

void Sudoku::clear()
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            data[i][j] = 0;
        }
    }
}

//生成终局
void Sudoku::initsudoku()
{
    clear();
    setRandomInArray();
    int row=1, col = 0;
    int marks[9][9][9] = { {{0}} };	//标记是否被尝试过
    for (;;) {
        if (row == 9 && col == 9) {                         //如果已经填充满了，则返回
            return;
        }

        int *ranNum = random();                             //产生随机数组,实际只需要一个
        bool fill_row_success = false;
        for (int k = 0; k < 9; k++) {
            if (marks[row][col][ranNum[k] - 1] != 0) {      //如果数字ranNum[k]已经被尝试过
                continue;
            }
            //从第二行开始，依次填写数字，并且判断是否重复
            if (tryfill(row, col, ranNum[k])) {             //如果可以填写
                next(row, col);                             //则跳转下一个格子
                fill_row_success = true;                    //填充成功
                break;
            }
        }
        if (!fill_row_success) {                            //如果填充不成功
            for (int i = 0; i < 9; ++i) {
                marks[row][col][i] = 0;                     //重置状态
            }
            previous(row, col);                             // 回溯, 迭代后两个值都会发生变化
            if (this->data[row][col] != 0) {
                marks[row][col][this->data[row][col] - 1] = 1; //已尝试过，标记
            }

            if (row == 0 && col == 8) {                     //如果回溯到初始状态，则表示结束（一般不会出现这种现象）
                break;
            }
            this->data[row][col] = 0;
        }

    }
}

void Sudoku::makeLevel(int table[9][9], int blankGridNum)
{
    initsudoku();
    copyArray(this->data, this->answer);
    copyArray(this->data, table);
    int backupArray[9][9];
    copyArray(table, backupArray);
    for (int k = 0; k < blankGridNum;) {
        int i = uniform_dist(e1);
        int j = uniform_dist(e1);
        if (table[i][j] == 0) {
            //已设置mask
            continue;
        }

        table[i][j] = 0;
        if (this->getSolution(table)) {
            ++k;
            backupArray[i][j] = table[i][j]; //保持数据的同步
        }
        else {
            //restore array
            copyArray(backupArray, table);
        }
    }
}

bool Sudoku::getSolution(int question[9][9])
{
    copyArray(question, this->data);
    if (parseState(0, 0)) {
        return true;
    }
    return false;
}

bool Sudoku::parseState(int i, int j)
{
    if (_solution[i][j] != 0) {
        if (j < 8) {
            if (parseState(i, j + 1))   /* 到同一行的下一位置开始搜索 */
                return true;
        }
        else {
            if (i < 8) {
                if (parseState(i + 1, 0))    /* 到下一行的第一个空格开始搜索 */
                    return true;
            }
            else {
                return true;  /* i >= 9  && j >= 9, 搜索结束 */
            }
        }
        return false;
    }
    int k, m, n, p, q;
    if (i >= 9 || j >= 9)
        return true;

    for (k = 1; k <= 9; k++) {
        int can = 1;                // can 变量用于记录数字k能否放在 (i, j) 处
        for (m = 0; m < 9; ++m) {
            if (_solution[m][j] == k) {  // 检查同一列是否出现过数字k
                can = 0;
                break;
            }
        }

        if (can) {
            for (n = 0; n < 9; ++n) {
                if (_solution[i][n] == k) {  // 检查同一行是否出现过数字k
                    can = 0;
                    break;
                }
            }
        }

        if (can) {
            int up1 = (i / 3) * 3 + 3; // (i,j)方格所在的3×3小方格i坐标的上限
            int up2 = (j / 3) * 3 + 3;   // (i,j)方格所在的3×3小方格在j坐标的上限

            for (p = up1 - 3; p < up1; ++p) { /* 检查在3×3的小方格中是否出现了同一个数字 */
                if (can == 0)   //跳出外层循环
                    break;
                for (q = up2 - 3; q < up2; ++q) {
                    if (_solution[p][q] == k) {
                        can = 0;
                        break;
                    }
                }
            }
        }
        if (can) {
            _solution[i][j] = k;
            if (j < 8) {
                if (parseState(i, j + 1))   /* 到同一行的下一位置开始搜索 */
                    return true;
            }
            else {
                if (i < 8) {
                    if (parseState(i + 1, 0))    /* 到下一行的第一个空格开始搜索 */
                        return true;
                }
                else {
                    return false; /* i >= 9  && j >= 9, 搜索结束 */
                }
            }
            _solution[i][j] = 0;  /*找不到解要回复原状，否则会对下面的搜索造成影响 */
        }
    }
    return false; /*  1到9都尝试过都不行，则返回递归的上一步 */
}

bool Sudoku::checkSolution(int answer[9][9])
{
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (answer[i][j] != this->answer[i][j])return false;
        }
    }
    return true;
}



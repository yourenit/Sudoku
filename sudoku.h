#ifndef SUDO_H
#define SUDO_H


class Sudoku
{
public:
    Sudoku();
    ~Sudoku();

public:
    void initsudoku();
    void makeLevel(int table[9][9],int blankGridNum);
    bool getSolution(int question[9][9]);
    bool parseState(int i, int j);
    bool checkSolution(int answer[9][9]);
    void copyArray(int srcArray[9][9], int dstArray[9][9]);

private:
    int* random();
    void setRandomInArray();
    bool findFirstEmpty(int &x,int &y);
    bool tryfill(int i,int j,int k);	//尝试将k填入位置(i,j)中
    void next(int &x,int &y);
    void previous(int &x, int &y);
    void clear();

private:
    int data[9][9];
    int _solution[9][9];
    int answer[9][9];
};


#endif // SUDO_H

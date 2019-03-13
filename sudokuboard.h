#ifndef SUDOKUBOARD_H
#define SUDOKUBOARD_H

#include <QGridLayout>
#include <QWidget>
#include <QGroupBox>
#include <QDebug>
#include "itemwidget.h"
#include "sudoku.h"

class SudokuBoard : public QWidget
{
    Q_OBJECT
public:
    explicit SudokuBoard(QWidget *parent = nullptr);
    ~SudokuBoard();
    enum Level {
        Easy,
        Middle,
        Hard,
        Master,
        SuperMaster
    };
    typedef QList<ItemWidget*> Items;
    typedef QVector<QGroupBox *> Groups;

    void setLevel(Level level);
    void clearAnswer();
    bool checkAnswer();
signals:

public slots:
    void resetData();
    void updateItems();

private:
    void init();
    void initData();
    void paintEvent(QPaintEvent *e);

    int _blankGridNum;
    int _raw_data[9][9];//原始题目的数据
    int _going_data[9][9];//正在解答的数据

    QGridLayout *_glayout;
    Groups _groups;
    Items _items;

    Sudoku *_sudoku;
    Level _cur_level;
};

#endif // SUDOKUBOARD_H

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QLCDNumber>
#include <QGroupBox>
#include <QLayout>
#include <QString>
#include <QTimer>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include"sudokuboard.h"

class GameBoard : public QWidget
{
    Q_OBJECT
public:
    explicit GameBoard(QWidget *parent = nullptr);

Q_SIGNALS:
    void gameover();

private slots:
    void setup();
    void start();   //开始游戏空间的变化设置
    void stop();    //暂停游戏
    void finished();   //完成游戏
    void logout();      //退出游戏
    void switchLevel();     //更换等级
    void clearAnswer();     //清除答案
    void updateTimeWidget();
    void updateButtonStatus();  //更新控件状态
private:
    QFrame *createSeperator();
    QPushButton *_resetGameButton;
    QPushButton *_startGameButton;
    QPushButton *_submitButon;
    QPushButton *_clearAnswerButton;
    QPushButton *_suspendButton;
    QPushButton *_logoutButton;

    QComboBox *_levelCheckBox;

    QTimer *_pTimer;
    SudokuBoard * _sudoku_board;
    QLCDNumber *_showTimeWidget;

    int _iTimeSecond;
    int _iTimeMinute;
};

#endif // GAMEBOARD_H

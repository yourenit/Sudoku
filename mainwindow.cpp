#include "mainwindow.h"
#include "gameboard.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setMinimumSize(700,500);
    GameBoard * gameBoard = new GameBoard(this);
    connect(gameBoard,SIGNAL(gameover()),this,SLOT(close()));
    setCentralWidget(gameBoard);
}

MainWindow::~MainWindow()
{
}

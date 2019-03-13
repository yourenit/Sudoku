#include "gameboard.h"
#include"sudokuboard.h"

GameBoard::GameBoard(QWidget *parent) : QWidget(parent)
{
    _iTimeMinute = 0;
    _iTimeSecond = 0;
    setup();
    updateButtonStatus();
}

void GameBoard::setup()
{
    //垂直布局，用来放置QLCDNumber控件和GroupBox控件
    QVBoxLayout *lcdNumberLayout = new QVBoxLayout;
    _showTimeWidget = new QLCDNumber();
    _showTimeWidget->display("00:00:00");
    lcdNumberLayout->addWidget(_showTimeWidget);
    lcdNumberLayout->setSpacing(0);
    QGroupBox *lcdNumberGroup = new QGroupBox;
    lcdNumberGroup->setLayout(lcdNumberLayout);
    lcdNumberGroup->setTitle("计时器");
    _pTimer = new QTimer(this);
    connect(_pTimer, SIGNAL(timeout()), this, SLOT(updateTimeWidget()));

    //垂直布局，用来放置下拉选型控件和GroupBox控件
    QVBoxLayout *levelLayout = new QVBoxLayout;
    _levelCheckBox = new QComboBox();
    _levelCheckBox->setToolTip("选择难度");
    _levelCheckBox->addItems(QStringList() << "初级" << "中等" << "高级"
                             << "大师" << "骨灰");
    connect(_levelCheckBox, SIGNAL(currentIndexChanged(int)), this, SLOT(switchLevel()));
    levelLayout->addWidget(_levelCheckBox);
    QGroupBox *levelGroup = new QGroupBox;
    levelGroup->setTitle("难度选择");
    levelGroup->setLayout(levelLayout);

    _sudoku_board = new SudokuBoard();
    //定义游戏控制按钮
    _resetGameButton = new QPushButton("重置游戏");
    connect(_resetGameButton,SIGNAL(clicked(bool)),_sudoku_board,SLOT(resetData()));
    connect(_resetGameButton, SIGNAL(clicked()), this, SLOT(updateTimer()));
    _startGameButton = new QPushButton(tr("开始游戏"));
    connect(_startGameButton, SIGNAL(clicked()), this, SLOT(start()));
    _submitButon = new QPushButton("完成提交");
    connect(_submitButon,SIGNAL(clicked()),this,SLOT(finished()));
    _clearAnswerButton = new QPushButton("清除答案");
    connect(_clearAnswerButton,SIGNAL(clicked()),this,SLOT(clearAnswer()));
    _suspendButton = new QPushButton("暂停游戏");
     connect(_suspendButton,SIGNAL(clicked()),this,SLOT(stop()));
    _logoutButton = new QPushButton("退出游戏");
    connect(_logoutButton,SIGNAL(clicked()),this,SLOT(logout()));
    //定义游戏控制按钮响应事件



    //设置游戏控制按钮为Group控件,addWidget添加控件
    QGroupBox *buttonsBox = new QGroupBox;
    buttonsBox->setTitle("游戏控制");
    QGridLayout *buttonsLayout = new QGridLayout;
    buttonsLayout->setMargin(0);
    buttonsLayout->addWidget(_startGameButton, 0, 0);
    buttonsLayout->addWidget(_suspendButton, 0, 1);
    buttonsLayout->addWidget(_clearAnswerButton, 1, 0);
    buttonsLayout->addWidget(_submitButon, 1, 1);
    buttonsLayout->addWidget(_resetGameButton, 2, 0);
    buttonsLayout->addWidget(_logoutButton, 2, 1);
    buttonsBox->setLayout(buttonsLayout);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->setMargin(12);      //设置外边距，上下左右相同
    rightLayout->setSpacing(10);     //设置间距
    rightLayout->addWidget(lcdNumberGroup);  //向leftLayout布局中添加计时器控件
    rightLayout->addWidget(createSeperator());
    rightLayout->addWidget(levelGroup);
    rightLayout->addWidget(buttonsBox);      //向leftLayout布局中添加buttonBox布局

    //定义一个水平布局，将数独布局和右边布局添加到一个主布局中
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(_sudoku_board, Qt::AlignJustify);
    mainLayout->addLayout(rightLayout, Qt::AlignRight);

    setLayout(mainLayout);
}

void GameBoard::start()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if(!button){
        return;
    }
    if(button->text() == "再来一局"){
        emit _resetGameButton->clicked();
        _iTimeMinute=0;
        _iTimeSecond=0;
    }
    _pTimer->start();
    _pTimer->setInterval(1000);
    _sudoku_board->setEnabled(true);
    updateButtonStatus();
}

void GameBoard::stop()
{
    _pTimer->stop();
    _startGameButton->setText("继续游戏");
    updateButtonStatus();
    _sudoku_board->setEnabled(false);
}

void GameBoard::finished()
{
    if(_sudoku_board->checkAnswer()){
        QMessageBox::information(this,"过关","恭喜你，答对了！",QMessageBox::Ok);
        _pTimer->stop();
        _startGameButton->setText("再来一局");
        _startGameButton->setEnabled(true);
        updateButtonStatus();
    }else{
        QMessageBox::information(this,"失败","不对哦，再试试看",QMessageBox::Ok);
    }
}

void GameBoard::logout()
{
    if((QMessageBox::information(this,"提示","你确定退出游戏？",QMessageBox::Ok | QMessageBox::Cancel))
            ==QMessageBox::Ok){
        emit gameover();
    }
}

void GameBoard::switchLevel()
{
    int index = _levelCheckBox->currentIndex();
    switch(index) {
    case 0:
        _sudoku_board->setLevel(SudokuBoard::Easy);
        break;
    case 1:
        _sudoku_board->setLevel(SudokuBoard::Middle);
        break;
    case 2:
        _sudoku_board->setLevel(SudokuBoard::Hard);
        break;
    case 3:
        _sudoku_board->setLevel(SudokuBoard::Master);
        break;
    case 4:
        _sudoku_board->setLevel(SudokuBoard::SuperMaster);
        break;
    default:
        break;
    }
}

void GameBoard::clearAnswer()
{
    _sudoku_board->clearAnswer();
}

void GameBoard::updateTimeWidget()
{
    QString strTime = QString(":");
    strTime.push_front(QString(_iTimeMinute%10 + '0'));
    strTime.push_front(QString((_iTimeMinute%100)/10 + '0'));
    strTime.push_back(QString((_iTimeSecond%100)/10 + '0'));
    strTime.push_back(QString(_iTimeSecond%10 + '0'));
    _showTimeWidget->display(QString(strTime));
    _iTimeSecond++;
    if(_iTimeSecond >= 60){
        _iTimeSecond = 0;
        _iTimeMinute++;
        if(_iTimeMinute >= 60){
            _iTimeMinute = 0;
        }
    }
}

void GameBoard::updateButtonStatus()
{
    if(_pTimer->isActive()){
        _startGameButton->setEnabled(false);
        _suspendButton->setEnabled(true);
    }else{
        _startGameButton->setEnabled(true);
        _suspendButton->setEnabled(false);
    }
}

QFrame* GameBoard::createSeperator()
{
    QFrame *splitLine = new QFrame;
    splitLine->setFrameShape(QFrame::HLine);
    splitLine->setFrameShadow(QFrame::Sunken);
    return splitLine;
}

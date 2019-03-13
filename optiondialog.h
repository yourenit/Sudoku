#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

class OptionDialog : public QDialog
{
    Q_OBJECT
public:
    OptionDialog(QWidget* parent=0);
    typedef QVector<QPushButton*> Buttons;

    void accept();
    qint8 selectValue() const;

signals:


public slots:
    void chooseValue();

private:
    void setup();

    Buttons _buttons;
    int _select_value;
    int _temport_value;
};

#endif // OPTIONDIALOG_H

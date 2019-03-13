#ifndef ITEMWIDGET_H
#define ITEMWIDGET_H

#include <QPushButton>
#include <QDialog>
#include "optiondialog.h"

class ItemWidget : public QPushButton
{
public:
    ItemWidget(qint8 num);
    ~ItemWidget();

    void setEditable(bool editable);
    void setValue(qint8 number,bool reset =false);

private:
    void mousePressEvent(QMouseEvent *ev);
    bool is_editable;
};

#endif // ITEMWIDGET_H

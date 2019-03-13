#include "itemwidget.h"

ItemWidget::ItemWidget(qint8 num):is_editable(true)
{
    setMaximumSize(40,40);
    if(num>9 || num<0)
        num=0;
    if(num == 0){
        setEditable(true);
        setText("");
    }else{
        setText(QString::number(num));
        setEditable(false);
    }
    QFont font = this->font();
    font.setBold(true);
    setFont(font);
    adjustSize();
}

ItemWidget::~ItemWidget()
{

}

void ItemWidget::setEditable(bool editable)
{
    is_editable=editable;
    QPalette p=this->palette();
    if(editable)
    {
        setStyleSheet("color: red");
    }else{
        setStyleSheet("color: blue");
    }
    setPalette(p);
}

void ItemWidget::setValue(qint8 number, bool reset)
{
    if(number>9 | number<0) number=0;
    if(number==0){
        setText("");
        setEditable(true);
    }else{
        setText(QString::number(number));
        setEditable(!reset);
    }
}

void ItemWidget::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);

    if(!is_editable){
        return;
    }
    OptionDialog w(this);
    w.setModal(false);
    if(w.exec()==QDialog::Accepted){
        qint8 value =w.selectValue();
        setValue(value);
    }
}

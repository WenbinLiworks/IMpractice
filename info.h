#ifndef INFO_H
#define INFO_H

#include <QWidget>
#include<QMouseEvent>
#include"sysbutton.h"
namespace Ui {
    class Info;
}

class Info : public QWidget
{
    Q_OBJECT

public:
    explicit Info(QWidget *parent = 0);
    void Icon_Init();
    void photo_Init();
    //头像链表
    QStringList headList;                                        
    QStringList photoList;
   // sysButton *btnMin;
    //sysButton *btnMax;
    sysButton *btnClose;
    ~Info();

private:
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
    bool isMove;
    Ui::Info *ui;
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
public slots:
    void showMin();
    void showclose();
    void submit_slot();
    void calcel_slot();
};

#endif // INFO_H

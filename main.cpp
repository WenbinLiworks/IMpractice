#include <QtGui/QApplication>
#include "list_widget.h"
#include<QTextStream>
#include"widget.h"
#include"new_msg.h"
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    // QtSingleApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    QTextCodec *codec=QTextCodec::codecForName("GBK");
     if(codec==NULL)
     codec=QTextCodec::codecForLocale();
     QTextCodec::setCodecForTr(codec);
     QTextCodec::setCodecForLocale(codec);
     QTextCodec::setCodecForCStrings(codec);
   // if(a.isr)

    List_Widget w;
    New_msg tips;
    w.move(1100,50);
    w.show();

    QObject::connect(&w,SIGNAL(initMsg(QString,QString,QString)),&tips,SLOT(setMsg(QString,QString,QString)));

    return a.exec();
}

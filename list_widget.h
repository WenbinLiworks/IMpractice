#ifndef LIST_WIDGET_H
#define LIST_WIDGET_H

#include <QtDesigner/QDesignerExportWidget>
#include<QTreeWidgetItem>
#include<QVBoxLayout>
#include"share.h"
#include<QtNetwork>
#include<QTcpSocket>
#include<titlewidget.h>
#include<QSystemTrayIcon>
#include "skinwidget.h"
#include"info.h"
#include"qplayer.h"
#include<QPalette>
namespace Ui {
    class List_Widget;
}

class List_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit List_Widget(QWidget *parent = 0);

    bool more_flag;//标志其他功能框的标志位
    myPushButton *btnSkin;
    sysButton *btnMenuBar;
    sysButton *btnMin;
    //sysButton *btnMax;
    sysButton *btnClose;
    bool All_flag;
    QSystemTrayIcon *trayicon;
    QString Head_icon;//头像
    QString Myfriend;
    QString Myclassmater;
    QString Myfimily;
    QString Blacklist;
    QString Stranger;
    QString friend_list_msg;
    int Count_online;
    void Init();//初始化函数，用于初始化一些变量；
    titleWidget *Tittle;//标题蓝框架
    QTreeWidget *tree;//窗口父载体
    QTreeWidgetItem *item10;//临时-好友列表
    QTreeWidgetItem *item_class;//临时-同学列表
    QTreeWidgetItem *item_family;//临时-爱人列表
    QTreeWidgetItem *item_black;//临时-黑名单列表
    QTreeWidgetItem *item_stranger;//临时-陌生人列表

    QTreeWidgetItem *item1[100];//临时-黑名单列表
    QTreeWidgetItem *no;
    QString mes_unOpen;
    int item1Count;
    QVBoxLayout *Group_VLayout;

    QString getUserName();//获取主机名称

    QMenu *menu;                                                             //用户 菜单
    QMenu *childMenu;                                                        //子菜单// Right menu
    QPlayer *music_play;
    Info *my_info;

    /*函数名：QString getIp()*/
    /*描述：用于获取本机的IP地址*/
    /*返回值：QString*/
    QString getIP();//获取IP地址

    void newUser(QString userName,QString localHostName,QString ipAddress);//新用户加入
     void Userleave(QString userName,QString localHostName,QString time);//用户离开
    void sendMessage(MessageType type,QString serverAddress="");//发送下消息，
    ~List_Widget();

private:
    QTcpSocket *tcpSocket;//通信协议
    QString bkPicName;//主窗口背景图片的名称
    void paintEvent(QPaintEvent *);
    qint16 port;
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
    bool isMove;
    myPushButton *btnLook;
    QString url;//网址
    Ui::List_Widget *ui;

signals:
    void initMsg(QString title,QString content,QString url);
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

public slots:
    void processPendingDatagrams();//接受UDP数据包信息
    void on_tree_itemDoubleClicked(QTreeWidgetItem*,int);//双击事件响应器
    void Chat_all_slot();
    void showMin();
    void showclose();
    void setPicName(QString);
    void showSkin();//点击换肤按钮
    void show_info();//显示个人信息
    void openWeb(); //网址查看
    void weather();
    void settingslot();
    void music_slot();
    void film_slot();
    void refresh_slot();
    void help_slot();
  //  void trayicon_activated(QSystemTrayIcon::ActivationReason);

};

#endif // LIST_WIDGET_H

#ifndef DIALOG_WIDGET_H
#define DIALOG_WIDGET_H

#include <QWidget>
#include"list_widget.h"
#include"mypushbutton.h"
#include"sysbutton.h"
#include<QHBoxLayout>
#include<QLabel>
#include<QtNetwork>
#include<list_widget.h>
#include<QTextCharFormat>
namespace Ui {
    class dialog_Widget;
}

class dialog_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit dialog_Widget(QString LocalHostName,QString Ip,QString mes,QWidget *parent = 0);
    void loading();//加载初始化配置
    bool font_flag;
    bool font_bold;
    bool font_xie;
    bool font_xian;
    QString get_myIp();
     QString get_myName();//获取主机名称
    QString getMessage();
    QLabel *versionText;
    QLabel *Head_Icon;//头像的Label
    QString user_name;
    QString getName();
    QString IP_addr;
    QString user_photo;
    QString user_icon;
    QTcpServer *TcpServer;
    QTcpSocket *TcpSocket;
    QTcpSocket *clientConnection;

    QUdpSocket *UdpSocket;
    void send_msg(MessageType type);
    void shaking();//屏幕抖动函数
    ~dialog_Widget();


private:
    Ui::dialog_Widget *ui;

        QString bkPicName;//主窗口背景图片的名称
        QPoint normalTopLeft;//正常显示的窗口左上角坐标
        QWidget *topWidget;
        QWidget *icon_Widget;
        QWidget *toolWidget;
        QWidget *showWidget;
        QWidget *text_buttonWidget;
        QWidget *textWidget;

    QColor color;//颜色
    QPoint pressedPoint;//鼠标按下去的点
    bool isMove;
    myPushButton *btnSkin;
    sysButton *btnMenuBar;
    sysButton *btnMin;
    sysButton *btnMax;
    sysButton *btnClose;
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;

     bool saveFile(const QString& fileName);//保存聊天记录
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    bool eventFilter(QObject *target, QEvent *event);//事件过滤器
 public slots:
    void showMax();
    void showMin();
    void msg_recive();
    void send_button_slot();
    void tcpConnected();
    void displayError(QAbstractSocket::SocketError);  //显示错误
  //  void close_button_slot();
    void server_send_msg();
    void show_font_W();
    void on_textUnderline_clicked();
    void on_textcolor_clicked();
    void on_textitalic_clicked();
    void on_textbold_clicked();
    void on_fontComboBox_currentFontChanged(QFont f);
    void on_fontsizecomboBox_currentIndexChanged(QString );
    void currentFormatChanged(const QTextCharFormat &format);
 // void screenshot();//截图
 //void clear_slot();//清屏
 //   void shake_slot();//震动；
    void on_save();
    void refresh_slot();

//    void showSkinWidget();
//    void setPicName(QString);
};

#endif // DIALOG_WIDGET_H

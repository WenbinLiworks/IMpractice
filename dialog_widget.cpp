#include "dialog_widget.h"
#include "ui_dialog_widget.h"
#include<QLabel>
#include<QToolButton>
#include<QMouseEvent>
#include<QDesktopWidget>
#include"share.h"
#include<QMessageBox>
#include<list_widget.h>
#include<QBitmap>
#include<QPainter>
#include<QColorDialog>
#include<QDebug>
#include<fullscreenwidget.h>
#include<QSound>


//别问 问就是不想看
dialog_Widget::dialog_Widget(QString LocalHostName, QString Ip, QString mes, QWidget *parent):
    QWidget(parent),
    ui(new Ui::dialog_Widget)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":img/qq.ico"));
    QPalette palette_main;
    palette_main.setBrush(QPalette::Background,QBrush(QPixmap(":img/blue_BG.jpg")));
    this->setPalette(palette_main);
    this->setAutoFillBackground(true);
    setWindowFlags(Qt::FramelessWindowHint);
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
//    p.setPen(Qt::NoPen);
//    p.setBrush(Qt::black);
    p.setRenderHint(QPainter::Antialiasing);    //抗锯齿
   // p.drawRoundedRect(bmp.rect(), 20, 20); //四个角都是圆弧
    //只要上边角圆弧
    int arcR = 10;    //弧度
    QRect rect = this->rect();
    QPainterPath path;
    //逆时针
    path.moveTo(arcR, 0);
    path.arcTo(0, 0, arcR * 2, arcR * 2, 90.0f, 90.0f);
    path.lineTo(0, rect.height());
    path.lineTo(rect.width(), rect.height());
    path.lineTo(rect.width(), arcR);
    path.arcTo(rect.width() - arcR * 2, 0, arcR * 2, arcR * 2, 0.0f, 90.0f);
    path.lineTo(arcR, 0);
    p.drawPath(path);
    p.fillPath(path, QBrush(Qt::red)); //arm和windows平台没有这行代码将显示一个透明的空空的框
    setMask(bmp);
    //==========实现上边角圆弧==========

    //this->resize(500,500);
   // this->setFixedSize(535,530);
   // this->setWindowFlags(Qt::FramelessWindowHint);
    ui->toolW->setFixedHeight(40);
    QPalette palette;
    palette.setBrush(QPalette::Window,QBrush(QColor(250,255,240,150)));
    ui->dialog_W->setPalette(palette);
    ui->dialog_W->setAutoFillBackground(true);

    IP_addr = Ip;

    user_name = LocalHostName;
   // qDebug()<<"IP dialog"<<Ip<<endl;
    TcpSocket = new QTcpSocket(this);
    TcpServer = new QTcpServer(this);//新的客户端和服务器
    clientConnection = new  QTcpSocket();
    loading();

    QString Icon_path = ":img/n6.png";//默认头像
    ui->Head_label->setPixmap(Icon_path);
    ui->Head_label->setFixedSize(40,40);
    versionText = new QLabel;
    versionText->setText(LocalHostName);//加载用户名

//    ui->radio_COMM->setToolTip("视频会话");
//    ui->camera_COMM->setToolTip("拍照发送");
//    ui->file_COMM->setToolTip("发送文件");
    btnMin=new sysButton("sys_button_min.png",tr("最小化"));
    connect(btnMin,SIGNAL(clicked()),this,SLOT(showMin()));
    btnMax=new sysButton("sys_button_max.png",tr("最大化"));
    btnClose=new sysButton("sys_button_close.png",tr("关闭"));
    connect(btnClose,SIGNAL(clicked()),this,SLOT(close()));

    UdpSocket = new QUdpSocket;

    if ( !UdpSocket->bind(public_port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint ) )
    {
        QMessageBox::critical(0, QString::fromUtf8("错误"), QString::fromUtf8("本地端口连接错误"));
        exit(1);
    }
    connect(UdpSocket,SIGNAL(readyRead()),this,SLOT(msg_recive()));//UDP接受消息

    ui->mainLayout->addWidget(versionText/*,0,Qt::AlignVCenter*/);
    versionText->setContentsMargins(0,0,0,0);
    ui->mainLayout->addStretch();
    ui->mainLayout->addWidget(btnMin);
    ui->mainLayout->addWidget(btnClose);
    ui->mainLayout->setSpacing(0);
    ui->mainLayout->setContentsMargins(0,0,0,0);
    QVBoxLayout *DG_mainLayout = new QVBoxLayout(this);
    //DG_mainLayout->setContentsMargins(0,0,0,0);
    DG_mainLayout->addLayout(ui->mainLayout);
    DG_mainLayout->addWidget(ui->toolW);
    DG_mainLayout->addWidget(ui->dialog_W);
    
    //连接对方作为服务器...连接不上 总是超时 
    //换成本地连接之后出现connection refused
    //connect(TcpSocket,SIGNAL(readyRead()),this,SLOT(msg_recive()));//监听服务端发送的数据  消息由服务端发送？
    TcpSocket->connectToHost(QHostAddress::LocalHost,45454);
    
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(close_button_slot()));//两个按钮的信号槽
    connect(TcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    connect(TcpSocket,SIGNAL(connected()),this,SLOT(tcpConnected()));
    connect(ui->sendButton,SIGNAL(clicked()),this,SLOT(send_button_slot()));
    connect(ui->font_button,SIGNAL(clicked()),this,SLOT(show_font_W()));
    connect(ui->MSG_Edit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(currentFormatChanged(const QTextCharFormat)));
    connect(ui->screen_button,SIGNAL(clicked()),this,SLOT(screenshot()));
    connect(ui->clear_button,SIGNAL(clicked()),this,SLOT(clear_slot()));
    connect(ui->shake_button,SIGNAL(clicked()),this,SLOT(shake_slot()));
 //   connect(ui->textbold,SIGNAL(clicked()),this,SLOT(on_textbold_clicked(bool)));
    connect(ui->info_button,SIGNAL(clicked()),this,SLOT(on_save()));
    connect(ui->color_button,SIGNAL(clicked()),this,SLOT(on_textcolor_clicked()));
    connect(ui->under_line,SIGNAL(clicked()),this,SLOT(on_textUnderline_clicked()));
    connect(ui->textitalic,SIGNAL(clicked()),this,SLOT(on_textitalic_clicked()));
    connect(ui->refresh,SIGNAL(clicked()),this,SLOT(refresh_slot()));


//    if (mes != NULL)
//    {
//        if((size_ip == my_ip) && (send_ip ==IP_addr))//如果收到的信息里面包含的IP地址 ==  我的IP地址 就是说明发给我的，
//        {

//            QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
//            ui->MSG_show->setTextColor(Qt::blue);
//            ui->MSG_show->setCurrentFont(QFont("Times New Roman",12));
//            ui->MSG_show->append("[" + mes+"]"+time);
//            //ui->MSG_show->append(size_msg);
//        }
//    }

}
void dialog_Widget::currentFormatChanged(const QTextCharFormat &format)
{//当编辑器的字体格式改变时，我们让部件状态也随之改变
    ui->fontComboBox->setCurrentFont(format.font());

    if(format.fontPointSize()<9)  //如果字体大小出错，因为我们最小的字体为9
        ui->fontsizecomboBox->setCurrentIndex(3); //即显示12
    else ui->fontsizecomboBox->setCurrentIndex(
            ui->fontsizecomboBox->findText(QString::number(format.fontPointSize())));

    ui->textbold->setChecked(format.font().bold());
    ui->textitalic->setChecked(format.font().italic());
    color = format.foreground().color();
}
void dialog_Widget::displayError(QAbstractSocket::SocketError)
{
    qDebug() <<TcpSocket->errorString(); //输出错误信息  、、、、在这显示网络连接失败
}
void dialog_Widget::tcpConnected()
{
    qDebug()<<"TCPSocket has connected"<<endl;
}
void dialog_Widget::loading()
{
    ui->font_W->hide();//字体编辑框首先隐藏；
    font_flag = false;
    ui->MSG_show->setFixedSize(384,260);
    ui->label_3->setText(user_name);
    ui->label_11->setText(IP_addr);

    ui->sendButton->setShortcut(QKeySequence(Qt::ALT+Qt::Key_S));
    font_bold = true;
    font_xie = true;
    font_xian = true;
    //ui->sendButton->setShortcut(QKeySequence(Qt::Key_Return),this);
    QSettings settings("info.ini", QSettings::IniFormat);

    user_photo = settings.value("infomation/User_photo").toString();
    QPixmap map;
    map.load(user_photo);
     ui->label_13->setPixmap(map);
//    user_icon = settings.value("infomation/User_icon").toString();
//    QPixmap map_ico;
//    map_ico.load(user_icon);
//    ui->Head_label->setPixmap(map_ico);


}
void dialog_Widget::show_font_W()
{
    if(false == font_flag)
    {
        ui->font_W->show();
        ui->MSG_show->setFixedSize(384,230);
        font_flag = true;
    }
    else
    {
        ui->font_W->hide();
        ui->MSG_show->setFixedSize(384,260);
        font_flag = false;
    }
}
void dialog_Widget::screenshot()
{
    fullScreenWidget *fullWidget  =  new fullScreenWidget();
      QPixmap pixmap = fullWidget->getFullScreenPixmap();
      fullWidget->show();
      fullWidget->loadBackgroundPixmap(pixmap);
}
void dialog_Widget::close_button_slot()
{
    TcpSocket->close();
    this->close();
}


QString dialog_Widget::getName()
{
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";
    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, envVariables)
    {
        int index = environment.indexOf(QRegExp(string));
        if (index != -1)
        {

            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2)
            {
                return stringList.at(1);
                break;
            }
        }
    }
    return false;
}




void dialog_Widget::msg_recive()
{
    while(UdpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(UdpSocket->pendingDatagramSize( ) );
        UdpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_4_6);
        QString size_name;
        QString size_msg;
        QString send_name;
        QString send_ip;
        QString size_ip;
        QString my_ip = get_myIp();
        int type;
        in>>type;
        switch(type)
        {
        case Message:
            {

                in>>size_name>>size_msg>>size_ip>>send_name>>send_ip;
                if((size_ip == my_ip) && (send_ip ==IP_addr))//如果收到的信息里面包含的IP地址 ==  我的IP地址 就是说明发给我的，
                {

                    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
                    ui->MSG_show->setTextColor(Qt::blue);
                    ui->MSG_show->setCurrentFont(QFont("Times New Roman",12));
                    ui->MSG_show->append("[" + send_name+"]"+time);
                    ui->MSG_show->append(size_msg);
                    QSound player("E:\msg.wav");
                    player.play();


                }
                break;
            case Shake:
                {
                    QSound player("E:msg.wav");
                    player.play();//发出好友上线提示音
                   //  shaking();
                   // QString RPG = "【系统提示】：对方给您发送了窗口抖动";
                    ui->MSG_show->setTextColor(Qt::gray);
                    ui->MSG_show->append(RPG);
                    break;
                }
            }
        default:
            {
                break;
            }
        }



    }
}

/*以上都不是北理工写的*/

QString dialog_Widget::get_myIp()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol) 
            return address.toString();
    }
       return 0;
}
/*获取信息全部按照同样格式仿写*/
QString dialog_Widget::get_myName()
{
    
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";
    
    QStringList environment = QProcess::systemEnvironment();
    
    foreach (QString string, envVariables)
    {
        int index = environment.indexOf(QRegExp(string));
        if (index != -1)
        {

            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2)
            {
                return stringList.at(1);
                break;
            }
        }
    }
    return false;
}

/*从这里开始*/

QString dialog_Widget::getMessage()
{
    QString msg = ui->MSG_Edit->toHtml();

    ui->MSG_Edit->clear();
    ui->MSG_Edit->setFocus();
    return msg;
}
void dialog_Widget::send_button_slot()
{
    send_msg(Message);
}
void dialog_Widget::send_msg(MessageType type)
{
    //TcpSocket->abort();//取消已有的连接
    switch(type)
    {
    case Message:
        {
            if(ui->MSG_Edit->toPlainText() == "")
            {
                QMessageBox::warning(0,tr("警告"),tr("发送内容不能为空"),QMessageBox::Ok);
                return;
            }
            QString my_name = get_myName();
            QString msg_send = getMessage();
            QString send_my_ip = get_myIp();
            QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
            ui->MSG_show->setTextColor(Qt::green);
            ui->MSG_show->setCurrentFont(QFont("Times New Roman",12));
            ui->MSG_show->append("[" + my_name+"]"+time);
            ui->MSG_show->append(msg_send);


            QByteArray data;
            QDataStream out(&data, QIODevice::WriteOnly);

            out<<type<<user_name<<msg_send<<IP_addr<<my_name<<send_my_ip;//IP_addr  是接受方的IP，send_myIp是发送方的ip
            //qDebug()<<"IP send"<<IP_addr<<endl;
            UdpSocket->writeDatagram(data, QHostAddress::Broadcast,public_port);//这样就把名字和消息 发送走了？
            UdpSocket->writeDatagram(data,QHostAddress::Broadcast,SHAREPORT);
            break;

        }
    /*case Shake:
        {
            QString my_name = get_myName();
            QString send_my_ip = get_myIp();
            QByteArray data;
            QDataStream out(&data, QIODevice::WriteOnly);
            out<<type<<user_name<<IP_addr<<my_name<<send_my_ip;
            UdpSocket->writeDatagram(data, QHostAddress::Broadcast,public_port);//这样就把名字和消息 发送走了？
            UdpSocket->writeDatagram(data,QHostAddress::Broadcast,SHAREPORT);

        }
     */
    default:
        break;
    }


}
/*
void dialog_Widget::shaking()
{
    QPoint p = this->pos();
    int i = p.x();
    int j = p.y();
     for(int k=1;k<1001;k++)
     {
         if(0 == k%2)
         {
              this->move(i,j);
         }
           this->move(i+50,j-50);
     }

}
void dialog_Widget::shake_slot()
{
    shaking();//自己先抖 然后把这样的一个信息发给别人
    send_msg(Shake);

}
*/
void dialog_Widget::server_send_msg()
{
  //  ui->sendButton->setEnabled(true);
    //ui->net_label->setText(tr("与对方通信连接成功"));

    //进来之后 说明 通信已经建立 就等待发送按钮  按下
}

void dialog_Widget::mousePressEvent(QMouseEvent *e)
{

     this->windowPos = this->pos();
     this->mousePos = e->globalPos();
     this->dPos = mousePos - windowPos;
}

void dialog_Widget::mouseMoveEvent(QMouseEvent *e)
{

    this->move(e->globalPos() - this->dPos);
}
void dialog_Widget::mouseReleaseEvent(QMouseEvent *e)
{
    if(isMove)
        isMove=false;
}
void dialog_Widget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->x()+170>=this->width())
        return;
    emit showMax();
}
void dialog_Widget::showMax()
{
//    static bool isMax=false;
//    if(isMax)
//    {
//        move(normalTopLeft);
//        resize(850,600);
//        isMax=false;
//    }
//    else
//    {
//        normalTopLeft=this->pos();
//        this->setGeometry(QApplication::desktop()->availableGeometry());
//        isMax=true;
//    }
}
/*
void dialog_Widget::showMin()
{
    qDebug()<<"zuixiao"<<endl;
    this->showMinimized();
}
*/

void dialog_Widget::on_fontComboBox_currentFontChanged(QFont f)//字体设置
{
    ui->MSG_Edit->setCurrentFont(f);
    ui->MSG_Edit->setFocus();
}

void dialog_Widget::on_fontsizecomboBox_currentIndexChanged(QString size)
{
   ui->MSG_Edit->setFontPointSize(size.toDouble());
   ui->MSG_Edit->setFocus();
}

void dialog_Widget::on_textbold_clicked()
{
    if(true == font_bold)
    {
         ui->MSG_Edit->setFontWeight(QFont::Bold);
         font_bold = false;
    }

    else
    {
        ui->MSG_Edit->setFontWeight(QFont::Normal);
        ui->MSG_Edit->setFocus();
        font_bold =  true;
    }

}
//刷新slot
void dialog_Widget::refresh_slot()
{
    QSettings settings("info.ini", QSettings::IniFormat);

    user_photo = settings.value("infomation/User_photo").toString();
    QPixmap map;
    map.load(user_photo);

    //background-image: url(:/img/boy1.png);
    ui->label_13->setPixmap(map);

}
void dialog_Widget::on_textitalic_clicked()
{
    if(true == font_xie)
    {
        ui->MSG_Edit->setFontItalic(font_xie);
        qDebug()<<"xienengjima"<<endl;
        ui->MSG_Edit->setFocus();
        //font_xie = false;

    }
    if(false == font_xie)
    {
        ui->MSG_Edit->setFontItalic(font_xie);
        
        //qDebug()？
        
        ui->MSG_Edit->setFocus();
        //font_xie = true;
    }

}

void dialog_Widget::on_textUnderline_clicked()
{
    if(false == font_xian )
    {
        ui->MSG_Edit->setFontUnderline(font_xian);
        ui->MSG_Edit->setFocus();
        font_xian = true;
    }
    if(true == font_xian)
    {
        ui->MSG_Edit->setFontUnderline(font_xian);
        ui->MSG_Edit->setFocus();
        font_xian = false;

    }

}
void dialog_Widget::on_textcolor_clicked()
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid())
    {
        ui->MSG_Edit->setTextColor(color);
        ui->MSG_Edit->setFocus();
    }
}
/*到这里为止 北理工修改*/

/*以下为北理工自创*/

//保存聊天记录
void dialog_Widget::on_save()
{
    if(ui->MSG_show->document()->isEmpty())
        QMessageBox::warning(0,tr("警告"),tr("聊天记录为空，无法保存！"),QMessageBox::Ok);
    else
    {
       //获得文件名
       QString fileName = QFileDialog::getSaveFileName(this,tr("保存聊天记录"),tr("聊天记录"),tr("文本(*.txt);;All File(*.*)"));
       if(!fileName.isEmpty())
           saveFile(fileName);
    }
}

bool dialog_Widget::saveFile(const QString &fileName)//保存文件
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))

    {
        QMessageBox::warning(this,tr("保存文件"),
        tr("无法保存文件 %1:\n %2").arg(fileName)
        .arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    out << ui->MSG_show->toPlainText();

    return true;
}

void dialog_Widget::clear_slot()
{
    ui->MSG_show->clear();
}
bool dialog_Widget::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->MSG_Edit)
    {
        if(event->type() == QEvent::KeyPress)
        {
             QKeyEvent *k = static_cast<QKeyEvent *>(event);
             if(k->key() == Qt::Key_Return)
             {
                 qDebug()<<"press"<<endl;
                 send_button_slot();
                 return true;
             }
        }
    }
    return QWidget::eventFilter(target,event);
}
dialog_Widget::~dialog_Widget()
{
    delete ui;
}

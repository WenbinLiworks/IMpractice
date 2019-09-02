#include "list_widget.h"
#include "ui_list_widget.h"
#include<QVBoxLayout>
#include<QMessageBox>
#include<QTableWidgetItem>
#include<QDebug>
#include"dialog_widget.h"
#include"widget.h"
#include<QObject>
#include<QDialog>
#include"share.h"
#include<QSound>
#include"videoplayer.h"
#include"qplayer.h"
#include"help.h"

List_Widget::List_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::List_Widget)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":img/qq.ico"));
   // this->setWindowFlags(Qt::FramelessWindowHint);//不显示框架
    //this->setWindowFlags(this->windowFlags()&Qt::WindowMaximizeButtonHint&Qt::WindowMinimizeButtonHint);

//    QPixmap pixmap(":img/pinkLove.jpg");
//    setMask(pixmap.mask());
//    QPalette palette;
//    palette.setBrush(QPalette::Background, QBrush( pixmap ) );
//    setPalette(palette);
//    resize( pixmap.size() );
//    setMask(pixmap.mask());


//    setAttribute(Qt::WA_TranslucentBackground, true);
//        setWindowFlags(Qt::Window | Qt::FramelessWindowHint
//                       | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint
//                       | Qt::WindowMaximizeButtonHint);
    bkPicName=":/img/skin/pinkLove.jpg";
        //==========实现上边角圆弧==========
    
        setWindowFlags(Qt::FramelessWindowHint);
        QBitmap bmp(this->size());
        bmp.fill();
        QPainter p(&bmp);
    //    p.setPen(Qt::NoPen);
    //    p.setBrush(Qt::black);
        p.setRenderHint(QPainter::Antialiasing);    //抗锯齿
        //p.drawRoundedRect(bmp.rect(), 20, 20); //四个角都是圆弧
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

   // btnSkin=new myPushButton("SkinButton.png",tr("换肤"));
    connect(btnSkin,SIGNAL(clicked()),this,SLOT(showSkin()));
    //btnMin=new sysButton("sys_button_min.png",tr("最小化"));
    //connect(btnMin,SIGNAL(clicked()),this,SIGNAL(showMin()));
   // connect(btnMin,SIGNAL(clicked()),this,SLOT(showMin()));
    
    btnClose=new sysButton("sys_button_close.png",tr("关闭"));
    
    connect(btnClose,SIGNAL(clicked()),this,SLOT(showclose()));//关闭 要发送离开信息！
    
    // connect(ui->web_button,SIGNAL(clicked()),this,SLOT(openWeb())); //查看网页
    
    ui->Mian_Layout->addStretch();
    ui->Mian_Layout->addWidget(btnSkin,0,Qt::AlignVCenter);
    btnSkin->setContentsMargins(0,0,5,0);
 //   ui->Mian_Layout->addWidget(btnMin);
    ui->Mian_Layout->addWidget(btnClose);
    ui->Mian_Layout->setSpacing(0);
    ui->Mian_Layout->setContentsMargins(0,0,0,0);


    Init();//初始化一些变量

     //头标的大小和初始化
    ui->Head_button->setIcon(QIcon(Head_icon));
    ui->Head_button->setIconSize(QSize(80,80));

    //friendlist  列表中定义纵向布局；
    Group_VLayout = new QVBoxLayout(ui->friend_list);

    //使用QTreeWidget控件生成分组列表
    item10 = new QTreeWidgetItem();
    
    //好友
    item10->setText(0,Myfriend);//添加列表1；
    //connect(item10,SIGNAL)
   // no = new QTreeWidgetItem();
    item_class = new QTreeWidgetItem();
    
    //同学
    item_class->setText(0,Myclassmater);//添加列表1
    //item_class->addChild(no);
    item_family = new QTreeWidgetItem();
    
    item_family->setText(0,Myfimily);//添加列表1
    //item_family->addChild(no);
    
    //黑名单
    item_black = new QTreeWidgetItem();
    item_black->setText(0,Blacklist);//添加列表1
    //item_black->addChild(no);
  
    //陌生人
    item_stranger = new QTreeWidgetItem();
    item_stranger->setText(0,Stranger);//添加列表1
   // item_stranger->addChild(no);

    //
    ui->treeW->installEventFilter(this);
    ui->treeW->headerItem()->setHidden(true);


    //从顶层开始插入节点
    ui->treeW->addTopLevelItem(item_class);
    ui->treeW->addTopLevelItem(item_family);
    ui->treeW->addTopLevelItem(item_stranger);
    ui->treeW->addTopLevelItem(item_black);

    
     udpSocket = new QUdpSocket(this);

    udpSocket->bind(public_port,QUdpsocket::ShareAddress | QUdpSocket::ReuseAddressHint);//绑定预定端口

    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));//检测到端口消息，并处理

    //作为新用户，广播自己的资料
    sendMessage(NewParticipant);

    connect(ui->treeW,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this,SLOT(on_tree_itemDoubleClicked(QTreeWidgetItem*,int)));

    connect(ui->Chat_All,SIGNAL(clicked()),this,SLOT(Chat_all_slot()));

    connect(ui->Head_button,SIGNAL(clicked()),this,SLOT(show_info()));
    // connect(ui->weatherButton,SIGNAL(clicked()),SLOT(weather()));
  
    //设置
    connect(ui->settingbutton,SIGNAL(clicked()),this,SLOT(settingslot()));
    
   // connect(ui->other_button,SIGNAL(clicked()),this,SLOT(film_slot()));
    
    //音乐
    connect(ui->music_button,SIGNAL(clicked()),this,SLOT(music_slot()));
  
    //刷新
    connect(ui->fresh,SIGNAL(clicked()),this,SLOT(refresh_slot()));
   
    //帮助
    connect(ui->helpbutton,SIGNAL(clicked()),this,SLOT(help_slot()));
}
   

void List_Widget::Init()
{
    more_flag  =true;
    Myfriend = "我的好友";
    Myclassmater = "我的同学(0)";
    Myfimily = "我的家人(0)";
    Blacklist = "黑名单(0)";
    Stranger  = "陌生人(0)";

    Count_online = 0;//在线人数初始化为0；
    item1Count = 0;//好友个数初始化为0；最多100；
    port = xxxxx;//预定端口
    this->setFixedSize(210,550);//固定大小

    //color
    QPalette palette_list;
    
    palette_list.setBrush(QPalette::Window,QBrush(QColor(210,155,140,50)));
    
    ui->treeW->setPalette(palette_list);
    ui->treeW->setAutoFillBackground(true);
    
    All_flag = true;
    
    my_info = new Info(); 
    
    music_play = new QPlayer();//申请音乐播放器的内存

    //设置
    QSettings settings("info.ini", QSettings::IniFormat);
   
    int mainFun = settings.value("DevOption/mainFun").toInt();
    
    QString name_ini = settings.value("infomation/User_name").toString();
    QString sign_ini = settings.value("infomation/User_sign").toString();

    //头像
    Head_icon = settings.value("infomation/User_icon").toString();
  
    //个签
    ui->namelabel->setText(name_ini);
    
  // ui->sign->setText(sign_ini);
    
  //  ui->otherW->hide();//qelse功能框架隐藏

}

/*函数名：QStrig getUserName()*/   //getUserName位()于widget.h
/*描述：用于获取主机名称*/
/*返回值：QString*/


//刷新 
void List_Widget::refresh_slot()
{
    QSettings settings("info.ini", QSettings::IniFormat);
//    int mainFun = settings.value("DevOption/mainFun").toInt();
    QString name_ini = settings.value("infomation/User_name").toString();
    QString sign_ini = settings.value("infomation/User_sign").toString();

    Head_icon = settings.value("infomation/User_icon").toString();

    //头标的大小和初始化
    ui->namelabel->setText(name_ini);
    ui->sign->setText(sign_ini);
    ui->Head_button->setIcon(QIcon(Head_icon));
    ui->Head_button->setIconSize(QSize(80,80));
}

QString List_Widget::getUserName()
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
//获取ip
QString List_Widget::getIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol) //我们使用IPv4地址
            return address.toString();
    }
       return 0;
}

//新用户
void List_Widget::newUser(QString userName, QString localHostName, QString ipAddress)
{
    QIcon icon(":img/qq.ico");//初始化头标
    int index_search = 0;
    QString user_msg = userName+'('+localHostName+')'+'\n'+ipAddress;
    QString user_info = userName+'('+localHostName+')';
    QString tips_msg = user_msg + '\n'+"上线";//上线

    /*自己上线*/
    if(0 == item1Count)
    {
        Count_online++; //在线人数增加
        
        ui->Online_count->setText(QString::number(Count_online,10));//更新好友在线数量

        //写入新用户信息
        item1[item1Count]=new QTreeWidgetItem();
        item1[item1Count]->setIcon(0,icon);
        item1[item1Count]->setText(0,user_msg);
        item1[item1Count]->setText(1,ipAddress);
        item1[item1Count]->setText(2,user_info);
        item10->addChild(item1[item1Count]);
        item1Count++;//累加

        qDebug()<<"NEw_name"<<user_msg/*item1[item1Count]->text(0)*/<<endl;

       //额外定义一个变量用于各个分组在线人数计数
        QString friend_list_msg = Myfriend+'('+QString::number(Count_online,10)+')';
        
        item10->setText(0,friend_list_msg);//添加列表1；
        
        /*发送新用户登录信息*/
       /*否则新加入的端点无法获得已在线端点信息*/
        sendMessage(NewParticipant);
    }

    else
    {
        for(int j=0;j<item1Count;j++)
        {

            if(user_msg == item1[j]->text(0))
            {
               index_search = 1;
            }
        }
        
        
  /*设置好友上限为100 因为用了Udp*/
  
        //用户好友个数未达到上限 且没有重复的
        if((item1Count < 100) && (0 == index_search))
        {
            Count_online++;
            ui->Online_count->setText(QString::number(Count_online,10));//及时更新好友在线数量
            item1[item1Count]=new QTreeWidgetItem();
            item1[item1Count]->setIcon(0,icon);
            //写入新用户信息
            item1[item1Count]->setText(0,user_msg) ;
            item1[item1Count]->setText(1,ipAddress);
            item1[item1Count]->setText(2,user_info);
            
            //QWidgetTree用法，增加子节点
            item10->addChild(item1[item1Count]);
            item1Count++;//累加

            qDebug()<<"NEw_name"<<user_msg/*item1[item1Count]->text(0)*/<<endl;

            //额外定义变量用于个分组在线人数计数
            friend_list_msg = Myfriend+'('+QString::number(Count_online,10)+')';
            item10->setText(0,friend_list_msg);//添加列表1；
        
            //发射提示信号，提示新用户上线；
           emit initMsg(userName,tips_msg,ipAddress);
            
           QSound player("");
           
           player.play();//发出好友上线提示音

          //同上
            sendMessage(NewParticipant);

        }

        else if((item1Count >=100) && (0 == index_search))
        {
            QMessageBox::warning(0,tr("警告"),tr("您好友数量已达上限！无法增加新用户！"),QMessageBox::Ok);
            return;
        }
       
        else if((item1Count <100) && (1 == index_search))//重复添加同一个用户
        {
            return;
        }

    }



}

void List_Widget::Userleave(QString userName, QString localHostName, QString ipAddress)
{
     QString down = "已经下线";

     QString user_msg = '('+localHostName+')'+'\n'+ipAddress + '\n'+ down;

     //ui->treeW->findItems(localHostName,Qt::MatchExactly).first()->removeChild();//删除 离开的好友
     
      Count_online--;
      //额外定义变量用于个分组在线人数计数
     friend_list_msg = Myfriend+'('+QString::number(Count_online,10)+')';

     //及时更新好友在线数量
     ui->Online_count->setText(QString::number(Count_online,10));
     //treeW减去下线的节点
     QTreeWidgetItem *tmp_item =  
             ui->treeW->findItems(user_msg,Qt::MatchExactly,0).first()-;
             ui->treeW->findItems(user_msg,Qt::MatchExactly).first();
             item10->removeChild( ui->treeW->findItems(user_msg,Qt::MatchExactly));
             //qDebug()<<tmp_item->text(0);
     
   //建立remove列 这里胡写的 API想不出来
  item10->setText(0,friend_list_msg);//添加列表1；
  item10->removeChild(item10->child(1);
     
     //QSound leave_player("");
     //leave_player.play();
  emit initMsg(userName,user_msg,ipAddress);
      //QDialog::accept();
  item1Count--;
}

void List_Widget::processPendingDatagrams()
{

    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int messageType;
        //ut<<type<<user_name<<msg_send<<IP_addr<<my_name<<send_my_ip;
        in >> messageType;
        QString userName,localHostName,ipAddress,message;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        //in >>userName >>mes_unOpen;
        switch(messageType)
        {
            case Message:
                {
                    qDebug()<<"大家好 我还没打开对话框 我能在这接收到消息吗"<<endl;
                    break;
                }
            case NewParticipant:
                {
                    in >>userName >>localHostName >>ipAddress;
                    newUser(userName,localHostName,ipAddress);

                    break;
                }
            case ParticipantLeft:
                {
                    in >>userName >>localHostName;
                    Userleave(userName,localHostName,ipAddress);
                    break;
                }
        case Shake:
            {

            }
        case Refuse:
            {
//                in >> userName >> localHostName;
//                QString serverAddress;
//                in >> serverAddress;
//                QString ipAddress = getIP();

//                if(ipAddress == serverAddress)
//                {
//                    server->refused();
//                }
//                break;
            }
        }
    }
}

void List_Widget::sendMessage(MessageType type, QString serverAddress)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString localHostName = QHostInfo::localHostName();
    QString address = getIP();
    out << type << getUserName() << localHostName;


    switch(type)
    {
        case ParticipantLeft:
            {

                break;
            }
        case NewParticipant:
            {
                out << address;
                break;
            }

        case Message :
            {


            }
        case FileName:
            {

            }
        case Refuse:
            {

            }
    }
    qDebug()<<"是不是没有发送？？"<<endl;
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast, public_port);//搜集数据 发送
}

void List_Widget::on_tree_itemDoubleClicked(QTreeWidgetItem *item, int a)
{

    dialog_Widget *chatdialog=new dialog_Widget(item->text(2),item->text(1),mes_unOpen);


    chatdialog->move(400,50);
    chatdialog->show();

}

void List_Widget::settingslot()
{
    my_info->move(700,50);
    my_info->show();
}

void List_Widget::Chat_all_slot()
{
     Widget *w = new Widget();
    if(true == All_flag)
    {
        //ui->Chat_All->setText(tr("聊天室开启中.."));
        w->show();
        All_flag = false;
    }

   else
   {
      // ui->Chat_All->setText(tr("开启聊天室"));
       w->hide();
       All_flag = true;
   }
}

void List_Widget::paintEvent(QPaintEvent *)
{
    QBitmap bitmap(this->size());
    bitmap.fill();
    QPainter painter(&bitmap);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRoundedRect(bitmap.rect(),5,5);
    setMask(bitmap);

    QPixmap pixmap(this->size());
    pixmap.fill();
    painter.end();
    painter.begin(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing,true);
    QBrush brush;
    brush.setStyle(Qt::TexturePattern);
    brush.setTextureImage(QImage(bkPicName));
    painter.setBrush(brush);
    painter.setPen(Qt::black);
    painter.drawRoundedRect(rect(),5,5);
    painter.end();
    painter.begin(this);
    painter.drawPixmap(this->rect(),pixmap);
}
void List_Widget::showSkin()
{
    skinWidget *skinW=new skinWidget(bkPicName);
    skinW->setAttribute(Qt::WA_DeleteOnClose);
    connect(skinW,SIGNAL(changeSkin(QString)),this,SLOT(setPicName(QString)));
    QPoint p=rect().topRight();
    p.setX(p.x()-150-350);
    p.setY(p.y()+30);
    skinW->move(this->mapToGlobal(p));
    skinW->show();
}
void List_Widget::show_info()
{
    my_info->move(700,50);
    my_info->show();
}
void List_Widget::music_slot()
{

    music_play->show();
}
void List_Widget::film_slot()
{
    videoPlayer *new_film = new videoPlayer();
    new_film->show();
}
void List_Widget::setPicName(QString picName)
{
    bkPicName=picName;
    update();
}
void List_Widget::showMin()
{
    showMinimized();
}
void List_Widget::showclose()
{
    this->close();
    sendMessage(ParticipantLeft);
    //send  user leave msg
}

void List_Widget::mousePressEvent(QMouseEvent *e)
{

     this->windowPos = this->pos();
     this->mousePos = e->globalPos();
     this->dPos = mousePos - windowPos;
}

void List_Widget::mouseMoveEvent(QMouseEvent *e)
{

    this->move(e->globalPos() - this->dPos);
}
void List_Widget::mouseReleaseEvent(QMouseEvent *e)
{
    if(isMove)
        isMove=false;
}
/*
void List_Widget::openWeb()
{
    url = ui->lineEdit->text();
    QDesktopServices::openUrl(QUrl(url));
}
/*
void List_Widget::weather()
{
    QString weather_url = "www.weather.com.cn";
    QDesktopServices::openUrl(QUrl(weather_url));
}
*/
void List_Widget::help_slot()
{
    Help *help_tmp = new Help();
    help_tmp->show();
}
List_Widget::~List_Widget()
{
    delete ui;
}

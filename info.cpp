#include "info.h"
#include "ui_info.h"
#include<QBitmap>
#include<QPainter>
#include<QSettings>
#include<QMessageBox>
#include<QDebug>
#include<QDir>
Info::Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":img/qq.ico"));
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
    btnMin=new sysButton("sys_button_min.png",tr("最小化"));
    connect(btnMin,SIGNAL(clicked()),this,SLOT(showMin()));
    btnClose=new sysButton("sys_button_close.png",tr("关闭"));
    connect(btnClose,SIGNAL(clicked()),this,SLOT(showclose()));//关闭 记者要发送离开信息

    ui->Mian_Layout->addStretch();
    ui->Mian_Layout->addWidget(btnMin);
    ui->Mian_Layout->addWidget(btnClose);
    ui->Mian_Layout->setSpacing(0);
    ui->Mian_Layout->setContentsMargins(0,0,0,0);
    connect(ui->submit,SIGNAL(clicked()),this,SLOT(submit_slot()));
    connect(ui->calcel,SIGNAL(clicked()),this,SLOT(calcel_slot()));

    QSettings settings("info.ini", QSettings::IniFormat); // 当前目录的INI文件
    ui->name->setText(settings.value("infomation/User_name").toString());
    ui->sign->setText(settings.value("infomation/User_sign").toString());
    Icon_Init();
    photo_Init();



    //QString tmp_name = ;
    qDebug()<<"here"<<ui->Head_box->currentIndex()<<endl;
}
void Info::photo_Init()
{
    QSettings settings("info.ini", QSettings::IniFormat); // 当前目录的INI文件
    QDir dir(":img/xingxiang");//设置默认路径
    if (!dir.isReadable())                                                //是否该路径可读
    {
        QMessageBox::critical(0, QString::fromUtf8("错误"), QString::fromUtf8("头像文件路径不正确"));
    }
    int myHeadIndex = 0;
    photoList = dir.entryList(QDir::Files);                                //获得文件夹的所有文件
    QString now_photo = settings.value("information/User_photo").toString();

    for (int i=0; i<photoList.count(); i++)
    {
            QString fileName = dir.filePath(photoList.at(i));                  //设置文件相对路径名
            if (fileName == now_photo)                                           //记录当前用户头像索引
            {
                    myHeadIndex = i;
            }

            ui->photo_box->addItem(QIcon(fileName), NULL);                          //添加进列表框
    }

    ui->photo_box->setCurrentIndex(myHeadIndex);                                //显示当前本地头像

}
void Info::Icon_Init()
{
    QSettings settings("info.ini", QSettings::IniFormat); // 当前目录的INI文件
    QDir dir(":img/head_icon");//设置默认路径
    if (!dir.isReadable())                                                //是否该路径可读
    {
        QMessageBox::critical(0, QString::fromUtf8("错误"), QString::fromUtf8("头像文件路径不正确"));
    }
    int myHeadIndex = 0;
    headList = dir.entryList(QDir::Files);                                //获得文件夹的所有文件
    QString now_head_icon = settings.value("information/User_icon").toString();

    for (int i=0; i<headList.count(); i++)
    {
            QString fileName = dir.filePath(headList.at(i));                  //设置文件相对路径名
            if (fileName == now_head_icon)                                           //记录当前用户头像索引
            {
                    myHeadIndex = i;
            }

            ui->Head_box->addItem(QIcon(fileName), NULL);                          //添加进列表框
    }

    ui->Head_box->setCurrentIndex(myHeadIndex);                                //显示当前本地头像

}
void Info::submit_slot()
{
    QSettings settings("info.ini", QSettings::IniFormat); // 当前目录的INI文件
    QString name_ini = ui->name->text();
    QString sign_ini = ui->sign->text();
    settings.beginGroup("infomation");
    settings.setValue("User_name",name_ini);
    settings.setValue("User_sign",sign_ini);//存储到配置文件中
    QDir dir(":img/head_icon");
    if (!dir.isReadable())                                                //是否该路径可读
    {
        QMessageBox::critical(0, QString::fromUtf8("错误"), QString::fromUtf8("头像文件路径不正确"));
    }

    int index = ui->Head_box->currentIndex();                                 //获得选择头像索引
    QString head = dir.filePath(headList.at(index));
    settings.setValue("User_icon",head);

    QDir dir_photo(":img/xingxiang");
    if(!dir_photo.isReadable())
    {
        QMessageBox::critical(0, QString::fromUtf8("错误"), QString::fromUtf8("头像文件路径不正确"));
    }
    int photo_index = ui->photo_box->currentIndex();
    QString photo = dir_photo.filePath(photoList.at(photo_index));
    settings.setValue("User_photo",photo);//加载用户形象照片
    QSettings check("info.ini", QSettings::IniFormat);//检查加载是否成功
    QString name_che = check.value("infomation/User_name").toString();
    QString sign_che = check.value("infomation/User_sign").toString();
    QString icon_che = check.value("infomation/User_icon").toString();
    QString photo_che = check.value("infomation/User_photo").toString();

    if((name_che == name_ini ) && (sign_che == sign_ini) && (icon_che == head) && (photo_che == photo))
    {
        QMessageBox::information(0,"恭喜","信息存已成功导入，下次启动有效");
    }
    else
    {
         QMessageBox::information(0,"错误","信息存入失败");
    }
    this->close();
}
void Info::calcel_slot()
{
    this->close();
}
Info::~Info()
{
    delete ui;
}
void Info::mousePressEvent(QMouseEvent *e)
{

     this->windowPos = this->pos();
     this->mousePos = e->globalPos();
     this->dPos = mousePos - windowPos;
}

void Info::mouseMoveEvent(QMouseEvent *e)
{

    this->move(e->globalPos() - this->dPos);
}
void Info::mouseReleaseEvent(QMouseEvent *e)
{
    if(isMove)
        isMove=false;
}
void Info::showMin()
{
    showMinimized();
}
void Info::showclose()
{
    this->close();
}

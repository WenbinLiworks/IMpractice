#include "widget.h"
#include "ui_widget.h"
#include"list_widget.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    //初始化
    ui->setupUi(this);
    ui->label->setText(" ");
    this->resize(850,550);
    ui->textEdit->setFocusPolicy(Qt::StrongFocus);
    ui->textBrowser->setFocusPolicy(Qt::NoFocus);
    this->setWindowIcon(QIcon(":img/qq.ico"));
    ui->textEdit->setFocus();
    ui->textEdit->installEventFilter(this);

    udpAllSocket = new QUdpSocket(this);
    port = 45450;
    
    udpAllSocket->bind(port,QUdpSocket::ShareAddress
                                    | QUdpSocket::ReuseAddressHint);
    
    connect(udpAllSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
    
    sendMessage(all_new_user);

    server = new TcpServer(this);
   
    //文件传输
    connect(server,SIGNAL(sendFileName(QString)),this,SLOT(sentFileName(QString)));
    
    //？？？
    connect(ui->textEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(currentFormatChanged(const QTextCharFormat)));
}

//当编辑器的字体格式改变时，让部件状态也随之改变
void Widget::currentFormatChanged(const QTextCharFormat &format)
{
    ui->fontComboBox->setCurrentFont(format.font());

    //最小的字体为9 小于则调整，显示12
    if(format.fontPointSize()<9)  
        ui->fontsizecomboBox->setCurrentIndex(3); 
    
    else ui->fontsizecomboBox->setCurrentIndex(
            ui->fontsizecomboBox->findText(QString::number(format.fontPointSize())));

    ui->textbold->setChecked(format.font().bold());
    
    ui->textitalic->setChecked(format.font().italic());
    
    ui->textUnderline->setChecked(format.font().underline());
    
    color = format.foreground().color();
}

//接收数据UDP
//Slot:ProcessPendingDatagrams
void Widget::processPendingDatagrams()   
{
    while(udpAllSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpAllSocket->pendingDatagramSize());
        udpAllSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        
        int messageType;
        in >> messageType;
        
        QString userName,localHostName,ipAddress,message;
        
        //得到即时时间
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        
        switch(messageType)
        {
            case all_msg:
                {
                    in >>userName >>localHostName >>ipAddress >>message;
                    ui->textBrowser->setTextColor(Qt::blue);
                    ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
                    ui->textBrowser->append("[ " +userName+" ] "+ time);
                    ui->textBrowser->append(message);
                    QSound player("E:\msg.wav");
                    player.play();
                    break;
                }
            case all_new_user:
                {
                    in >>userName >>localHostName >>ipAddress;
                    newParticipant(userName,localHostName,ipAddress);

                    break;
                }
            case all_user_leave:
                {
                    in >>userName >>localHostName;
                    participantLeft(userName,localHostName,time);
                    break;
                }
        case all_file_name://传输文件 参数为接收者的信息以及文件保存地址
            {
                in >>userName >>localHostName >> ipAddress;
                QString clientAddress,fileName;
                in >> clientAddress >> fileName;
                hasPendingFile(userName,ipAddress,clientAddress,fileName);
                break;
            }
       /* case all_file_refuse:
            {
                in >> userName >> localHostName;
                QString serverAddress;
                in >> serverAddress;
                QString ipAddress = getIP();

                if(ipAddress == serverAddress)
                {
                    server->refused();
                }
                break;
            }
       */
        }
    }
}

//处理新用户加入
void Widget::newParticipant(QString userName,QString localHostName,QString ipAddress)
{
    bool isEmpty = ui->tableWidget->findItems(localHostName,Qt::MatchExactly).isEmpty();
    if(isEmpty)
    {
        QTableWidgetItem *user = new QTableWidgetItem(userName);
        QTableWidgetItem *host = new QTableWidgetItem(localHostName);
        QTableWidgetItem *ip = new QTableWidgetItem(ipAddress);
        ui->tableWidget->insertRow(0);
        
        //Get user,userHost,userIP
        ui->tableWidget->setItem(0,0,user);
        ui->tableWidget->setItem(0,1,host);
        ui->tableWidget->setItem(0,2,ip);
        ui->textBrowser->setTextColor(Qt::gray);
        ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
        
      /*
       * ui->textBrowser->append(tr("%1 加入聊天室！").arg(userName));
        ui->onlineUser->setText(tr("聊天室人数：%1").arg(ui->tableWidget->rowCount()));
      
       
 */
        //仍需调用sendMessage
        //发送新用户登录信息
        //否则新加入的端点无法获得已在线端点信息
          sendMessage(all_new_user);
    }
}

//处理用户离开
void Widget::participantLeft(QString userName,QString localHostName,QString time)
{
    int rowNum = ui->tableWidget->findItems(localHostName,Qt::MatchExactly).first()->row();
    ui->tableWidget->removeRow(rowNum);
    ui->textBrowser->setTextColor(Qt::gray);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
    /*
    ui->textBrowser->append(tr("%1 于 %2 离开聊天室！").arg(userName).arg(time));
    ui->onlineUser->setText(tr("聊天室人数：%1").arg(ui->tableWidget->rowCount()));
    */
}

Widget::~Widget()
{
    delete ui;
}


void Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//将在用户列表中的信息删除掉，然后进行提示
QString Widget::getIP()  //获取ip地址
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
       return 0;
}

QString Widget::getUserName()  //获取用户名
{
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";
    
   //用户名使用QProcess获取
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


void Widget::sendMessage(All_msg_type type, QString serverAddress)  //发送信息
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString localHostName = QHostInfo::localHostName();
    QString address = getIP();
    
    out << type << getUserName() << localHostName;


    switch(type)
    {
        case all_user_leave:
            {
                break;
            }
        case all_new_user:
            {
                out << address;
                break;
            }

        case all_msg :
            {
                if(ui->textEdit->toPlainText() == "")
                {
                    QMessageBox::warning(0,tr("警告"),tr("发送内容不能为空"),QMessageBox::Ok);
                    return;
                }
               out << address << getMessage();
               ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
               break;

            }
        case all_file_name:
            {
                int row = ui->tableWidget->currentRow();
                QString clientAddress = ui->tableWidget->item(row,2)->text();
                out << address << clientAddress << fileName;
                break;
            }
        case all_file_refuse:
            {
                out << serverAddress;
                break;
            }
    }
    udpAllSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast, port);

}

//获得要发送的信息
//不可忘记清空文本编辑器
QString Widget::getMessage()  
{
    QString msg = ui->textEdit->toHtml();

    ui->textEdit->clear();
    
    ui->textEdit->setFocus();
    return msg;
}

void Widget::closeEvent(QCloseEvent *)
{
    sendMessage(all_user_leave);
}

//以下为文件传输
void Widget::sentFileName(QString fileName)
{
    this->fileName = fileName;
    sendMessage(all_file_name);
}

void Widget::hasPendingFile(QString userName,QString serverAddress,  //接收文件
                            QString clientAddress,QString fileName)
{
    QString ipAddress = getIP();
    
    if(ipAddress == clientAddress)
    {
        int btn = QMessageBox::information(this,tr("接受文件"),
                                           
                       tr("来自%1(%2)的文件：%3,是否接收？")
                                           
          .arg(userName).arg(serverAddress).arg(fileName),
                                               QMessageBox::Yes,QMessageBox::No);
        
        if(btn == QMessageBox::Yes)
        {
            QString name = QFileDialog::getSaveFileName(0,tr("保存文件"),fileName);
            if(!name.isEmpty())
            {
                TcpClient *client = new TcpClient(this);
                client->setFileName(name);
                client->setHostAddress(QHostAddress(serverAddress));
                client->show();

            }

        }
        else{
            sendMessage(all_file_refuse,serverAddress);
        }
    }
}

void Widget::on_send_clicked()//发送
{
    sendMessage(all_msg);
}

void Widget::on_sendfile_clicked()
{
    if(ui->tableWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(0,tr("选择用户"),tr("请先从用户列表选择要传送的用户！"),QMessageBox::Ok);
        return;
    }
    server->show();
    server->initServer();
}

void Widget::on_close_clicked()//关闭
{
    this->close();
}

bool Widget::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->textEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
             QKeyEvent *k = static_cast<QKeyEvent *>(event);
             if(k->key() == Qt::Key_Return)
             {
                 on_send_clicked();
                 return true;
             }
        }
    }
    return QWidget::eventFilter(target,event);
}

void Widget::on_fontComboBox_currentFontChanged(QFont f)//字体设置
{
    ui->textEdit->setCurrentFont(f);
    ui->textEdit->setFocus();
}

void Widget::on_fontsizecomboBox_currentIndexChanged(QString size)
{
   ui->textEdit->setFontPointSize(size.toDouble());
   ui->textEdit->setFocus();
}

void Widget::on_textbold_clicked(bool checked)
{
    if(checked)
        ui->textEdit->setFontWeight(QFont::Bold);
    else
        ui->textEdit->setFontWeight(QFont::Normal);
    ui->textEdit->setFocus();
}

void Widget::on_textitalic_clicked(bool checked)
{
    ui->textEdit->setFontItalic(checked);
    ui->textEdit->setFocus();
}

void Widget::on_textUnderline_clicked(bool checked)
{
    ui->textEdit->setFontUnderline(checked);
    ui->textEdit->setFocus();
}

void Widget::on_textcolor_clicked()
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid())
    {
        ui->textEdit->setTextColor(color);
        ui->textEdit->setFocus();
    }
}

void Widget::on_save_clicked()//保存聊天记录
{
    if(ui->textBrowser->document()->isEmpty())
        QMessageBox::warning(0,tr("警告"),tr("聊天记录为空，无法保存！"),QMessageBox::Ok);
    else
    {
       //获得文件名
       QString fileName = QFileDialog::getSaveFileName(this,tr("保存聊天记录"),tr("聊天记录"),tr("文本(*.txt);;All File(*.*)"));
       if(!fileName.isEmpty())
           saveFile(fileName);
    }
}

bool Widget::saveFile(const QString &fileName)//保存文件
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
    out << ui->textBrowser->toPlainText();

    return true;
}

void Widget::on_clear_clicked()//清空聊天记录
{
    ui->textBrowser->clear();
}

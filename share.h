#ifndef SHARE_H
#define SHARE_H
#define public_port xxxx
#define SHAREPORT xxxx

enum MessageType{Message,NewParticipant,ParticipantLeft,FileName,Refuse,Shake};

enum MsgType
{
    NEW_COMING,  //新用户
    WELCOME,
    I_LEAVE,    //离开
//   I_HIDE,  //隐身
    CHAT_TO_ONE,
    CHAT_TO_ALL
};
//枚举变量标志信息的类型，分别为消息，新用户加入，和用户退出 
#endif // SHARE_H

# IMpractice
局域网实时通信
## 主要功能文件：<br>widget.cpp, list_widget.cpp, dialog.cpp
<br> 考虑到文件数量较多，代码并不完善，ui文件没有上传（ui比较繁琐正在研究阶段）
<br> 因此另建了repo
<br>
### 主要实现的功能：
<br>

<br>
widget.cpp: tcp文件传输以及文件存储到客户端， 添加好友以及广播， 和list_widget互动更新好友列表，保存和删除聊天记录，设置头像字体
<br>
list_widget.cpp: 好友分组，好友上下线实时更新以及广播
<br>
dialog.cpp: 聊天和字体头像
<br>注:<br>
 由于我参考教程使用不多以及来不及看一些函数库文档，我的API实现基本搬运实战类教程，导致我目前使用了udp进行消息传输，非常抱歉;
<br>只有文件传输使用了tcp;<br>接下来的目标：UI完善（比如消息头像抖动），和sjr合作实现完整功能，函数库继续学习等<br> 万分感谢耐心等待🙏

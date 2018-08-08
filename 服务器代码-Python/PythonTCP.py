#TCP监听服务器
import socket,threading
from  SendInformation import senddata
from flask import Flask
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind(('0.0.0.0',9999))
s.listen(5)
print('服务器成功启动')
def tcplink(sock,addr):
    while True:
        data=sock.recv(1024)
        if not data:
            break
        with open('c://data.txt','w') as f:
            f.write(str(data))
        print('服务器成功接收到信息'+data.decode('utf8'))
        senddata(data.decode('utf8'))
while True:
    sock,addr=s.accept()
    t=threading.Thread(target=tcplink,args=(sock,addr))
    t.start()
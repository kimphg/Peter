# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
from time import sleep
import numpy as np
import socket
UDP_IP = "127.0.0.1"
UDP_PORT_LOCAL = 31001
UDP_PORT_REMOTE = 31000
sock = socket.socket(socket.AF_INET, # Internet
                        socket.SOCK_DGRAM) # UDP
sock.bind(('', UDP_PORT_LOCAL))
def RecvData( buf_size=1024):
    data, sender_addr = sock.recvfrom(buf_size)
    return data
def ParseData():
    return
while True: 
    #sock.sendto("test", (UDP_IP, UDP_PORT_REMOTE))
    recvData = RecvData().split(",")
    if (recvData[0]=='$RATAR')&(len(recvData)>=5):
        print(recvData)
        t_azi = float()
    else :
        print('wrong data')
        break
    
    



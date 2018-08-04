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
plot_list = np.zeros(0,dtype=[('time', 'i8'), ('azi', 'f4'), ('eazi', 'f4'),('rg', 'f4'), ('erg', 'f4')])
def RecvData( buf_size=1024):
    data, sender_addr = sock.recvfrom(buf_size)
    return data

while True: 
    #sock.sendto("test", (UDP_IP, UDP_PORT_REMOTE))
    recvData = RecvData().split(",")
    if (recvData[0]=='$RATAR')&(len(recvData)>5):
        #print(recvData)
        #plot_list.append()
        plot_list = np.append(plot_list,np.array([
                                    (np.uint32(recvData[1]),
                                    float(recvData[2]),
                                    float(recvData[3]),
                                    float(recvData[4]),
                                    float(recvData[5]))],dtype=plot_list.dtype))
        if(len(plot_list)==10):
            print(plot_list)
            break
    else :
        print('wrong data')
        break
    
    



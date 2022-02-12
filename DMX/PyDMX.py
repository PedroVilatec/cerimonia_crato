import serial
import serial.tools.list_ports
import time
import numpy as np
from PyQt5.QtCore import QThread, pyqtSignal, QTimer
import sys


class PyDMX(QThread):
    new_value = pyqtSignal(bool)
    serial_dmx_read = pyqtSignal(str)
    loopFlag = 1

    def __init__(self, timer_send, use_prev_data=True, preserve_data_name="preserved_data.txt", parent=None):
        QThread.__init__(self, parent)
        self.timer_send = timer_send
        self.Cnumber=8
        self.channel_num = self.Cnumber
        self.ser = None
        self.antigo = np.zeros([self.channel_num+1], dtype='uint8')
        self.antigo[1:self.Cnumber+1] = np.full((self.Cnumber), 255, dtype=int)
        self.antigo[0] = 0 # StartCode
        self.novo= [*self.antigo]
        self.sleepms = 50.0
        # self.breakus = 176.0
        # self.MABus = 16.0
        self.breakus = 250.0
        self.MABus = 30.0
        self.open_serial()
        time.sleep(1)
        self.processing_dmx = True

        self.timer = QTimer()
        self.timer.timeout.connect(self.fade_data)
        self.timer.setInterval(self.timer_send)
        self.timer.start()

        self.inicio = time.time()
        # save filename
        self.preserve_data_name = preserve_data_name
        self.use_prev_data = use_prev_data
        # load preserved DMX data
#         if use_prev_data:
#             try:
#                 self.load_data()
#             except:
#                 print("Something is wrong. please check data format!")
#         while True:
#
#             if self.inicio + .001 < time.time():
#                 self.inicio = time.time()
#                 self.fade_data()
    def run(self):
        while True:
            self.send()
            # self.send()

    def fade_data(self):
        counter = 0
        self.increase = 2

        for antigo, novo in zip(self.antigo, self.novo):
            self.processing_dmx = False

            if counter > 9:
                break
            if antigo < novo:
                if self.antigo[counter] < self.novo[counter] - self.increase and self.antigo[counter] + self.increase < 256:
                    self.antigo[counter] = self.antigo[counter] + self.increase
                else:
                    self.antigo[counter] = self.antigo[counter] + 1
                # self.new_value.emit(True)


            elif antigo > novo:
                if self.antigo[counter] > self.novo[counter] + self.increase and self.antigo[counter] - self.increase > -1:
                    self.antigo[counter] = self.antigo[counter] - self.increase

                else:
                    self.antigo[counter] = self.antigo[counter] - 1
                # self.new_value.emit(True)

            if self.antigo[counter] == self.novo[counter]:
                counter += 1
                time.sleep(.003)
                continue
            else:
                # print("break", counter, self.antigo[counter], self.novo[counter])
                break


        # self.loop()

    def loop(self):
        if self.antigo[1] == 255:
           self.novo[1:self.channel_num+1] = np.full((self.Cnumber), 0, dtype=int)
           print("alternou 255")

        elif self.antigo[1] == 0:
            self.novo[1:self.channel_num+1] = np.full((self.Cnumber), 255, dtype=int)
            print("alternou 0")
    def set_random_data(self):
        self.novo[1:self.channel_num+1]= np.random.randint(255, size=self.Cnumber)
        print(self.novo)

    def set_data_lento(self,id,data):
        if id < self.Cnumber:
            self.novo[id]=data

    def set_data_rapido(self,id,data):
        if id < self.Cnumber:
            self.novo[id]=data
            self.antigo[id]=data
            print(self.antigo)
            # self.new_value.emit(True)

    def set_datalist(self,list_id,list_data):
        try:
            for id,data in zip(list_id,list_data):
                self.set_data(id,data)
        except:
            print('list of id and data must be the same size!')

    def send(self):
        # self.processing_dmx = False
        # Send Break : 88us - 1s
        self.ser.break_condition = True
        time.sleep(self.breakus/1000000.0)

        # Send MAB : 8us - 1s
        self.ser.break_condition = False
        time.sleep(self.MABus/1000000.0)

        # Send Data
        # print(len(self.antigo))
        if self.ser.isOpen():
            self.ser.write(bytearray(self.antigo))
        else:
            print("serial port not opened")

        # print(self.antigo)
            # Sleep
            #time.sleep(self.sleepms/1000.0) # between 0 - 1 sec
        # else:
        #     print(self.processing_dmx)

    def sendzero(self):
        self.antigo = np.zeros([self.channel_num+1],dtype='uint8')
#         self.send()

    def load_data(self):
        self.antigo = np.loadtxt(self.preserve_data_name,dtype='int')
        self.novo = [*self.antigo]
#         print("load data", self.antigo)

    def preserve_data(self):
        np.savetxt(self.preserve_data_name,self.antigo)

    def __del__(self):
        print('Close serial server!')
        # close with preserving current DMX data, I guess you may not need to reset DMX signal in this option.
        if self.use_prev_data:
            self.preserve_data()
        else:
            self.sendzero()
        self.ser.close()

    def open_serial(self):
        self.Brate=250000
        self.Bsize=8
        self.StopB=2
        port_list = serial.tools.list_ports.comports()
        for port in port_list:

            # print(port.name)
            # print(port.product)
            # print(port.serial_number)
            # print(port.device)
            # print("-" * 10)
            if port.serial_number == 'A10KC0IZ':
                print(port.product, port.device)
                self.ser = serial.Serial(port.device, baudrate=self.Brate, bytesize=self.Bsize, stopbits=self.StopB)
                break
        if self.ser == None:
            print("USB DMX512 NÃO ENCONTRADO")
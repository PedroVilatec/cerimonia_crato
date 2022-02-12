import serial
import serial.tools.list_ports
import time
import numpy as np
from PyQt5.QtCore import QThread, pyqtSignal
import sys

class PyDMX(QThread):
    serial_dmx_enabled = pyqtSignal(bool)
    serial_dmx_read = pyqtSignal(str)
    loopFlag = 1
    
    def __init__(self, use_prev_data=True, preserve_data_name="preserved_data.txt", parent=None):
        QThread.__init__(self, parent)
        self.Cnumber=512
        self.channel_num = self.Cnumber
        self.ser = None
        self.data = np.zeros([self.channel_num+1], dtype='uint8')
        self.data[1:self.channel_num+1]= np.random.randint(255, size=512)
        self.data[0] = 0 # StartCode
        self.new_data= [*self.data]
        self.sleepms = 50.0
        self.breakus = 176.0
        self.MABus = 16.0
        self.open_serial()

        time.sleep(1)

        self.inicio = time.time()
        # save filename
        self.preserve_data_name = preserve_data_name
        self.use_prev_data = use_prev_data
        # load preserved DMX data
        if use_prev_data:
            try:
                self.load_data()
            except:
                print("Something is wrong. please check data format!")
#         while True:
#             

    def run(self):
        while True:
            ...
#             print(self.send)
#             self.send()
            
    def set_random_data(self):
        self.new_data[1:self.channel_num+1]= np.random.randint(255, size=512)
        print(self.new_data)
#         sys.exit()

    def set_data(self,id,data):
        print(id, data)
        self.new_data[id]=data

    def set_datalist(self,list_id,list_data):
        try:
            for id,data in zip(list_id,list_data):
                self.set_data(id,data)
        except:
            print('list of id and data must be the same size!')

    def send(self):
        # Send Break : 88us - 1s
        self.ser.break_condition = True
        time.sleep(self.breakus/1000000.0)

        # Send MAB : 8us - 1s
        self.ser.break_condition = False
        time.sleep(self.MABus/1000000.0)

        # Send Data
#         print(self.data)
        if self.ser.isOpen():
            self.ser.write(bytearray(self.data))
        else:
            print("serial port not opened")
        

        # Sleep
#         time.sleep(self.sleepms/1000.0) # between 0 - 1 sec

    def sendzero(self):
        self.data = np.zeros([self.channel_num+1],dtype='uint8')
#         self.send()

    def load_data(self):
        self.data = np.loadtxt(self.preserve_data_name,dtype='int')
        self.new_data = [*self.data]
#         print("load data", self.data)

    def preserve_data(self):
        np.savetxt(self.preserve_data_name,self.data)

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
            print(port.name)
            print(port.product)
            print(port.serial_number)
            print(port.device)
            print("-" * 10)
            if port.serial_number == 'A10KC0IZ':
                self.ser = serial.Serial(port.device, baudrate=self.Brate, bytesize=self.Bsize, stopbits=self.StopB)     
                break
        if self.ser == None:
            print("USB DMX512 NÃO ENCONTRADO")


class SetValue(QThread):

    dmx_value_signal = pyqtSignal(int, int)
    def __init__(self, dmx_data,channel_num, parent=None):
        super(SetValue, self).__init__(parent)
        self.is_running = True
        self.data = dmx_data
        self.channel_num = channel_num
        print(self.data)
        self.new_data = [*self.data]
        self.inicio = time.time()

    def run(self):
        print('Starting thread...')
        self.increase = 2
        while (True):
            if self.inicio + .03 < time.time():
                self.inicio = time.time()
                counter = 0
                for o, n in zip(self.data, self.new_data):
                    if o < n:
                        if self.data[counter] < 255 - self.increase:
                            self.data[counter] = self.data[counter] + self.increase
                        else:
                            self.data[counter] = self.data[counter] + 1
                        self.dmx_value_signal.emit(counter, self.data[counter])
                        
                    elif o > n:
                        if self.data[counter] > 0 + self.increase:
                            self.data[counter] = self.data[counter] - self.increase
                        else:
                            self.data[counter] = self.data[counter] - 1
                        self.dmx_value_signal.emit(counter, self.data[counter])
                    counter += 1
#                 print(self.data)
    #             
    #                 
            if len(self.data)== len(self.new_data) and len(self.new_data) == sum([1 for i, j in zip(self.data, self.new_data) if i == j]):
                
    #         if self.data == self.new_data:
    #             self.set_random_data()
                if self.new_data[1] == 255:
#                     print("255")
                    self.new_data[1:self.channel_num+1] = np.full((512), 0, dtype=int)
#                    print(self.new_data)
                    
                else:
#                     print("0")
                    self.new_data[1:self.channel_num+1] = np.full((512), 255, dtype=int)             
    def stop(self):
        self.is_running = False
        print('Stopping thread...')
        self.terminate()

# if __name__ == '__main__':
#     thread = PyDMX()
#     thread.start()
    
    




    
    
    
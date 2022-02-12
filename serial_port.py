import os
import sys
import time
from PyQt5.QtCore import QThread, pyqtSignal
import serial
import serial.tools.list_ports
import linecache

def printException():
    exc_type, exc_obj, tb = sys.exc_info()
    f = tb.tb_frame
    lineno = tb.tb_lineno
    filename = f.f_code.co_filename
    linecache.checkcache(filename)
    line = linecache.getline(filename, lineno, f.f_globals)
    print ('EXCEPTION IN ({}, LINE {} "{}"): {}'.format(filename, lineno, line.strip(), exc_obj))

class SerialThread(QThread):
    serial_mega_enabled = pyqtSignal(bool)
    serial_read = pyqtSignal(str)
    loopFlag = 1

    def __init__(self, identificador, parent=None):
        QThread.__init__(self, parent)
        self.serial_mega = None
        self.serial_nano = None
        self.initial_time = time.time()
        self.S_PA = None
        self.S_PF = None
        self.S_MG = None
        self.S_ME = None
        self.FP = None
        self.AP = None
        self.GM = None
        self.EM = None
        self.fecha_porta = False
        self.abre_porta = False
        self.expoe_mesa = False
        self.guarda_mesa = False
        self.serial_nano = None
        self.serial_nano = None

        self.atrb_serial = True
        self.identificador = identificador
        print("Microcontrolador", self.identificador)


    def run(self):
        self.atribui_serial()
        while True:
            try:
                if self.initial_time < time.time():
                    self.initial_time = time.time()+.5
                    self.fechaporta()
                    self.abreporta()
                    self.guardamesa()
                    self.expoemesa()
                if self.serial_nano is not None and self.serial_nano.isOpen():
                    if self.serial_nano.inWaiting():
                        print("Dado serial nano", self.serial_nano.readline().decode("utf-8").rstrip())

                if self.serial_mega is not None and self.serial_mega.isOpen():
                    if self.serial_mega.inWaiting():

                        try:
                            serial_data = self.serial_mega.readline().decode("utf-8").rstrip()
                            self.serial_read.emit(serial_data)
                            if "S_PA" in serial_data:
                                for elements in serial_data.split(" "):
                                    if "S_PA" in elements:
                                        self.S_PA = int(elements.split("=")[1])
                                    if "S_PF" in elements:
                                        self.S_PF = int(elements.split("=")[1])
                                    if 'S_MG' in elements:
                                        self.S_MG = int(elements.split("=")[1])
                                    if 'S_ME' in elements:
                                        self.S_ME = int(elements.split("=")[1])
                                    if 'G_M' in elements:
                                        self.GM = int(elements.split("=")[1])
                                    if 'E_M' in elements:
                                        self.EM = int(elements.split("=")[1])
                                    if 'F_P' in elements:
                                        self.FP = int(elements.split("=")[1])
                                    if 'A_P' in elements:
                                        self.AP = int(elements.split("=")[1])
                        except:
                            printException()

                else:
                    if self.atrb_serial == False:
                        self.serial_mega = None
                        print("Porta  Serial desconectada!, tentando reconexão")
                        self.atrb_serial = True
                        self.atribui_serial()
                time.sleep(.3)
            except:
                printException()

    def serialMegaWrite(self, dados):
        if self.serial_mega.isOpen():
            self.serial_mega.write(dados)

    def serialNanoWrite(self, dados):
        if self.serial_nano.isOpen():
            self.serial_nano.write(dados)

    def atribui_serial(self):
        timeout = time.time() + 10
        portas_abertas = []
        port_list = serial.tools.list_ports.comports()
        for port in port_list:

            # print(port.name, port.device)
            # print(port.product)
            # print(port.serial_number)
            # print(port.device)
            # print("-" * 10)
            if port.product == 'USB2.0-Serial':
                try:
                    portas_abertas.append(serial.Serial(port.device, 9600, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=.1))
                    # print("Portas abertas",  port.device)
                except:
                    print(port.device, "Acesso negado")

        while self.serial_mega == None and timeout > time.time():
            string = ""
            for channel in portas_abertas:
                channel.write(b'MODEL\n')
                time.sleep(.9)
                if channel.in_waiting > 0:
                    string = channel.readline().decode("utf-8", "ignore")
                    # print("Porta = {} String {}".format(channel.port, string.rstrip()))
                else:
                    print(channel.port, "Sem dados")
                if self.serial_mega == None:
                    if self.identificador in string:

                        # print("Porta", self.identificador, channel.port)
                        self.serial_mega = channel
                        self.serial_mega.write(b'DEBUG,1\n')
                        # print("Porta serial aberta = {}, identificador = {}".format(self.serial_mega.port, string.rstrip()))

                if self.serial_nano == None:
                    if "NANO" in string:
                        self.serial_nano = channel

            if self.serial_mega is not None:
                self.atrb_serial = False

            if time.time() > timeout - len(portas_abertas):
                print("RESETANDO MICROCONTROLADOR")
                for channel2 in portas_abertas:
                    channel2.setDTR(False)
                    time.sleep(1)
                    channel2.setDTR(True)
        if self.serial_mega == None:
            print("Reiniciando App por nao detecção da porta serial")
            os.execv(sys.executable, ['python3'] + sys.argv)
        else:
            print("Porta serial MEGA = {}".format(self.serial_mega.port))
            self.serial_mega_enabled.emit(True)

    def guardamesa(self):
        if self.guarda_mesa == True:
            if self.S_MG == 0:
                if self.GM == 0:
                    self.serialMegaWrite(b'GUARDAMESA\n')
            else:
                self.guarda_mesa = False

    def expoemesa(self):
        if self.expoe_mesa == True:
            if self.S_ME == 0:
                if self.EM == 0:
                    self.serialMegaWrite(b'EXPOEMESA\n')
            else:
                self.expoe_mesa = False

    def fechaporta(self):
        if self.fecha_porta == True:
            if self.S_PF == 0:
                if self.FP == 0:
                    print("passou fecha")
                    self.serialMegaWrite(b'FECHAPORTA\n')
            else:
                self.serialMegaWrite(b'LED_CABINE,0,0,0,0,0,0\n')
                self.fecha_porta = False

    def abreporta(self):
        if self.abre_porta == True:
            if self.S_PA == 0:
                if self.AP == 0:
                    self.serialMegaWrite(b'ABREPORTA\n')
            else:
                self.abre_porta = False

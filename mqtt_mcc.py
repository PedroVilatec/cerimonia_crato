
import sys
from PyQt5 import QtWidgets
from PyQt5 import QtCore
import PyQt5
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.Qt import *
import datetime
import time
from instance.devices import Devices
import json
import paho.mqtt.client as mqtt
import threading
import signal
import serial
global serial_data
import logging
import subprocess
import datetime
import os

mqttc = mqtt.Client("App")
global btn
btn = []
global indice_btn
indice_btn = []

class MQTT(QtCore.QThread):

	def __init__(self, parent=None):
		super(MQTT, self).__init__(parent=parent)
		# The shutdown_flag is a threading.Event object that
		# indicates whether the thread should be terminated.
		self.shutdown_flag = threading.Event()



	def run(self):
		mqttc.on_message = MyApp.on_message
		mqttc.on_connect = MyApp.on_connect
		mqttc.on_publish = MyApp.on_publish
		mqttc.on_subscribe = MyApp.on_subscribe
		# Uncomment to enable debug messages
		# mqttc.on_log = on_log

		mqttc.message_callback_add("MCC\#", MyApp.on_message)
		#mqttc.message_callback_add("mcc/#", MyApp.on_message_msgs)
		#mqttc.message_callback_add("#", MyApp.on_message_bytes)
		#mqttc.message_callback_add("mcc/devices", MyApp.devices_connected)
		mqttc.connect("192.168.0.100", 1883, 60)
		#mqttc.subscribe("mcc/#", 0)
		mqttc.subscribe("#", 0)
		#~ mqttc.subscribe("esp32/output", 0)
		mqttc.loop_forever()


class MyApp(QWidget):
	def __init__(self):
		super(MyApp, self).__init__()
		window_width = 800
		window_height = 768
		#self.setFixedSize(window_width, window_height)
		self.buttons = []
		self.labels = []
		self.initUI()

	def createLayout_group_coletor(self, number):
		global btn
		global indice_btn

		sgroupbox = QGroupBox("{}".format(number), self)
		layout = QGridLayout()
		layout_groupbox = QVBoxLayout(sgroupbox)
		sgroupbox.setStyleSheet("QGroupBox { background-color: rgb(51, 212,\
				194);  solid rgb(0, 0, 0); }")

		linha = 0
		coluna = 0
##        print(Devices.CONTROLADORES[number])
		for a in sorted(Devices.CONTROLADORES[number]):
				b = a
##                print(Devices.CONTROLADORES[number].keys())
				try:
					if "STATUS" in a:

						label = QLabel(a)
						label.setAlignment(Qt.AlignCenter)
						label.setStyleSheet('QLabel {color: red;}')
						layout_groupbox.addWidget(label)
						a = QLineEdit()
						a.setAlignment(Qt.AlignCenter)
						a.setText(Devices.CONTROLADORES[number][b]+" "+time.strftime("%d/%m/%Y %H:%M"))
						a.setReadOnly(True)

						name = number+ "/"+b
						indice_btn.append(name)# iteração para poder atualizar a label específica pela ordem de criação
##                        print(name)
						a.setObjectName(name)
						btn.append(a)
						layout_groupbox.addWidget(a)
					else:
						a = QPushButton(b)
						self.buttons.append(a)
						layout.addWidget(a,linha,coluna)
						command = number+"/"+b
						a.clicked.connect(lambda checked, text= command: self.on_button(text))
						if coluna > 2:
		##                    print(linha)
							coluna = 0
							linha += 1
						else:
							coluna += 1

					layout_groupbox.addWidget(a)
				except Exception as e:
					print(e)
##        layout_groupbox.addStretch(1)
		return sgroupbox

	def createLayout_Container_coletor(self):
		self.scrollarea = QScrollArea(self)
		self.scrollarea.setFixedWidth(350)
		self.scrollarea.setWidgetResizable(True)

		widget = QWidget()
		self.scrollarea.setWidget(widget)
		self.layout_SArea = QVBoxLayout(widget)

		for i in sorted(Devices.CONTROLADORES):
			if "A" in i:
				self.layout_SArea.addWidget(self.createLayout_group_coletor(i))
		self.layout_SArea.addStretch(1)

	def on_button(self, n):
		print("topico %s" %(str(n.split("/")[0]+"/")))
		print("payload %s" %(str(n.split("/")[1]+"/")))
		try:
##          pass

##            infot = mqttc.publish(str(n.split("/")[0]+"/"), str(n.split("/")[1]), qos=0)
			infot = mqttc.publish(str(n.split("/")[0]), str(n.split("/")[1]), qos=0)
		except Exception as e:
			print(e)

##        for a in self.buttons:
##            try:
##                if a.text() == n:
##                    a.setStyleSheet('QPushButton {background-color: #A3C1DA; color: red;}')
##
##                else:
##                    a.setStyleSheet('QPushButton {background-color: #A3C1DA; color: blue;}')
##  ##        print('Button {0} clicked'.format(n))
##
##            except Exception as e:
##                print(e)





	''' FUNÇÕES DO MQTT'''
	def on_message_bytes(mosq, obj, msg):
		# This callback will only be called for messages with topics that match
		# $SYS/broker/bytes/#
		print("BYTES: " + msg.topic + " " + str(msg.qos) + " " + str(msg.payload))

	def devices_connected(mosq, obj, msg):
		# This callback will only be called for messages with topics that match
		# $SYS/broker/bytes/#
		print("BYTES: " + msg.topic + " " + str(msg.qos) + " " + str(msg.payload))



	def on_message(mosq, obj, msg):
		# This callback will only be called for messages with topics that match
		# $SYS/broker/messages/#
		#~ print("EVAPORADOR: " + msg.topic + " " + str(msg.qos) + " " + str(msg.payload))
		try:
				turned = msg.payload.decode()
				#print(turned)
		except:
				pass

		#print(data["STATUS"])
##        print()
##        print(turned)
##        print(msg.topic.split("\\")[1])
####        print()

		try:
				data = dict(json.loads(turned))
				msgFromMcc = msg.topic.split("\\")[0]
		except:
				msgFromMcc = ""
		if(msgFromMcc == "MCC"):

			for key in data:
				dados = msg.topic.split("\\")[1]
				if(key == "STATUS"):
							for subKey in data[key]:
								dictStatus = str(data[key]) # DICT STATUS##
								nameLabel =data["DISPOSITIVO"] + "/" +subKey # retorna o nome das qlinedit
								valueLabel = str(data[key][subKey]) #RETORNA O VALOR DAS KEYS STATUS
								#print (" tornou: %s " %(nameLabel))
								#print(" tornou: %s " %(dados))
								#print (" tornou: %s, nameLabel %s " %(str(data[key][subKey]),nameLabel)) #RETORNA O VALOR DAS KEYS STATUS
		
								for i in range(len(indice_btn)):
										#print()
										#print(indice_btn[i])
										#print(nameLabel)
										#print()
										if indice_btn[i] == nameLabel:
												#print("valueLabel %s"%(valueLabel))
												#print(btn[i].text())
												btn[i].setText(valueLabel +" -> " + time.strftime("%d/%m/%Y %H:%M:%S"))
	









##        try:
##          data = dict(json.loads(turned))
##          msgFromMcc = msg.topic.split("\\")[0]
##        except:
##          msgFromMcc = ""
##        if(msgFromMcc == "MCC"):
##          print(data)
##          for key in data:
##            dados = msg.topic.split("\\")[1]
##            if(key == "STATUS"):
##               for subKey in data[key]:
##                  dictStatus = str(data[key]) # DICT STATUS
##                  nameLabel = dados + "/" +subKey # retorna o nome das qlinedit
##                  valueLabel = str(data[key][subKey]) #RETORNA O VALOR DAS KEYS STATUS
##                  #print (" tornou: %s " %(nameLabel))
##                  #print(" tor: %s " %(dados))
##                  #print (" tornou: %s, nameLabel %s " %(str(data[key][subKey]),nameLabel)) #RETORNA O VALOR DAS KEYS STATUS
##
##                  for i in range(len(indice_btn)):
##                          #print()
##                          #print(indice_btn[i])
##                          #print(nameLabel)
##                          #print()
##                          if indice_btn[i] == nameLabel:
##                                  print("valueLabel %s"%(valueLabel))
##                                  #print(btn[i].text())
##                                  btn[i].setText(valueLabel +" -> " + time.strftime("%d/%m/%Y %H:%M:%S"))
##

	def on_connect(mqttc, obj, flags, rc):
		print("rc: " + str(rc))


	def on_message_msgs(mqttc, obj, msg):
		#print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))
		print(msg.topic + str(msg.payload.decode()))


	def on_publish(mqttc, obj, mid):
		print("mid: " + str(mid))


	def on_subscribe(mqttc, obj, mid, granted_qos):
		print("Subscribed: " + str(mid) + " " + str(granted_qos))


	def on_log(mqttc, obj, level, string):
		print(string)

	''' FIM DAS FUNÇÕES DO MQTT'''

	def recurring_timer(self):

		botoes = [
		"ACESSO_MCC_1E/STATUS_PORTA",
		"COLETORA_BLOCO_1A/STATUS_NIVEL",
		"COLETORA_BLOCO_1A/STATUS_CORRENTE_BOMBA",
		"EVAPORADOR_A_1E/STATUS_NIVEL",
		"EVAPORADOR_A_1E/STATUS_RESISTENCIA",
		"EVAPORADOR_CABINE_1E/STATUS_NIVEL",
		"VALVULA_CABINE_1D/STATUS_VALVULA"]
##        for itens in botoes:
##                print(type(itens))


				#infot = mqttc.publish(str(itens), "MEIO", qos=0)

##              time.sleep(.1)

	def initUI(self):
		self.createLayout_Container_coletor()
		self.layout_All = QVBoxLayout(self)
		self.layout_All.addWidget(self.scrollarea)
		self.timer = QTimer()
		self.timer.setInterval(1000)
		self.timer.timeout.connect(self.recurring_timer)
		Mqtt = MQTT()
		Mqtt.start()
		self.timer.start()
		self.show()


if __name__ == '__main__':
	app = QApplication(sys.argv)
	ex = MyApp()
	sys.exit(app.exec_())


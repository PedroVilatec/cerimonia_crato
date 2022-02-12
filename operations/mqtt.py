# -*- coding: utf-8 -*-
import filecmp
from fnmatch import fnmatch
import linecache
import time, datetime
import os, sys
import threading
import paho.mqtt.client as mqtt
import paho.mqtt.client as mqtt_server
import threading

import json
from operations import saveFile
import urllib.request

def printException():
	exc_type, exc_obj, tb = sys.exc_info()
	f = tb.tb_frame
	lineno = tb.tb_lineno
	filename = f.f_code.co_filename
	linecache.checkcache(filename)
	line = linecache.getline(filename, lineno, f.f_globals)
	print ('EXCEPTION IN ({}, LINE {} "{}"): {}'.format(filename, lineno, line.strip(), exc_obj))

def printException():
	exc_type, exc_obj, tb = sys.exc_info()
	f = tb.tb_frame
	lineno = tb.tb_lineno
	filename = f.f_code.co_filename
	linecache.checkcache(filename)
	line = linecache.getline(filename, lineno, f.f_globals)
	print ('EXCEPTION IN ({}, LINE {} "{}"): {}'.format(filename, lineno, line.strip(), exc_obj))

class Mosquitto():
	def __init__(self, client, porta):
		self.client = client
		self.porta = porta
		self.inverte_led_urna = False
		self.cenas = {}
		self.mqttc = mqtt.Client(self.client)
		self.mqttc.on_message = self.on_message
		self.mqttc.on_disconnect = self.on_disconnect
		self.mqttc.on_connect = self.on_connect
		# mqttc.on_publish = on_publish
		# ~ self.mqttc.on_subscribe = self.on_subscribe
		self.desligar = False
		self.local_connected = False
		# mqttc.on_connect = on_connect
		# mqttc.on_publish = on_publish
		# mqttc.on_subscribe = on_subscribe
		# Uncomment to enable debug messages
		# mqttc.on_log = on_log
		self.inicio = time.time() + 20
		self.envia_telegram_single = None # variavel sentenciada para enviar telegramas
		self.envia_telegram_all = None # variavel sentenciada para enviar telegramas
		self.player = None
		self.serial_mega = None
		self.serial_nano = None
		self.thread_loop_mqtt = None
		self.thread = None

		self.thread = threading.Thread(target=self.connect_mqtt)
		self.thread.start()
	def connect_mqtt(self):
		while not self.local_connected:
			print("Conectando ao servidor local...")
			try:
				self.mqttc.connect("localhost",  self.porta, 60)
				self.local_connected = True
				if self.thread_loop_mqtt is None:
					self.thread_loop_mqtt = threading.Thread(target=self.mqttc.loop_forever)
					self.thread_loop_mqtt.start()

			except Exception as e:
				print("Conexao local falhou")


				time.sleep(5)

	def on_disconnect(self, client, userdata,  rc):
		try:
			print("Mqtt desconectado")
			self.local_connected = False
			self.thread = threading.Thread(target=self.connect_mqtt)
			self.thread.start()
		except:
			printException()

	def on_connect(self, mqttc, obj, flags, rc):
		try:
			print("Mqtt local conectado")
			print(self.client)
			self.mqttc.subscribe(self.client, 0)
		except:
			printException()

	def powerOff(self):
		cmd = {"desligarApp":"desligarAPP"}
		cmd = json.dumps(cmd)
		self.mqttc.publish("capela/ihm", cmd, qos=0)

	def updateVideo(self, json_data):
		...

	def on_message(self, mqttc, obj, msg):

			try:
				self.turned = msg.payload.decode()
				# ~ print(self.turned)
				data = json.loads(self.turned)
				print("local", data)

				for k,v in data.items():
					if k == 'CONFIG':
						print(json.dumps(data, indent=4, sort_keys=True, ensure_ascii=False))
						data['CONFIG']["UPDATE"] = False
						cmd = json.dumps(data)
						self.mqttc.publish("capela/ihm", cmd, qos=0)
						print("enviou updated")
						self.updateVideo(data)


					if k == 'fumaca':
						comando_serial = str(v)+"\n"
						print(comando_serial)
						self.serial_mega.write(comando_serial.encode('utf-8'))

					if k == "sistema":
						if v == "reset":
							os.system("sudo reboot")
						for k2,v2 in v.items():
							if k2 == "iluminacao":
								comando_serial = str(v2)+"\n"
								print(comando_serial)
								self.serial_mega.write(comando_serial.encode('utf-8'))
					if k == "pause":
						print()
						print("Pause")
						self.player.pause()
						print(self.player.position())

					if k == "play":
						print("Player")
						self.player.play()

					if k == "reboot":
						os.system("sudo reboot")

					if k == "restartApp":
						os.system("sudo reboot")
						# print("restarting App")
						# os.execv(sys.executable, ['python3'] + sys.argv)

					if k == "desligarApp":
						thread = threading.Thread(target=self.powerOff)
						thread.start()
						self.desligar = True

					if k == "closeSerial":
						print("close_serial")
						self.serial_mega.close()

					if k == "openSerial":
						print("open_serial")
						self.serial_mega.open()
					if k == 'controle':
						comando_serial = str(v)+"\n"
						print(comando_serial)
						self.serial_mega.write(comando_serial.encode('utf-8'))

					if k == 'mecanismo':
						if v == "ilum_petalas":
							if self.inverte_led_urna == False:
								self.inverte_led_urna = True
								self.serial_mega.write(b'LED_URNA,255\nLED_ESTEIRA,1\n')
							else:
								self.inverte_led_urna = False
								self.serial_mega.write(b'LED_URNA,0\nLED_ESTEIRA,0\n')
						else:
							comando_serial = str(v)+"\n"
							print(comando_serial)
							self.serial_mega.write(comando_serial.encode('utf-8'))

					if k == 'leds_mega':
						rgb = str(v) + "\n"
						print("comando_mega",rgb)
						self.serial_mega.write(rgb.encode('utf-8'))


					if k == 'APAGAR':
						try:
							self.serial_mega.write(b'APAGAR\n')
							self.serial_nano.write(b'APAGAR\n')
							time.sleep(5)
						except Exception as e:
							print(e)

					if k == 'dimer':
						self.serial_mega.write(b'ACENDER\n')
						for k2,v2 in v.items():
							if "COMANDO" in k2:
								# ~ print(v2)
								self.serial_mega.write(v2.encode('utf-8'))
								time.sleep(.02)

					if k == 'dim_led':
						self.serial_mega.write(b'ACENDER\n')
						for k2,v2 in v.items():
							if "COMANDO" in k2:
								if v["DISPOSITIVO"] == "MEGA":
									self.serial_mega.write(v2.encode('utf-8'))
									time.sleep(.02)
								if v["DISPOSITIVO"] == "NANO":
									self.serial_nano.write(v2.encode('utf-8'))
									time.sleep(.02)

			except Exception as e:
				printException()

	def _map(self, x, in_min, in_max, out_min, out_max):
		return int((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

class MosquittoServer():
	def __init__(self, client):

		self.client = client
		self.mqttc_server = mqtt_server.Client(self.client)
		self.mqttc_server.on_message = self.on_message_server
		self.connected = False
		self.serial_nano = None
		self.serial_mega = None
		self.player = None
		self.mqttc_server.on_connect = self.on_connected_server
		# mqttc.on_publish = on_publish
		# mqttc.on_subscribe = on_subscribe
		# Uncomment to enable debug messages
		# mqttc.on_log = on_log
		self.inicio = time.time() + 20
		self.send_telegram_single = "" # variavel sentenciada para enviar telegramas
		self.send_telegram_all = "" # variavel sentenciada para enviar telegramas
		thread2 = threading.Thread(target=self.persistent)
		thread2.start()

	def persistent(self):
		while not self.connected:
			print("Conectando ao servidor...")
			try:
				self.mqttc_server.connect("201.46.55.138", 1883, 60)
				self.mqttc_server.subscribe(self.client, 0)
				print("conectado ao Servidor !")
				thread = threading.Thread(target=self.mqttc_server.loop_forever)
				thread.start()
				thread1 = threading.Thread(target=self.verifica_online_server)
				thread1.start()
				self.connected = True
			except:
				print("Conexao falhou")


			time.sleep(1)

	def on_connected_server(self):
		print("Conectado ao mqttServer")

	def on_message_server(self, mqttc, obj, msg):

			try:
				self.turned = msg.payload.decode()
				data = json.loads(self.turned)

				print("server", data)
				#~ for p in data:

					#~ print(data[p])

				for k,v in data.items():
					print(k)
					#~ if "cena" in k:
						#~ print(k)
					if k == "sistema":
						for k2,v2 in v.items():
							if k2 == "iluminacao":
								comando_serial = str(v2)+"\n"
								print(comando_serial)
								self.serial_mega.write(comando_serial.encode('utf-8'))


					if k == "play":
						print("Player")
						self.player.play()

					if k == "reboot":
						print("restarting App")
						os.system("sudo reboot")

					if k == "restartApp":
						print("restarting App")
						os.execv(sys.executable, ['python3'] + sys.argv)

					if k == "closeSerial":
						print("close_serial")
						self.serial_mega.close()

					if k == "openSerial":
						print("open_serial")
						self.serial_mega.open()

					if k == 'fumaca':
						comando_serial = str(v)+"\n"
						print(comando_serial)
						self.serial_mega.write(comando_serial.encode('utf-8'))

					if k == 'controle':
						comando_serial = str(v)+"\n"
						print(comando_serial)
						self.serial_mega.write(comando_serial.encode('utf-8'))


					if k == 'mecanismo':
						comando_serial = str(v)+"\n"
						print(comando_serial)
						self.serial_mega.write(comando_serial.encode('utf-8'))
					if k == 'dimer':
						for k2,v2 in v.items():
							dim = str(k2) + "," + str(v2) + "\n"
							self.serial_mega.write(dim.encode('utf-8'))
					if k == 'dimmer':
						dim = str(v) + "\n"
						self.serial_mega.write(dim.encode('utf-8'))

					if k == 'leds':
						rgb = str(v) + "\n"
						self.serial_mega.write(rgb.encode('utf-8'))

					if k == 'dim_led':
						print(k)
						#~ for k2,v2 in v.items():

							#~ dim = str(k2)+"," + ",".join(str(x) for x in v2) + "\n"
							#~ print(dim)
							#~ self.serial_mega.write(dim.encode('utf-8'))

			except:
				printException()

	def verifica_online_server(self):

		while True:
			try:
				dataDict = {"CLIENTE": self.client, "STATUS": { \
					"STATUS CONEXAO": "On line", \
					# ~ "STATUS_INVERSOR": status_inversor, \
					# ~ "PRESSAO": pressao, \
					}}
				dataDict = json.dumps(dataDict)
				if self.connected:
					self.mqttc_server.publish(self.client+"/retorno", dataDict, qos=0)
				time.sleep(10)

			except Exception as e:
				printException()

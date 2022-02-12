#encoding: utf-8
#!/usr/bin/env python3.5
#import smbus
#from fnmatch import fnmatch
############################# imports app.py   #####################
import sys, os, repackage
import socket
repackage.up()

from infra.lib_sensors_db import get_sensors_id
from operations.sheet import Sheet
from operations.telegram import Bot
from operations import saveFile
from operations.op_facade import setTime, updateData, alertEmail, dailyEmail
from infra.raspserial import getSerial

import sqlite3
import datetime
import schedule
import time

from flask import Flask
from flask import redirect
from flask import render_template
from flask import request
from flask import url_for
from flask_sqlalchemy import SQLAlchemy
from flask_socketio import SocketIO
###################################################
from operations.mqtt import MosquittoServer, Mosquitto
import config #configurações dos controladores rgb
import linecache
import threading

import signal
import serial
global serial_data
import json
from omxplayer.player import OMXPlayer
from instance.config import Config
#from operations.telegram import Bot
from pathlib import Path
from time import sleep
import logging
import subprocess
import datetime
import termios, tty
from infra.check_internet import have_internet

app = Flask(__name__)

db_uri = 'sqlite:///{}'.format('/home/pi/Crabit/instance/banco/vilatec.db')
app.config['SQLALCHEMY_DATABASE_URI'] = db_uri
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = True

app.config['TEMPLATES_AUTO_RELOAD'] = True
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)

volume = 1
inicio_chuva_petalas = 218
acende_scenario = 365
inicio_pulsa_cruz = 340
fim_pulsa_cruz = 350
inicio_sequencia = inicio_chuva_petalas - 5
inicio_fumaca = inicio_chuva_petalas - 5 - 5


def printException():
	exc_type, exc_obj, tb = sys.exc_info()
	f = tb.tb_frame
	lineno = tb.tb_lineno
	filename = f.f_code.co_filename
	linecache.checkcache(filename)
	line = linecache.getline(filename, lineno, f.f_globals)
	print ('EXCEPTION IN ({}, LINE {} "{}"): {}'.format(filename, lineno, line.strip(), exc_obj))

def buttonPressed(status):
	'''ATRIBUI A CERIMONIA AO BOTAO PRESSIONADO NO PAINEL'''
	global CONFIGURACAO
	global videoDuration
	
	if status == 1:     
		CONFIGURACAO = config.CERIMONIA_1
		try:
			player.load(config.video_1,  pause=False)
			time.sleep(1)
#			player.pause()
			player.set_aspect_mode('stretch')

			player.set_video_pos(200, 0, 680, 480)			
			print(player.video_pos())			
		except Exception as ex:
			print(ex)
			
	if status == 2:     
		CONFIGURACAO = config.CERIMONIA_2
		print(CONFIGURACAO)
		try:
			player.load(config.video_2,  pause=False)
			time.sleep(1)
			player.pause()
			player.set_aspect_mode('stretch')
		except Exception as ex:
			print(ex)   
		
	if status == 3:     
		CONFIGURACAO = config.CERIMONIA_3
		print(CONFIGURACAO)
		try:
			player.load(config.video_3,  pause=False)
			time.sleep(1)
			player.pause()
			player.set_aspect_mode('stretch')
		except Exception as ex:
			print(ex)
			
	if status == 4:     
		CONFIGURACAO = config.CERIMONIA_4
		print(CONFIGURACAO)
		try:
			player.load(config.video_4,  pause=False)
			time.sleep(1)
			#player.pause()
			player.set_aspect_mode('stretch')
		except Exception as ex:
			print(ex)
			
		
	if status == 5:     
		CONFIGURACAO = config.CERIMONIA_5
		print(CONFIGURACAO)             
		try:
			player.load(config.video_5,  pause=False)
			time.sleep(1)
			player.pause()
			player.set_aspect_mode('stretch')
		except Exception as ex:
			print(ex)
		
	videoDuration = player.duration()
	#~ while player.playback_status() != "Paused":
		#~ player.pause()
		#~ time.sleep(.01)
	time.sleep(.1)
	#~ tempo = time.time()
	player.set_volume(volume)
	print("Volume " + str(player.volume()))
	print("O vídeo %d tem %.2f segundos de duração "%(status, player.duration()))           
	print("passou")

def process_serial(dados):
	global CONFIGURACAO
	print("Dados serial " +str(dados))
	global tempo
	global intervalo
	global lastIntervalo
	
	if dados == 'MESA GUARDADA':
		for itens in config.luzUrna:
			
			#apaga cenario       
		
			pass

	if dados == 'ABRINDO A PORTA':
		#scenario_2()
		pass
	
	if dados == 'PORTA FECHADA':
		'''ACENDE A CAPELA APÓS O TEMPO config.ACENDE_SCENARIO'''
		
		#~ J2=Job('change', "SANCA_CABINE", ' -C gradual 90 "(0,0,0)"')
		#~ J2.start()

		pass
	
	if dados == 'TIME':
		print(tempo - time.time())
		tempo = time.time()
		pass
	if dados == 'APAGA_CABINE':
		#~ J2=Job('change', "SANCA_CABINE", ' -C gradual 90 "(0,0,0)"')
		#~ J2.start()
		
		pass
	if dados != 'ONLINE':
		pass

	if dados == 'CERIMONIA_COMPLETA':
		# ~ try:
			# ~ telegram_bot.envia_telegram_single("334240998", "CERIMONIA INICIADA")
		# ~ except:
			# ~ print("ERRO TELEGRAM")
		global iniciada
		global passedScenario1
		global passedScenario2
		iniciada = True
		passedScenario1 = False
		passedScenario2 = False     
		ser.write(b'COMPLETA\n')
		ser.write(b'<0,0,0,0,0,0,0,0,0,100>\n')
		cmd = {"display_off":"display_off"}
		cmd = json.dumps(cmd)
		mqttLocal.mqttc.publish("capela/ihm", cmd, qos=2)
		#scenario_1()
		#~ ex = Cortinas("desce")
		#~ ex.start()

		#time.sleep(25) #delay da cortina

		intervalo = []
		for a in range(6):
			a = datetime.datetime.now()
			intervalo.append(a)           

		lastIntervalo = []
		for a in range(6):
			a = 0;    
			lastIntervalo.append(intervalo[a])
		#~ print("intervalo antes "+str(intervalo[0]))
		
		'''SETA O VIDEO 1 APóS CONTAGEM DE TEMPO EM config.DELAY_2 SUBTRAINDO A EMISSÃO DA FUMAÇA'''        
		intervalo[0] = datetime.datetime.now() +\
					   datetime.timedelta(seconds = CONFIGURACAO["DELAY_VIDEO"] - CONFIGURACAO["DELAY_FUMACA"])
					   
		#~ print("intervalo depois "+str(intervalo[0]))
		#~ print("datetime "+str(datetime.datetime.now() ))
	   
		'''SETA O ACENDIMENTO DA CAPELA APóS O ENCERRAMENTO DA CERIMONIA'''       
		intervalo[1] = datetime.datetime.now() + datetime.timedelta(seconds = CONFIGURACAO["ACENDE_SCENARIO"])
							  
		
		'''SETA INICIO DA PULSAÇÃO DA CRUZ SUBTRAINDO A EMISSÃO DA FUMAÇA'''
		intervalo[2] = datetime.datetime.now() + datetime.timedelta(seconds = CONFIGURACAO["DELAY_PULSA"])       

		'''SEM USO NO MOMENTO'''        
		intervalo[3] = datetime.datetime.now() + datetime.timedelta(seconds = CONFIGURACAO["DELAY_3"])




##        player.seek(280)
		player.set_position(1.)
		player.play()
		#player.set_position(330)	   
		#player.load(source, pause=False)

def scenario_1():
	'''ROTINA QUANDO INICIA A CERIMONIA'''
	print("cena_1")
	for itens in config.CENA_1:
		print("itens", itens)
		J2=Job(itens[0], itens[1], itens[2])
		J2.start()
		sleep(.1)
		pass

		
def scenario_2():
	print("cena_2") 
	'''ROTINA CHAMADA QUANDO ABRE A PORTA DURANTE A CERIMONIA'''
	for itens in config.CENA_2:
		print("Cena 2")
		J2=Job(itens[0], itens[1], itens[2])
		J2.start()
		sleep(.1)
		pass

def scenario_3():
	print("cena_3") 
	'''ROTINA CHAMADA DEPOIS QUE FECHA A PORTA DURANTE A CERIMONIA'''
	for itens in config.CENA_3:
		print("Cena 3")
		
		J2=Job(itens[0], itens[1], itens[2])
		J2.start()
		sleep(.1)

	pass

def liga_todos():
	'''LIGA TODAS AS SANCAS NA INICIALIZAÇÃO'''
	print("Liga todos os leds")
	for k, v in leds.LedsWifi.items():
				
		J2=Job('change', k, ' -1')
		J2.start()
		sleep(.1)
	J2=Job('change',"LATERAL", ' -C gradual 90 "(255,255,255)"')
	J2.start()        
	sleep(1)


		
class Job(threading.Thread):
 
	def __init__(self, a, b, c):
		threading.Thread.__init__(self)
 
		# The shutdown_flag is a threading.Event object that
		# indicates whether the thread should be terminated.
		self.shutdown_flag = threading.Event()
		self.a = a
		self.b = b
		self.c = c
		
 
		# ... Other thread setup code here ...
 
	def run(self):

		if self.a == 'change':
			cont = 0
			for a in range(50):
				ip =  leds.getIp(self.b)           
				if ip == "0.0.0.0":
					print("Buscando IP ", self.b)                    
					time.sleep(1)
				else:
					print("ip", ip)
					cmd="flux_led " + ip + self.c
					p = subprocess.Popen(cmd, shell=True, stdin=subprocess.PIPE,\
					stdout=subprocess.PIPE, stderr=subprocess.STDOUT,close_fds=False)
					bulbs = p.stdout.read().decode().rstrip()
					print(bulbs)
					if not "Unable" in bulbs:
						print("Ok", self.b, ip)
						break
					else:
						cont = cont +1
						
			
			
				
		elif self.a != 'start' and self.a != 'change':
			cont = 0
			for a in range(50):
				ip =  leds.getIp(self.a)
				if ip == "0.0.0.0":
					print("Buscando IP ", self.a)                    
					time.sleep(1)
				
				else:
					try:
						cmd="flux_led " + ip + " -C gradual " + self.b + " " + self.c
						p = subprocess.Popen(cmd, shell=True, stdin=subprocess.PIPE,\
						stdout=subprocess.PIPE, stderr=subprocess.STDOUT,close_fds=False)
						bulbs = p.stdout.read().decode().rstrip()
						print("1", bulbs)
						if not "Unable" in bulbs:
							print("Ok", self.a, ip)                         
							break
						else:
							cont = cont +1
					except Exception as e:
						print(e)
				
					
				
				
			   
##        print('Thread #%s stopped' % self.ident)

class readSerial(threading.Thread):
##  '''VERIFICAR SE A THREAD RODA SEM O PCF8574 INSTALADO''' 
	def __init__(self):
		threading.Thread.__init__(self)     
		# The shutdown_flag is a threading.Event object that
		# indicates whether the thread should be terminated.
		self.shutdown_flag = threading.Event()
 
	def run(self):  
			#liga_todos()
			while not self.shutdown_flag.is_set():
				try:
					global ser
					if ser.isOpen():
						reading = ser.readline().decode("utf-8").rstrip()
						process_serial(reading)

				except:
					pass                                                                                                        
 

 
class pulsaCruz(threading.Thread):
##  '''VERIFICAR SE A THREAD RODA SEM O PCF8574 INSTALADO''' 
	def __init__(self):
		threading.Thread.__init__(self)
					
 
	def run(self):      
		for a in range(20):
			ip = leds.LedsWifi[config.CRUZ_1[0][0]]["IP"]
			velocidade = config.CRUZ_1[0][1]
			color = config.CRUZ_1[0][2]
			cmd="flux_led " + ip + " -C gradual " + velocidade + " " + color
			p = subprocess.Popen(cmd, shell=True, stdin=subprocess.PIPE,\
				 stdout=subprocess.PIPE, stderr=subprocess.STDOUT,close_fds=False)
			bulbs = p.stdout.read().decode().rstrip()
			if not "Unable" in bulbs:
				print("Ok", "CRUZ", ip)
				break            
					
		time.sleep(config.CRUZ_1[1]["SLEEP_CRUZ"])
		for a in range(20):        
			velocidade = config.CRUZ_2[0][1]
			color = config.CRUZ_2[0][2]                 
			cmd="flux_led " + ip + " -C gradual " + velocidade + " " + color
			p = subprocess.Popen(cmd, shell=True, stdin=subprocess.PIPE,\
				 stdout=subprocess.PIPE, stderr=subprocess.STDOUT,close_fds=False)
			bulbs = p.stdout.read().decode().rstrip()
			if not "Unable" in bulbs:
				print("Ok", "CRUZ", ip)
				break

class Cortinas(threading.Thread):
##  '''VERIFICAR SE A THREAD RODA SEM O PCF8574 INSTALADO''' 
	def __init__(self, acao):
		threading.Thread.__init__(self)
		self.acao_cortina = acao            
 
	def run(self):
		global cortinaVitralSupDesceu
		global cortinaPortaSupDesceu
		global cortinaVitralInfDesceu
		global cortinaPortaInfDesceu
		global cortinaVitralSupSubiu
		global cortinaPortaSupSubiu
		global cortinaVitralInfSubiu
		global cortinaPortaInfSubiu        
		recebido = [0,0,0,0]
		duracao = time.time()
		if self.acao_cortina == "sobe":
			cortinaVitralSupSubiu = False
			cortinaPortaSupSubiu = False
			cortinaVitralInfSubiu = False
			cortinaPortaInfSubiu = False           
			counter = 0        
			while True:
				if duracao + 15000 > time.time():

					if cortinaVitralInfSubiu == False:
						infot = mqttc.publish("CORTINA_INFERIOR", "SOBE_VITRAL", qos=2)
	
					else:
						if recebido[0] == 0:
							print("CORTINA_INFERIOR VITRAL ACIONADA")
							recebido[0] = 1
					time.sleep(.5)
					if cortinaPortaInfSubiu == False:
						infot = mqttc.publish("CORTINA_INFERIOR", "SOBE_PORTA", qos=2)
					else:
						if recebido[1] == 0:                        
							print("CORTINA_INFERIOR PORTA ACIONADA")
							recebido[1] = 1                                           
					time.sleep(.5)
					if cortinaVitralSupSubiu == False:
						infot = mqttc.publish("CORTINA_SUPERIOR", "SOBE_VITRAL", qos=2)
					else:
						if recebido[2] == 0:                        
							print("CORTINA_SUPERIOR VITRAL ACIONADA") 
							recebido[2] = 1
					time.sleep(.5)
					if cortinaPortaSupSubiu == False:            
						infot = mqttc.publish("CORTINA_SUPERIOR", "SOBE_PORTA", qos=2)
					else:
						if recebido[3] == 0:                        
							print("CORTINA_SUPERIOR PORTA ACIONADA")
							recebido[3] = 1
					time.sleep(2)
					
					if recebido == [1,1,1,1]:
						break
				else:
					#telegram_bot.envia_telegram_single("334240998", "Cortina sobe nao acionada")
					break    
	
		if self.acao_cortina == "desce":
			print("desce")
			cortinaVitralSupDesceu = False
			cortinaPortaSupDesceu = False
			cortinaVitralInfDesceu = False
			cortinaPortaInfDesceu = False
			while True:
				if duracao + 15000 > time.time():
					if cortinaVitralInfDesceu == False:
						infot = mqttc.publish("CORTINA_INFERIOR", "DESCE_VITRAL", qos=2)
					else:
						if recebido[0] == 0:
							print("CORTINA_INFERIOR VITRAL ACIONADA")
							recebido[0]=1
	
					time.sleep(.5)
					if cortinaPortaInfDesceu == False:
						infot = mqttc.publish("CORTINA_INFERIOR", "DESCE_PORTA", qos=2)
					else:
						if recebido[1] == 0:
							print("CORTINA_INFERIOR PORTA ACIONADA")
							recebido[1]=1
											  
					time.sleep(.5)
					if cortinaVitralSupDesceu == False:
						infot = mqttc.publish("CORTINA_SUPERIOR", "DESCE_VITRAL", qos=2)
					else:
						if recebido[2] == 0:
							print("CORTINA_SUPEIOR VITRAL ACIONADA")
							recebido[2] = 1
											   
					time.sleep(.5)
					if cortinaPortaSupDesceu == False:            
						infot = mqttc.publish("CORTINA_SUPERIOR", "DESCE_PORTA", qos=2)
					else:
						if recebido[3] == 0:
							print("CORTINA_SUPERIOR PORTA ACIONADA")
							recebido[3]=1
					time.sleep(2)
					if recebido == [1,1,1,1]:
						break
				else:
					#~ telegram_bot.envia_telegram_single("334240998", "Cortina desce nao acionada")
					break


class Leds:
	def __init__(self):
		self.wait = 0
		self.LedsWifi = config.CONTROLADORES
		self.inicio = time.time() + 20
		#self.send_telegram_single = "" # variavel sentenciada para enviar telegramas
		#self.send_telegram_all = "" # variavel sentenciada para enviar telegramas
		thread1 = threading.Thread(target=self.verifica_ip)
		thread1.start()
	def getIp(self, device):
		'''
		Busca o ip do controlador led pelo nome
		'''
		for k, v in self.LedsWifi.items():
			#print(k)
			if k == device:
				return self.LedsWifi[k]['IP']

	def verifica_ip(self):
		mac_list = []
		for k, v in self.LedsWifi.items():
			
			mac_list.append(self.LedsWifi[k]['MAC'])
			#print("mac_list",mac_list)
			self.LedsWifi[k]['COUNTER'] = 0 # CONTADOR PARA VERIFICAR LED OnLINE  
			self.LedsWifi[k]['MSG_ENVIADA'] = False # CONTADOR PARA VERIFICAR LED ODDLINE  
		global iniciada
		new_controller = False
		while True:
			try:
				cmd="flux_led -s"
				p = subprocess.Popen(cmd, shell=True, stdin=subprocess.PIPE,\
				stdout=subprocess.PIPE, stderr=subprocess.STDOUT,close_fds=False)
				bulbs = p.stdout.readlines()
				del bulbs[0]
				if len(self.LedsWifi) == len(bulbs):
					self.wait = 1
			
				for itens in bulbs:
#                    print("itens bulbs encontrados", itens)
					itens = itens.decode().rstrip()
					if "  " in itens:
						itens = itens.replace("  ","")
					endereco_mac = itens.split(" ")[0]
					endereco_ip = itens.split(" ")[1]    
					for k, v in self.LedsWifi.items():
							
						if self.LedsWifi[k]['MAC'] in endereco_mac:
							self.LedsWifi[k]['IP'] = endereco_ip
							self.LedsWifi[k]['COUNTER'] = 0
							
							if have_internet() == "Conectado" and self.LedsWifi[k]['MSG_ENVIADA'] == True and iniciada == False:
								#~ telegram_bot.envia_telegram_all("{}: controlador wifi {} online novamente.".format(config.CAPELA, k))                        
								self.LedsWifi[k]['MSG_ENVIADA'] = False                            
								
						if self.LedsWifi[k]['COUNTER'] >= config.TENTATIVAS_LED:
							
							if have_internet() == "Conectado" and self.LedsWifi[k]['MSG_ENVIADA'] == False and iniciada == False:
								#~ telegram_bot.envia_telegram_all("{}: controlador wifi {} offline.".format(config.CAPELA, k))                     
								self.LedsWifi[k]['MSG_ENVIADA'] = True
								self.LedsWifi[k]['COUNTER'] = 0
						else:
							self.LedsWifi[k]['COUNTER'] += 1
							
						if not endereco_mac in mac_list and new_controller ==  False:
							print("Novo Controlador encontrado", endereco_mac)
							new_controller = True
					
						   
					
							
						# ~ else:
							# ~ print("Novo controlador encontrado", endereco_mac, endereco_ip)
				time.sleep(self.wait)
			except Exception as e:
				print("exception check ",e)


class ServiceExit(Exception):
	"""
	Custom exception which is used to trigger the clean exit
	of all running threads and the main program.
	"""
	pass

	 
def service_shutdown(signum, frame):
	print('Caught signal %d' % signum)
	print("passou")
	raise ServiceExit
 
 
def main():
	global iniciada
	global videoDuration
	global intervalo
	global lastIntervalo
	global CONFIGURACAO
	global passedScenario1
	global passedScenario2
	# Register the signal handlers
	signal.signal(signal.SIGTERM, service_shutdown)
	signal.signal(signal.SIGINT, service_shutdown)
 
	print('Starting main program')

	# Start the job threads
	buttonPressed(1)
	try:
		
		#~ for itens in config.luzUrna:
			
			#~ J2=Job(itens[0],itens[1], itens[2])
			#~ J2.start()
			#~ sleep(.1)       
		
		pass        
		time_video = player.position()
		ler_serial = readSerial()     
		ler_serial.start()

		looping = time.time()
		lastLooping = time.time()
		time_cortinas = time.time()

		while True:
			if mqttLocal.desligar == True:
				time.sleep(2)
				os.system("sudo poweroff")
			if time.time() > time_cortinas:
				pass
				# ~ if status_cortina == False:
					# ~ time_cortinas = time.time()+2000
                  

			if player.playback_status() == "Playing" and looping < time.time():
				time_video = player.position()
				looping = time.time() + .5
				
				if time_video >= inicio_fumaca and time_video <= inicio_fumaca + 1:
					ser.write(b'fum,12000\n')			
									
					
				if time_video >= inicio_chuva_petalas and time_video <= inicio_chuva_petalas +1:
					print("chuva_petalas")					
					ser.write(b'chuvapetalas\n')
					ser.write(b'<0,0,0,0,0,100,0,0,0,100>\n')
						
				if time_video >= inicio_sequencia and time_video <= inicio_sequencia + 1:
					print(" sequencia")					
					ser.write(b'sequencia\n')
					time.sleep(1)
					ser.write(b'LED_CABINE,255,255,0\n')
					                   
									
				if time_video >= inicio_pulsa_cruz and time_video <= inicio_pulsa_cruz + 1:
					print("pulsa cruz")
					ser.write(b'EN_RGB_GERAL,1\n')# habilita o loop pulsante vermelho
					time.sleep(1)
					ser.write(b'[100,0,0,100,0,0,5,0,0,5,0,0]\n')
					
				if time_video >= fim_pulsa_cruz and time_video <= fim_pulsa_cruz + 1:
					print(" desabilita pulsa cruz")
					ser.write(b'EN_RGB_GERAL,0\n')
					time.sleep(1)					
					ser.write(b'(0,0,0,0,0,0,0,0,0)\n')
					
				if time_video >= acende_scenario and time_video <= acende_scenario + 1:
					print(" fim de cerimonia")					
					ser.write(b'<0,100,0,0,0,0,0,0,0,0>\n')
					time.sleep(1)					
					ser.write(b'(255,255,255,0,0,0,255,255,255)\n')				
					
				print(time_video, end = '\r')               
                      
											   
			if player.can_control() and time_video > videoDuration - 2:
				time.sleep(.1)
				while player.playback_status() != "Paused":
					player.pause()										

				#videoDuration = player.duration()
				time.sleep(.5)

				iniciada = False
			

 
	except ServiceExit:
		pass
		# Terminate the running threads.
		# Set the shutdown flag on each thread to trigger a clean shutdown of each thread.
##        j1.shutdown_flag.set()
		
		# Wait for the threads to close...
##        j1.join()
	   
 
	print('Exiting main program')

	
#telegram_bot = Bot(Config.CAPELA)

#~ try:
	#~ if have_internet() == "Conectado":
		#~ telegram_bot.envia_telegram_single("334240998", "CERIMONIA INICIADA")
#~ except:
	#~ print("ERRO TELEGRAM")


CONFIGURACAO = {}
cortinaVitralSupDesceu = False
cortinaVitralSupSubiu = False
cortinaPortaSupDesceu = False
cortinaPortaSupSubiu = False
cortinaVitralInfDesceu = False
cortinaVitralInfSubiu = False
cortinaPortaInfDesceu = False
cortinaPortaInfSubiu = False
iniciada = False
passedScenario1 = False
passedScenario2 = False
intervalo = []

for a in range(6):
	a = datetime.datetime.now()
	intervalo.append(a)
	
global lastIntervalo
lastIntervalo = []

for a in range(6):    
	lastIntervalo.append(intervalo[a])
videoDuration = 0

##flux_led 192.168.0.104 -C gradual 90 '(255,0,0)'"
os.system("sudo killall omxplayer.bin")
#~ try:
	#~ p = subprocess.Popen('sudo killall omxplayer.bin', stdout=subprocess.PIPE, shell=True)
#~ except:
	#~ pass
logging.basicConfig(level=logging.INFO)

source = "/usr/share/APP/videos/video_1.mp4"
try:
	player_log = logging.getLogger("Player 1")
	player = OMXPlayer(source, args=[  '-o', 'local', '--no-osd','-b'], dbus_name='org.mpris.MediaPlayer2.omxplayer1')
	player.set_aspect_mode('stretch')
except NameError:# dbus.exceptions.DBusException:
	print("Reiniciando App")
	os.execv(sys.executable, ['python3'] + sys.argv)
else:
	print("Exception")

	
#~ player.set_aspect_mode('fill')

#time.sleep(20)
while player.playback_status() != "Paused":
	player.pause()
tempo = time.time()
print("video pausado")
player.set_volume(volume)
print("Volume " + str(player.volume()))
print("O vídeo tem %.2f segundos de duração "%player.duration())



ser = serial.Serial()
ser.port = '/dev/ttyUSB0'
ser.baubrate = 9600
try:
	ser.open()
except:
	print("Porta serial não disponível")	
mqttLocal = Mosquitto("capela/campo_santo")
mqttLocal.player = player
mqttLocal.serial_port = ser
mqttServer = MosquittoServer("capela/campo_santo")
mqttServer.player = player
mqttServer.serial_port = ser
leds = Leds()


@socketio.on("message")
def received_socket(str):
	'''
	Funçao chamada pela página web e/ou pelo app
	'''
	if str == "teste_estanqueidade":
		teste = Thread(target=realiza_teste_estanqueidade)
		teste.start()


	if str == "troca_gasosa":
		troca_sch()

@app.route("/")
def index():
	"""
	Página principal, onde são mostrados os dados em tempo real.
	"""
	global thread
	global thread_email
	global thread_online
	#ser.write(b'LH2S,1\n')
	# ~ if thread is None:
		# ~ pass

		# ~ thread = Thread(target=background_thread)
		# ~ thread.start()
	# ~ if thread_online is None:       
		# ~ thread_online = Thread(target=onlineServices)
		# ~ thread_online.start()
			
	# ~ if thread_email is None:
		# ~ thread_email = Thread(target=sch_email)
		# ~ thread_email.start()
	os.system("sudo cp /home/pi/Crabit/instance/imagens/LOGO_CEM.png /home/pi/Crabit/vilatec/static/LOGO_CEM.png")
	# ~ totalRows = getTotalSepultados()
	# ~ serial = getSerial()
	totalRows = 10
	serial = "Pedro"	
	return render_template("index.html", quant=totalRows, serial=serial[9:16].upper())

"""
OPERAÇÕES DE USUÁRIO
"""
@app.route("/cadastrarUsuario")
def cadastrarUsuario():
	"""
	Função de redirecionamento para a página de cadastro de usuário
	"""
	return render_template("cadastroUsuario.html")


@app.route("/cadastroUsuario", methods=["GET", "POST"])
def cadastroUsuario():
	"""
	Função que realiza o cadastro de usuário no banco, com as informações fornecidas na paǵina de cadastro de usuário
	"""
	if request.method == "POST":
		nome = request.form.get("nome")
		email = request.form.get("email")

		if nome and email:
			usuario = Usuario(nome, email)
			db.session.add(usuario)
			db.session.commit()

	return redirect(url_for("index.html"))


@app.route("/listaUsuarios")
def listaUsuarios():
	"""
	Função que mostra na tela a lista de usuários cadastrados
	"""
	usuarios = Usuario.query.all()
	return render_template("listaUsuarios.html", usuarios=usuarios)


@app.route("/excluirUsuario/<int:_id>")
def excluirUsuario(_id):
	"""
	Função para realizar a remoção de um usuário do sistema
	"""
	usuario = Usuario.query.filter_by(_id=_id).first()
	db.session.delete(usuario)
	db.session.commit()

	return redirect(url_for("listaUsuarios"))


@app.route("/editarUsuario/<int:_id>", methods=["GET", "POST"])
def editarUsuario(_id):
	"""
	Função para realizar edição nos dados de um usuário
	"""
	usuario = Usuario.query.filter_by(_id=_id).first()

	if request.method == "POST":
		nome = request.form.get("nome")
		email = request.form.get("email")

		if nome and email:
			usuario.nome = nome
			usuario.email = email
			db.session.commit()

		return redirect(url_for("listaUsuarios"))

	return render_template("editarUsuario.html", usuario=usuario)


"""
OPERAÇÕES DE SEPULTADOS
"""
@app.route("/cadastrarSepultado")
def cadastrarSepultado():
	"""
	Função de redirecionamento para a página de cadastro de sepultado
	"""
	return render_template("cadastroSepultado.html")


@app.route("/cadastroSepultado", methods=["GET", "POST"])
def cadastroSepultado():
	"""
	Função para cadastrar um sepultado
	"""
	if (request.method == "POST"):
		nome = request.form.get("nome")
		data_sepult = request.form.get("data_sepult")
		hora = request.form.get("hora_sepult")
		hora_sepult = datetime.time(int(hora[:2]), int(hora[3:]))
		ano = data_sepult[6] + data_sepult[7] + data_sepult[8] + data_sepult[9]
		mes = data_sepult[3] + data_sepult[4]
		dia = data_sepult[0] + data_sepult[1]
		data_obj = datetime.date(int(ano), int(mes), int(dia))
		loculo = request.form.get("loculo")

		if (nome and data_sepult and hora_sepult and loculo):
			c = Obito(nome, data_obj, hora_sepult, loculo)
			hc = Historico_Obito(nome, data_obj, hora_sepult, loculo)
			db.session.add(c)
			db.session.add(hc)
			db.session.commit()
			if telegram_bot:
				telegram_bot.envia_telegram_all("Cadastro de sepultamento:\nNome:\n%s\nLoculo:\n%sHora:\n%s"%(nome, loculo, hora_sepult))
			if sheet and have_internet() == 'Conectado':
				sheet.updateListBody([loculo, nome, data_sepult, hora])

	return redirect(url_for("listaSepultados"))


@app.route("/editarSepultado/<int:_id>", methods=["GET", "POST"])
def editarSepultado(_id):
	"""
	Função para editar os dados de um sepultado
	"""
	sepultado = Obito.query.filter_by(_id=_id).first()
	historico_sepultado = Historico_Obito.query.filter_by(_id=_id).first()
	if request.method == "POST":
		nome = request.form.get("nome")
		data_sepult = request.form.get("data_sepult")
		hora = request.form.get("hora_sepult")
		hora_sepult = datetime.time(int(hora[:2]), int(hora[3:5]))
		ano = data_sepult[6] + data_sepult[7] + data_sepult[8] + data_sepult[9]
		mes = data_sepult[3] + data_sepult[4]
		dia = data_sepult[0] + data_sepult[1]
		data_obj = datetime.date(int(ano), int(mes), int(dia))
		loculo = request.form.get("loculo")

		if nome and data_sepult and hora_sepult and loculo:
			sepultado.nome = nome
			sepultado.data_sepultamento = data_obj
			sepultado.hora_sepultamento = hora_sepult
			sepultado.loculo = loculo
			if sheet and have_internet() == 'Conectado':
				sheet.updateBody(historico_sepultado.loculo, [loculo, nome, data_sepult, hora])
			if telegram_bot:
				telegram_bot.envia_telegram_all("Edicao de dados sepultado:\nNome:\n%s\nLoculo:\n%sHora:\n%s"%(nome, loculo, hora_sepult))
			historico_sepultado.nome = nome
			historico_sepultado.data_sepultamento = data_obj
			historico_sepultado.hora_sepultamento = hora_sepult
			historico_sepultado.loculo = loculo

			db.session.commit()

			return redirect(url_for("listaSepultados"))

	dataFormatada = formatarData(sepultado.data_sepultamento)
	horaFormatada = formatarHora(sepultado.hora_sepultamento)
	return render_template("editarSepultado.html", sepultado=sepultado, data=dataFormatada, hora=horaFormatada)


@app.route("/excluirSepultado/<int:_id>")
def excluirSepultado(_id):
	"""
	Função para realizar a remoção de um sepultado do banco
	"""
	sepultado = Obito.query.filter_by(_id=_id).first()
	if sheet and have_internet() == 'Conectado':
		sheet.deleteBody(str(sepultado.loculo))
	db.session.delete(sepultado)
	db.session.commit()
	if telegram_bot:
		telegram_bot.envia_telegram_all("Sepultado excluído:\n%s"%(sepultado.loculo))
	return redirect(url_for("listaSepultados"))


@app.route("/listaSepultados")
def listaSepultados():
	"""
	Função para demontrar a lista de sepultados
	"""
	sepultados = Obito.query.all()
	return render_template("listaSepultados.html", sepultados=sepultados)
 
@app.route("/params")
def params():
	arg1 = request.args['arg1']
	arg2 = request.args['arg2']
	print(arg1, arg2)
	devices = mosquitto.dispositivosMqtt
#
	return render_template("listaValvulas.html", devices=devices) 
		
@app.route("/listaValvulas")
def listaValvulas():
	"""
	Função para demontrar a lista de sepultados
	"""
	
	devices = mosquitto.dispositivosMqtt
#
	return render_template("listaValvulas.html", devices=devices)    


@app.route("/listaHistoricoSepultados")
def listaHistoricoSepultados():
	"""
	Função para demonstrar o histórico de sepultados
	"""
	historico_sepultados = Historico_Obito.query.all()
	return render_template("listaHistoricoSepultados.html", sepultados=historico_sepultados)


"""
OPERAÇÕES DE VALOR IDEAL
"""
@app.route("/cadastrarValorIdeal")
def cadastrarValorIdeal():
	"""
	Função de redirecionamento para página de cadastro de valor ideal
	"""
	sensores = Tipo_Sensor.query.all()
	return render_template("cadastroValorIdeal.html", sensores=sensores)


@app.route("/cadastroValorIdeal", methods=["GET", "POST"])
def cadastroValorIdeal():
	"""
	Função para cadastrar um novo valor ideal
	"""
	if request.method == "POST":
		nome_sensor = request.form.get("nome_sensor")
		valor_ideal = request.form.get("valor_ideal")
		intervalo = request.form.get("intervalo")
		if nome_sensor and valor_ideal and intervalo:
			valor = Sensor(nome_sensor, valor_ideal, intervalo)
			db.session.add(valor)
			db.session.commit()
			if sheet and have_internet() == 'Conectado':
				sheet.setValuesParameters()
	return redirect(url_for("listaValoresIdeais"))


def registerValues(values):
	values_len, sensor_len = len(values), len(Config.SENSORS_NAME)
	if values_len == sensor_len:
		for index in range(values_len):
			sensor = Config.SENSORS_NAME[index]
			value_db = Sensor.query.filter_by(nome=sensor).first()
			if float(value_db.valor_ideal) == float(values[index]):
				pass
			else:
				value_db.valor = float(values[index])
				db.session.commit()
	else:
		print("ideal values - list out of range")


@app.route("/editarValorIdeal/<string:nome>", methods=["GET", "POST"])
def editarValorIdeal(nome):
	"""
	Função para editar os dados de um valor ideal
	"""
	sensor = Sensor.query.filter_by(nome=nome).first()
	if request.method == "POST":
		nome_sensor = request.form.get("nome_sensor")
		valor = request.form.get("valor_ideal")
		intervalo = request.form.get("intervalo")
		if nome_sensor and valor and intervalo:
			sensor.nome = nome_sensor
			sensor.valor_ideal = valor
			sensor.intervalo = intervalo
			db.session.commit()
			if sheet and have_internet() == 'Conectado':
				sheet.setValuesParameters()
			return redirect(url_for("listaValoresIdeais"))
	return render_template("editarValorIdeal.html", sensor=sensor)


@app.route("/excluirValorIdeal/<int:_id>")
def excluirValorIdeal(_id):
	"""
	Função para realizar a remoção de um valor ideal
	"""
	valor = Sensor.query.filter_by(_id=_id).first()
	nome = valor.nome
	db.session.delete(valor)
	db.session.commit()
	if sheet and have_internet() == 'Conectado':
		sheet.deleteValueParameter(nome)

	return redirect(url_for("listaValoresIdeais"))

@app.route("/listaValoresIdeais")
def listaValoresIdeais():
	"""
	Função para listar os valores ideais cadastrados
	"""
	sensores = Sensor.query.all()
	return render_template("listaValoresIdeais.html", sensores=sensores)


"""
OPERAÇÕES COM SENSORES
"""
@app.route("/listaSensores")
def listaSensores():
	"""
	Função para listar na tela os sensores cadastrados
	"""
	sensores = Tipo_Sensor.query.all()
	return render_template("listaSensores.html", sensores=sensores)


@app.route("/cadastrarSensor")
def cadastrarSensor():
	"""
	Função de redirecionamento para a página de cadastro de um novo sensor
	"""
	return render_template("cadastroSensor.html")


@app.route("/cadastroSensor", methods=["GET", "POST"])
def cadastroSensor():
	"""
	Função de cadastro de um novo sensor
	"""
	if request.method == "POST":
		id = request.form.get("id")
		tipo = request.form.get("tipo")

		if id and tipo:
			sensor = Tipo_Sensor(id, tipo)
			db.session.add(sensor)
			db.session.commit()

	return redirect(url_for("listaSensores"))


@app.route("/excluirSensor/<string:_id>")
def excluirSensor(_id):
	"""
	Função para realizar a remoção de um sensor
	"""
	sensor = Tipo_Sensor.query.filter_by(_id=_id).first()
	db.session.delete(sensor)
	db.session.commit()

	return redirect(url_for("listaSensores"))


@app.errorhandler(Exception)
def unhandled_exception(e):
	"""
	Função que mostra na tela uma determinada página quando ocorre qualquer tipo de erro
	"""
	app.logger.error("Unhadled exception: %s" % (e))
	return render_template("error.html")

# ABRE PORTA TEMPO DECORRIDO :30997 14:22:43
# FECHA_PORTA 47462
# Dados serial  EXPONDO MESA, 46853
# Dados serial  GUARDANDO MESA, 47180


#encoding: utf-8
#!/usr/bin/env python3.7
#TIME RECORTE 275.788, 291.04, 313.537
# SANCA FRENTE MACEIO = PINO 6
from serial_port import SerialThread
import jsondiff as diff
from instance.config_json import Config
from fnmatch import fnmatch
from operations.mqtt import MosquittoServer, Mosquitto
import linecache
from datetime import datetime
import time
import threading
import signal
import serial
import serial.tools.list_ports
import vlc
import json
#from omxplayer.player import OMXPlayer
#from operations.telegram import Bot
from pathlib import Path
from time import sleep
import logging
import subprocess
import os
import sys
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5 import uic, QtWidgets
os.system("xdotool windowminimize $(xdotool getactivewindow)")
def printException():
	exc_type, exc_obj, tb = sys.exc_info()
	f = tb.tb_frame
	lineno = tb.tb_lineno
	filename = f.f_code.co_filename
	linecache.checkcache(filename)
	line = linecache.getline(filename, lineno, f.f_globals)
	print ('EXCEPTION IN ({}, LINE {} "{}"): {}'.format(filename, lineno, line.strip(), exc_obj))
ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
class Window(QMainWindow):
	def __init__(self):
		super(Window, self).__init__() # Call the inherited classes __init__ method
		ui_file = os.path.join(ROOT_DIR, 'UI','tela_inicial.ui')
		uic.loadUi(ui_file, self)
		#self.setWindowFlags(Qt.WindowStaysOnTopHint | Qt.FramelessWindowHint)
		self.label_msg.hide()
		self.player_position = 0
		self.volume = 1
		self.send_cmd_fade = False
		self.etapas_cerimonia = 0
		self.enabledPlayer = False
		self.comando_serial = None
		self.flag_comando_serial = False
		self.mediaPlayer = None
		self.send_debug = time.time() + 3
		self.ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
		self.inicio_chuva_petalas = 218
		self.acende_scenario = 365
		self.inicio_pulsa_cruz = 339
		self.fim_pulsa_cruz = 352
		self.inicio_sequencia = 0
		self.inicio_fecha_porta = 0
		self.inicio_fumaca = 0
		self.apaga_cruz = self.fim_pulsa_cruz + 8
		self.conf_instance = Config()
		self.config = self.conf_instance.json_config['CONFIG']
		self.cerimonia_normal = False
		self.entrega = False
		self.change_video = False
		self.mqttLocal = Mosquitto(self.config['TOPICO_IN_SERVER'],self.config['PORTA_MQTT'])
		self.mqttLocal.updateVideo = self.update_video
		self.timer_comando_serial = 0
		self.reset_timers()
		self.media_name = ""



		self.serial_instance = SerialThread("MEGA")
		self.serial_instance.serial_mega_enabled.connect(self.on_serial_open)
		self.serial_instance.serial_read.connect(self.process_serial)
		self.serial_instance.start()

		monitor = QDesktopWidget().screenGeometry(1)
		self.move(monitor.left(), monitor.top())

		try:
			for children in self.findChildren(QtWidgets.QWidget):
				if isinstance(children, QtWidgets.QPushButton):
						children.clicked.connect(self.on_button)
		except:
			printException()
		#self.showFullScreen()
		#self.show()
		self.showMinimized()
		print("Show minimized")

	def send_serial(self, data):
		self.serial_instance.serial_mega.write(data.encode('utf-8', "ignore"))

	def on_serial_open(self, flag):
		print("Iniciando o media player")
		self.load_player()
		print("Iniciando o main")
		self.start_main()
		self.mqttLocal.serial_mega = self.serial_instance.serial_mega
		self.mqttLocal.serial_nano = self.serial_instance.serial_nano

	def start_main(self):
		try:
			print("start_main")
			self.timer = QTimer()
			self.timer.setInterval(0.01)
			self.timer.timeout.connect(self.main)
			self.timer.start()
		except:
			printException()

	def on_button(self):
		if self.sender().text() == "RESET_MEGA":

			self.serial_instance.serial_mega.setDTR(True)
			time.sleep(1)
			self.serial_instance.serial_mega.setDTR(False)
			# self.serial_instance.serial_mega.flushInput()


		elif self.sender().text() == "ABREPORTA":
			self.serial_instance.abre_porta = True
		elif self.sender().text() == "FECHAPORTA":
			self.serial_instance.fecha_porta = True
		elif self.sender().text() == "GUARDAMESA":
			self.serial_instance.guarda_mesa = True
		elif self.sender().text() == "EXPOEMESA":
			self.serial_instance.expoe_mesa = True

		elif self.sender().text() == "MARCA VIDEO":
			if self.mediaPlayer.get_fullscreen() == 1:
				self.mediaPlayer.set_fullscreen(False)
			else:
				self.mediaPlayer.set_fullscreen(True)

			# self.lineedit_marcavideo.setText(self.lineedit_marcavideo.text()+ ", "+str(self.player_position))

		elif self.sender().text() == "ASPECT_RATIO":
			self.mediaPlayer.video_set_aspect_ratio(self.lineedit_aspect_2.text())

		elif self.sender().text() == "PLAY/PAUSE":
			print("play/pause")
			if self.mediaPlayer.get_state() == vlc.State.Paused:
				self.mediaPlayer.play()
			elif self.mediaPlayer.get_state() == vlc.State.Playing:
				self.mediaPlayer.pause()

		elif self.sender().text() == "ESCALA":
			self.mediaPlayer.video_set_scale(float(self.lineedit_escala_2.text()))

		elif self.sender().text() == "PARA":
			self.serial_instance.abre_porta = False
			self.serial_instance.fecha_porta = False
			self.serial_instance.guarda_mesa = False
			self.serial_instance.expoe_mesa = False
			dados = self.sender().text()+'\n'
			self.serial_instance.serial_mega.write(dados.encode('utf-8', "ignore"))

		elif self.serial_instance.serial_mega is not None and self.serial_instance.serial_mega.isOpen():
			dados = self.sender().text()+'\n'
			self.serial_instance.serial_mega.write(dados.encode('utf-8', "ignore"))
		print(self.sender().text())


	def load_player(self):
		try:
			filename_player = self.config["VIDEO_LOCAL"]['URL']
			URL = os.path.join(self.ROOT_DIR, "videos", filename_player)
			# print("FILENAME PLAYER", filename_player)
			Player = vlc.Instance('--loop', '--aout=alsa', '--video-on-top')
			self.mediaList = Player.media_list_new()
			path = os.path.join(self.ROOT_DIR, "videos")
			songs = os.listdir(path)
			for s in songs:
				if any(s[-4:] in item for item in self.config["VIDEO_FILES"]):
					self.mediaList.add_media(Player.media_new(os.path.join(path,s)))
			self.listPlayer = Player.media_list_player_new()
			self.mediaPlayer = self.listPlayer.get_media_player()


			if sys.platform.startswith("linux"):  # for Linux using the X Server
				#self.alsa = alsaaudio.Mixer(alsaaudio.mixers()[2])
				#self.alsa.setvolume(100)
				#self.mediaPlayer.set_xwindow(self.frame_video.winId())
				...

			elif sys.platform == "win32":  # for Windows
				self.mediaPlayer.set_hwnd(self.frame_video.winId())

			elif sys.platform == "darwin":  # for MacOS
				self.mediaplayer.set_nsobject(self.frame_video.winId())
			self.mediaPlayer.toggle_fullscreen()
			# print("has video out", self.mediaPlayer.has_vout())
			self.listPlayer.set_media_list(self.mediaList)
			#self.mediaList.add_media(Player.media_new("/home/pi/CAPELA/cerimonia/Post.mp4")) # adiciona nova midia na lista
			for value in self.mediaList.__iter__():
				if value.get_meta(0) == filename_player:
					self.media_name = value.get_meta(0)
					self.listPlayer.play_item_at_index(self.mediaList.index_of_item(value))
					break
			#time.sleep(1)

			# print("CMD",self.mediaPlayer.get_xwindow())
			mediaplayer_vlc_event_manager = self.mediaPlayer.event_manager()
			listPlayer_vlc_event_manager = self.listPlayer.event_manager()
			# self.listPlayer.set_playback_mode(vlc.PlaybackMode.loop)
			# listPlayer_vlc_event_manager.event_attach(vlc.EventType.MediaListPlayerPlayed, playlistFinished)

			mediaplayer_vlc_event_manager.event_attach(vlc.EventType.MediaPlayerTimeChanged, self.durationChanged_vlc)
			mediaplayer_vlc_event_manager.event_attach(vlc.EventType.MediaPlayerEncounteredError, self.printError)
			# mediaplayer_vlc_event_manager.event_attach(vlc.EventType.MediaPlayerMediaChanged, playlistPositionChanged)
			# mediaplayer_vlc_event_manager.event_attach(vlc.EventType.MediaDurationChanged, positionChanged_vlc)

			self.mediaPlayer.video_set_aspect_ratio("16:9")
			time.sleep(.5)
			print("DURACAO DO VÍDEO {}", self.mediaPlayer.get_length()/1000)
			print("size video", self.mediaPlayer.video_get_size())
			print("audio", self.mediaPlayer.audio_output_device_enum())
			# self.mediaPlayer.video_set_logo_int(vlc.VideoLogoOption.logo_enable, 1)
			# self.mediaPlayer.video_set_logo_string(vlc.VideoLogoOption.logo_file, r"/usr/share/rpd-wallpaper/road.jpg")
			# self.mediaPlayer.video_set_logo_string(vlc.VideoLogoOption.logo_x, "500")
			# self.mediaPlayer.video_set_logo_string(vlc.VideoLogoOption.logo_y, "500")
			# self.mediaPlayer.video_set_logo_int(vlc.VideoLogoOption.logo_opacity, 0)
			# self.mediaPlayer.video_set_logo_int(vlc.VideoLogoOption.logo_position, 0)
			# self.mediaPlayer.video_set_logo_int(vlc.VideoLogoOption.logo_repeat, 1000)

			time.sleep(0.1)
			if self.config["VIDEO_LOCAL"]['URL'] == "Locução Masculino.mp4" or self.config["VIDEO_LOCAL"]['URL'] == "Locução Feminino.mp4":
				print("volume reduzido")
				self.mediaPlayer.audio_set_volume(60)
			else:
				print("volume normal")
				self.mediaPlayer.audio_set_volume(80)
				time.sleep(1)
			while self.mediaPlayer.get_state() != vlc.State.Paused:
				self.mediaPlayer.pause()
				time.sleep(0.3)
			self.mqttLocal.player = self.mediaPlayer
			# ~ self.start_main()
			# ~ while self.mediaPlayer.get_state() != vlc.State.Paused:
				# ~ self.mediaPlayer.pause()
				# ~ time.sleep(0.3)
			self.mqttLocal.player = self.mediaPlayer
			# ~ self.start_main()
			# ~ thread_1 = threading.Thread(target=self.main)
			# ~ thread_1.start()
		except:
			printException()
			print("Não foi possível carregar o vídeo {}".format(filename_player))

	def printError(self, error):
		print(error)

	def update_code(self):
		pattern = "*.py"
		lista_de_arquivos = []
		for path, subdirs, files in os.walk(self.ROOT_DIR):
			for name in files:
				if fnmatch(name, pattern):
					if not ".git" in path:
						full_path = os.path.join(path, name)
						cmd_turned = os.popen("date -r " + full_path).read().rstrip()
						lista_de_arquivos.append ([full_path, cmd_turned])
		while True:
			leitura_atual = []
			for path, subdirs, files in os.walk(self.ROOT_DIR):
				for name in files:
					if fnmatch(name, pattern):
						if not ".git" in path:
							full_path = os.path.join(path, name)
							cmd_turned = os.popen("date -r " + full_path).read().rstrip()
							leitura_atual.append ([full_path, cmd_turned])
			if lista_de_arquivos != leitura_atual:
				print("reiniciando app")
				os.execv(sys.executable, ['python3'] + sys.argv)
			time.sleep(5)





	def update_video(self, json_data):
		'''Configura os vídeos enviados pelo ihm'''
		URL = ""
		new_config = json_data["CONFIG"]
		try:
			if self.config["VIDEO_LOCAL"]['URL'] == "Locução Masculino.mp4" or self.config["VIDEO_LOCAL"]['URL'] == "Locução Feminino.mp4":
				print("volume reduzido")
				self.mediaPlayer.audio_set_volume(50)
			else:
				print("volume normal")
				self.mediaPlayer.audio_set_volume(70)

			if new_config['VIDEO_EXTERNO']['ENABLE'] == True:
				filename_player = self.config["VIDEO_LOCAL"]['URL']

			self.config.update(json_data['CONFIG'])
			self.conf_instance.salva_config(json_data)
			time.sleep(1)
			self.config["VIDEO_LOCAL"]['URL']
			for value in self.mediaList.__iter__():
				if value.get_meta(0) == self.config["VIDEO_LOCAL"]['URL']:
					# print(value.get_meta(0), self.mediaList.index_of_item(value))
					self.media_name = value.get_meta(0)
					self.listPlayer.play_item_at_index(self.mediaList.index_of_item(value))
					time.sleep(0.5)
					while self.mediaPlayer.get_state() != vlc.State.Paused:
						self.mediaPlayer.pause()
						time.sleep(0.3)
					break

		except Exception as ex:
			printException()
		time.sleep(.1)
	def update_gui(self, dados):
			self.lineedit_mecanismo.setText(dados)
			self.label_data.setText(datetime.now().strftime("%d/%m - %H:%M:%S"))

	def monta_comando_serial(self, comando):
		#utilizado nos comandos que requerem retorno
		self.comando_serial = comando+'+\n'
		self.flag_comando_serial = True

	def auto_send_serial(self):
			if self.timer_comando_serial < time.time():
				self.timer_comando_serial = time.time() + .5
				if self.flag_comando_serial == True:
					self.serial_instance.serialNanoWrite(self.comando_serial.encode('utf-8', "ignore"))
					self.flag_comando_serial = False
					print(self.comando_serial.replace("\n", ""), "Finalizado")

	def process_serial(self, dados):
		# if self.enabledPlayer == False:
		# 	self.enabledPlayer = True
		# 	print("Iniciando o media player")
		# 	self.load_player()
		# 	print("Iniciando o main")
		# 	self.start_main()
		# 	self.mqttLocal.serial_instance = self.serial_instance
		if dados != "":
			try:
				if dados == 'CONFIGURAR CENARIO':
					self.serial_instance.serial_mega.write(b'<0,0,100,0,0,0,100,0,0,0>\n')
					# print("\a")
					# duration = 1  # seconds
					# freq = 440  # Hz
					# os.system('aplay -nq -t alsa synth {} sine {}'.format(duration, freq))
					self.serial_instance.serial_mega.write(b'<0,0,100,0,0,0,100,0,0,0>\n')

				if self.comando_serial is not None and self.comando_serial.replace("\n", "")	in dados:
					# ~ print(self.comando_serial.replace("\n", ""), dados.replace("\n", ""), self.comando_serial.replace("\n", "") == dados.replace("\n", ""))
					# termina o envio em loop para porta Serial
					print("desabilitou envio serial", self.comando_serial.replace("\n", ""))
					self.flag_comando_serial = False

				if 'S_PA' in dados:
					self.send_debug = time.time() + 3
					self.update_gui(dados)

				else:
					print("Dados serial ",dados, datetime.now().strftime("%H:%M:%S"))
			except:
				printException()

			if dados == 'button_counter = 2':
				self.serial_instance.serial_nano.write(b'(0,0,0,0,0,0)\n')
				cmd = {"mute":"mute"}
				cmd = json.dumps(cmd)
				self.mqttLocal.mqttc.publish(self.config["TOPICO_INT_IHM"], cmd, qos=2)
				pass

			if dados == 'GUARDAMESA':
				# print("player position", self.player_position)
				pass

			if 'ABRINDO A PORTA' in dados:
				print("Player position abre porta", self.player_position)
				pass

			if 'FECHA PORTA' in dados:
				print("Player position inicio fechamento", self.player_position)
				pass

			if 'PORTA FECHADA' in dados:
				print("Player position final fechamento", self.player_position)
				pass

			if dados == 'ESTEIRA DESLIGADA':
				self.serial_instance.serial_mega.write(b'LED_ESTEIRA,0\n')

			if dados == 'CERIMONIA_COMPLETA':
				if self.cerimonia_normal == False:
					self.cerimonia_normal = True
					# try:
						# telegram_bot.envia_telegram_single("334240998", "CERIMONIA INICIADA")
					# except:
						# print("ERRO TELEGRAM")
					self.serial_instance.serial_mega.write(b'INICIADO\n')
					self.reset_timers()
					print(self.config["CEMITERIO"])

					time.sleep(.1)
					cmd = {"display_off":"display_off"}
					cmd = json.dumps(cmd)
					self.mqttLocal.mqttc.publish(self.config["TOPICO_INT_IHM"], cmd, qos=2)
					self.mediaPlayer.set_time(0)
					self.mediaPlayer.audio_set_volume(100)
					# self.mediaPlayer.set_time(180*1000)
					self.player_position = 0
					time.sleep(0.5)
					self.mediaPlayer.play()
					time.sleep(0.5)
					time.sleep(1)


					self.serial_instance.serial_mega.write(b'LED_URNA,1\n')
					self.serial_instance.serial_mega.write(b'APAGAR\n')
					self.serial_instance.serial_nano.write(b'APAGAR\n')

					if self.config['VIDEO_EXTERNO']['ENABLE'] == True:
						self.change_video = True
					else:
						...
					time.sleep(1)
			if dados == 'CERIMONIA_ENTREGA':
				self.serial_instance.serial_mega.write(b'INICIO_CERIMONIA\n')			
				self.reset_timers()
				if self.config["CEMITERIO"] == "VIDA":#desligar os balizadores da rampa
					serial_mega.write(b'GPIO,42,0\n')
				# ~ try:
					# ~ telegram_bot.envia_telegram_single("334240998", "CERIMONIA INICIADA")
				# ~ except:
					# ~ print("ERRO TELEGRAM")
				self.serial_instance.serial_mega.write(b'<0,0,0,0,0,0,0,0,0,0>\n')
				time.sleep(.02)
				self.serial_instance.serial_mega.write(b'(0,0,0,0,0,0,0,0,0)\n')
				self.serial_instance.serial_nano.write(b'(0,0,0,0,0,0)\n')
				cmd = {"display_off":"display_off"}
				cmd = json.dumps(cmd)
				self.mqttLocal.mqttc.publish(self.config["TOPICO_INT_IHM"], cmd, qos=2)
				self.mediaPlayer.set_position(0)
				self.mediaPlayer.play()
				time.sleep(1)
				self.cerimonia_normal = False
				self.entrega = True						


	def main(self):
		try:
			if self.send_debug < time.time():
				'''
				Rotina para reativar o debug em caso de perda do flag pelo microcotrolador
				'''
				print("REATIVANDO DEBUG")
				self.serial_instance.serial_mega.write(b'DEBUG,1\n')
				self.send_debug = time.time() + 3

			self.auto_send_serial()
			if self.config['VIDEO_EXTERNO']['ENABLE'] == True and self.change_video == True:
				...
				# if player1.position() > player.duration() - 0.2:
					# self.change_video = False
					# player1.pause()
					# player1.hide_video()
					# player.show_video()
			if self.mqttLocal.desligar == True:
				time.sleep(2)
				os.system("sudo poweroff")

			if self.cerimonia_normal:

				if self.player_position >= self.inicio_fumaca:
					self.inicio_fumaca += 10000
					self.serial_instance.serial_mega.write(b'fum,12000\n')
					self.serial_instance.serial_mega.write(b'LED_CABINE,128,255,0,255,255,255,0,255,0\n')

				if self.player_position >= self.inicio_chuva_petalas:
					self.inicio_chuva_petalas += 10000
					print("chuva_petalas")
					self.serial_instance.serial_mega.write(b'CHUVAPETALAS,255,60\n')
					# self.serial_instance.serial_mega.write(b'LED_ESTEIRA,1\n')
					# self.serial_instance.serial_mega.write(b'LED_URNA,1\n')


				if self.player_position >= self.inicio_sequencia:
					if self.etapas_cerimonia == 0:
						if self.serial_instance.S_PA == 0:

							self.serial_instance.abre_porta = True
							if self.send_cmd_fade == False:
								self.send_cmd_fade = True

						else:
							self.send_cmd_fade = False
							self.etapas_cerimonia += 1

					if self.etapas_cerimonia == 1:
						if self.serial_instance.S_MG == 0:

							self.serial_instance.guarda_mesa = True
							if self.send_cmd_fade == False:
								self.send_cmd_fade = True
						else:
							self.send_cmd_fade = False
							self.etapas_cerimonia += 1


					if self.etapas_cerimonia == 2:
						if self.serial_instance.S_PF == 0 and self.serial_instance.S_MG == 1:
							self.serial_instance.fecha_porta = True
							if self.send_cmd_fade == False:
								self.send_cmd_fade = True
								self.serial_instance.serial_mega.write(b'LED_URNA,0\n')

						else:
							self.etapas_cerimonia = 0
							self.inicio_sequencia += 10000

				if "Cachoeiras" in self.media_name:
					if self.player_position >= self.inicio_pulsa_cruz:
						self.inicio_pulsa_cruz += 10000
						print("pulsa cruz")
						self.serial_instance.serial_mega.write(b'EN_RGB_GERAL,1\n')# habilita o loop pulsante vermelho
						# self.serial_instance.serial_mega.write(b'T_RGB, 10\n')
						time.sleep(.5)
						self.serial_instance.serial_mega.write(b'(255,0,0,255,0,0,0,0,0)\n')
						#self.serial_instance.serial_mega.write(b'[100,0,0,100,0,0,5,0,0,5,0,0]\n') # casoo não esteja gravado na eeprom
						time.sleep(1)

					if self.player_position >= self.fim_pulsa_cruz:
						self.fim_pulsa_cruz += 10000
						print(" desabilita pulsa cruz")
						self.serial_instance.serial_mega.write(b'EN_RGB_GERAL,0\n')
						self.serial_instance.serial_mega.write(b'(0,0,0,255,0,0,0,0,0)\n')
						# self.serial_instance.serial_mega.write( b'(0,0,0,0,0,0,0,0,0,255,0,0)\n')
						# self.serial_instance.serial_mega.write(b'T_RGB, 20\n')

				if self.player_position >= self.apaga_cruz:
					self.apaga_cruz += 10000
					self.serial_instance.serial_mega.write( b'(0,0,0,0,0,0,0,0,0)\n')

				if self.player_position >= self.acende_scenario and self.player_position <= self.acende_scenario + 1:
					self.acende_scenario += 10000
					self.serial_instance.serial_mega.write(b'T_DIM,100\n')
					time.sleep(1)
					self.serial_instance.serial_mega.write(b'(255,255,255,255,255,255,0,0,0)\n')
					self.serial_instance.serial_mega.write(b'GPIO,42,1\n')#ligar os balizadores da rampa
					print(" fim de cerimonia")
					print("Condicao para pausar", self.player_position, self.mediaPlayer.get_length()/1000  - 2)
					# self.serial_instance.serial_mega.write(b'<0,100,100,100,100,0,100,0,0,0>\n')
					thread_2 = threading.Thread(target=self.liga_apos_cerimonia)
					thread_2.start()
				print(self.player_position, end='\r')

			if self.entrega:
				if self.player_position >= self.inicio_fumaca:
					self.inicio_fumaca += 10000
					self.serial_instance.serial_mega.write(b'fum,15000\n')

				if self.player_position >= self.inicio_sequencia:
					self.inicio_sequencia += 10000
					print(" sequencia")
					self.serial_instance.serial_mega.write(b'LED_CABINE,128,255,0,255,255,255,0,255,0\n')
					self.serial_instance.serial_mega.write(b'ENTREGA\n')
					self.serial_instance.serial_mega.write(b'LED_URNA,200\n')

				if self.player_position >= self.inicio_pulsa_cruz:
					self.inicio_pulsa_cruz += 10000
					print("pulsa cruz")
					self.serial_instance.serial_mega.write(b'EN_RGB_GERAL,1\n')# habilita o loop pulsante vermelho
					time.sleep(.5)
					self.serial_instance.serial_mega.write(b'(255,0,0,255,0,0,0,0,0)\n')
					#serial_mega.write(b'[100,0,0,100,0,0,5,0,0,5,0,0]\n')
					time.sleep(1)

				if self.player_position >= self.fim_pulsa_cruz:
					self.fim_pulsa_cruz += 10000
					print(" desabilita pulsa cruz")
					self.serial_instance.serial_mega.write(b'EN_RGB_GERAL,0\n')
					# ~ time.sleep(1)
					
					
					if self.config["CEMITERIO"] == "VIDA":
						#mantem apenas a cruz vermelha
						self.serial_instance.serial_mega.write( b'(0,0,0,255,0,0,0,0,0)\n')
					else:
						self.serial_instance.serial_mega.write( b'(0,0,0,0,0,0,0,0,0)\n')
				if self.config["CEMITERIO"] == "VIDA":
					if  self.player_position  >= self.apaga_cruz:
						self.apaga_cruz += 10000
						self.serial_instance.serial_mega.write( b'(0,0,0,0,0,0,0,0,0)\n')
						
				if self.player_position >= self.acende_scenario and self.player_position <= self.acende_scenario + 1:
					print(" fim de cerimonia")
					self.serial_instance.serial_mega.write(b'T_DIM,100\n')
					# ~ self.serial_instance.serial_mega.write(b'<100,100,100,100,100,100,100,100,100,100>\n')#luz no fim da cerimonia
					time.sleep(1)
					self.serial_instance.serial_mega.write(b'(255,255,255,255,255,255,0,0,0)\n')
					if self.config["CEMITERIO"] == "VIDA":#ligar os balizadores da rampa
						self.serial_instance.serial_mega.write(b'GPIO,42,1\n')
						self.serial_instance.serial_mega.write(b'LED_URNA,0\n')
					thread_2 = threading.Thread(target=self.liga_apos_cerimonia)
					thread_2.start()					

				
			if self.mediaPlayer.get_state() != vlc.State.Paused and self.player_position > self.mediaPlayer.get_length()/1000  - 2:
				print("pausando video")
				self.mediaPlayer.pause()
				self.cerimonia_normal = False
				self.entrega = False
				if self.serial_instance.serial_mega is not None and self.serial_instance.serial_mega.isOpen():
					self.serial_instance.serial_mega.write(b'FINAL_CERIMONIA\n')

		except ServiceExit:
			pass

	def reset_timers(self):
		self.inicio_chuva_petalas = 215
		self.acende_scenario = 365
		self.inicio_pulsa_cruz = 339
		self.fim_pulsa_cruz = 352
		self.inicio_sequencia = self.inicio_chuva_petalas - 9
		self.inicio_fumaca = self.inicio_chuva_petalas - 5 - 5
		self.apaga_cruz = self.fim_pulsa_cruz + 8

	def durationChanged_vlc(self, event):

		try:
			self.player_position = self.mediaPlayer.get_time()/1000
			# print("player get time", self.player_position)
			# self.player_position = event.u.new_time/1000
			# print(self.player_position)
		except Exception as e:
			print("Exception get time", e)


	def liga_apos_cerimonia(self):
		time.sleep(5)
		self.serial_instance.serial_mega.write(b'DIMER,1,100\n')
		self.serial_instance.serial_nano.write(b'(255,255,255,255,255,255)')
		time.sleep(2)
		self.serial_instance.serial_mega.write(b'DIMER,2,100\n')
		time.sleep(2)
		self.serial_instance.serial_mega.write(b'DIMER,3,100\n')
		time.sleep(2)
		self.serial_instance.serial_mega.write(b'DIMER,4,100\n')
		time.sleep(2)
		self.serial_instance.serial_mega.write(b'DIMER,5,100\n')
		time.sleep(2)
		self.serial_instance.serial_mega.write(b'DIMER,6,100\n')




if __name__ == '__main__':
	App = QApplication(sys.argv)
	window = Window()
	App.exit(App.exec())
	# sys.exit(App.exec())


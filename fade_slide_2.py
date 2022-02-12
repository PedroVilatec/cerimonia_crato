from PyQt5.QtWidgets import *
from PyQt5.QtCore import (pyqtSignal, pyqtSlot, Q_ARG, QAbstractItemModel, QVariantAnimation, QEasingCurve, QEventLoop, QVariant,
		QFileInfo, qFuzzyCompare, QMetaObject, QModelIndex, QObject, Qt,
		QThread, QTime, QUrl, QCoreApplication, QThread, pyqtSignal, QTimer)
from PyQt5.QtGui import (qRgb, QColor, qGray, QTouchEvent, QColor, QImage, QMovie, QPainter, QPen, QPalette, )
from PyQt5 import QtCore, QtGui, uic
import sys, os
import glob

from video_cerimonia_vlc import printException
ROOT_DIR = os.path.dirname(os.path.abspath(__file__))

class AnimationLabel(QLabel):
	next_image = pyqtSignal(bool)
	def __init__(self, *args, **kwargs):
		QLabel.__init__(self, *args, **kwargs)
		self.unfade_mode = False
		self.effect = QGraphicsOpacityEffect()
		self.setGraphicsEffect(self.effect )
		self.animation = QtCore.QPropertyAnimation(self.effect, b"opacity")
		self.animation.setDuration(1500)
		self.animation.finished.connect(self.animationFinished)
		self.fadeDirection = 0

	def fade(self):
		self.unfade_mode = False
		self.animation.setStartValue(1)
		self.animation.setEndValue(0)
		self.fadeDirection = 0
		self.animation.start()

	def unfade(self):
		self.unfade_mode = True
		self.animation.setStartValue(0)
		self.animation.setEndValue(1)
		self.fadeDirection = 1
		self.animation.start()


	@pyqtSlot()
	def animationFinished( self ):
		if self.unfade_mode == False:
			self.next_image.emit(True)
		# if self.fadeDirection == 0 :
		# 	self.changeImage()
		# else :
		# 	self.nextImage()

class Widget(QWidget):
	def __init__(self):
		super().__init__()
		# self.setWindowFlags(Qt.FramelessWindowHint)
		self.setAttribute(Qt.WA_NoSystemBackground, True)
		self.setAttribute(Qt.WA_TranslucentBackground, True)
		lay = QVBoxLayout(self)
		self.cnt_image = 0
		folder = os.path.join(ROOT_DIR,"img")
		self.files = []
		self.pixmap = []
		for ext in (["*.jpg", "*.png"]):
			self.files.extend(glob.glob(os.path.join(folder, ext)))
		for index in range(len(self.files)):
			print("Imagem file", self.files[index])
			self.im = QImage(self.files[index])
			self.pixmap.append(QtGui.QPixmap(QtGui.QPixmap.fromImage(self.im)))
			self.pixmap[index] = self.pixmap[index].scaledToWidth(600, Qt.SmoothTransformation)
		self.greeting_text = AnimationLabel()
		self.greeting_text.setPixmap(self.pixmap[0])
		self.greeting_text.next_image.connect(self.next_image)
		lay.addWidget(self.greeting_text)
		btnFadeIn = QPushButton("fade in")
		btnFadeOut = QPushButton("fade out")
		lay.addWidget(btnFadeIn)
		lay.addWidget(btnFadeOut)
		btnFadeIn.clicked.connect(self.greeting_text.fade)
		btnFadeOut.clicked.connect(self.greeting_text.unfade)

		self.timer = QTimer(self)
		self.timer.setInterval(10000)
		self.timer.timeout.connect(self.recurring_timer)
		self.timer.start()

	def next_image(self):
		if self.cnt_image == len(self.pixmap) -1:
			self.cnt_image = 0
			self.greeting_text.setPixmap(self.pixmap[self.cnt_image])
			self.greeting_text.unfade()

		else:
			try:
				self.cnt_image += 1
				self.greeting_text.setPixmap(self.pixmap[self.cnt_image])
				self.greeting_text.unfade()

			except Exception as e:
				printException()

	def recurring_timer(self):
		self.greeting_text.fade()

if __name__ == '__main__':
	app = QApplication(sys.argv)
	ex = Widget()
	ex.show()
	sys.exit(app.exec_())
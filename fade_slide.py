from PyQt5.QtWidgets import *
from PyQt5.QtCore import (pyqtSignal, pyqtSlot, Q_ARG, QAbstractItemModel, QVariantAnimation, QEasingCurve, QEventLoop, QVariant,
		QFileInfo, qFuzzyCompare, QMetaObject, QModelIndex, QObject, Qt,
		QThread, QTime, QUrl, QCoreApplication, QThread, pyqtSignal, QTimer)
from PyQt5.QtGui import (qRgb, QColor, qGray, QTouchEvent, QColor, QImage, QMovie, QPainter, QPen, QPalette, )
from PyQt5 import QtCore, QtGui, uic
import sys, os
ROOT_DIR = os.path.dirname(os.path.abspath(__file__))

class AnimationLabel(QLabel):
	def __init__(self, *args, **kwargs):
		QLabel.__init__(self, *args, **kwargs)
		self.animation = QVariantAnimation()
		self.animation.valueChanged.connect(self.changeColor)

	@pyqtSlot(QVariant)
	def changeColor(self, color):
		palette = self.palette()
		palette.setColor(QPalette.WindowText, color)
		self.setPalette(palette)

	def startFadeIn(self):
		self.animation.stop()
		self.animation.setStartValue(QColor(0, 0, 0, 0))
		self.animation.setEndValue(QColor(0, 0, 0, 255))
		self.animation.setDuration(2000)
		self.animation.setEasingCurve(QEasingCurve.InBack)
		self.animation.start()

	def startFadeOut(self):
		self.animation.stop()
		self.animation.setStartValue(QColor(0, 0, 0, 255))
		self.animation.setEndValue(QColor(0, 0, 0, 0))
		self.animation.setDuration(2000)
		self.animation.setEasingCurve(QEasingCurve.OutBack)
		self.animation.start()

	def startAnimation(self):
		self.startFadeIn()
		loop = QEventLoop()
		self.animation.finished.connect(loop.quit)
		loop.exec_()
		QTimer.singleShot(2000, self.startFadeOut)

class Widget(QWidget):
	def __init__(self):
		super().__init__()
		lay = QVBoxLayout(self)
		self.im = QImage(os.path.join(ROOT_DIR, "img", "frame_flores.jpg"))
		pixmap = QtGui.QPixmap(QtGui.QPixmap.fromImage(self.im))
		pixmap = pixmap.scaledToWidth(600, Qt.SmoothTransformation)
		self.greeting_text = AnimationLabel()
		self.greeting_text.setPixmap(pixmap)

		# self.greeting_text.setStyleSheet("font : 45px; font : bold; font-family : HelveticaNeue-UltraLight")
		lay.addWidget(self.greeting_text)
		btnFadeIn = QPushButton("fade in")
		btnFadeOut = QPushButton("fade out")
		btnAnimation = QPushButton("animation")
		lay.addWidget(btnFadeIn)
		lay.addWidget(btnFadeOut)
		lay.addWidget(btnAnimation)
		btnFadeIn.clicked.connect(self.greeting_text.startFadeIn)
		btnFadeOut.clicked.connect(self.greeting_text.startFadeOut)
		btnAnimation.clicked.connect(self.greeting_text.startAnimation)


if __name__ == '__main__':
	app = QApplication(sys.argv)
	ex = Widget()
	ex.show()
	sys.exit(app.exec_())
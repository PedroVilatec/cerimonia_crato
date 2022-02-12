from PyQt5 import QtGui
from PyQt5.QtWidgets import QApplication, QWidget, QFrame, QHBoxLayout, QPushButton
import sys


class Window(QWidget):
    def __init__(self):
        super().__init__()
        self.title = "PyQt5 Frame"
        self.top = 200
        self.left = 500
        self.width = 400
        self.height = 300
        self.setWindowIcon(QtGui.QIcon("icon.png"))
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)
        self.setStyleSheet('background-color:brown')
        hbox = QHBoxLayout()
        frame =QFrame(self)
        frame.setLineWidth(0.6)
        hbox.addWidget(frame)
        self.setLayout(hbox)
        self.show()



App = QApplication(sys.argv)
window = Window()
sys.exit(App.exec())

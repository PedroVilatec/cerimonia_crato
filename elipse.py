import OFT_MainWindow
import sys
import os
import numpy as np
import cv2
from PIL import Image, ImageQt, ImageEnhance
from PyQt5 import QtWidgets
from PyQt5 import QtGui
from PyQt5 import QtCore
from PyQt5.QtCore import Qt, QPoint
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.Qt import Qt

def main():
    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    window.show()
    app.exec_()

class MainWindow(QtWidgets.QMainWindow, OFT_MainWindow.Ui_MainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setupUi(self)
        self.pushButton.clicked.connect(self.browse_file)
        self.pushButton_2.clicked.connect(self.imageFrame)
        self.begin = QtCore.QPoint()
        self.end = QtCore.QPoint()
        self.ellipseItem = None
        self.show()

    def imageFrame(self):
        # ...
        scene.installEventFilter(self)

    def eventFilter(self, source, event):
        if event.type() == QtCore.QEvent.GraphicsSceneMousePress:
            for item in self.graphicsView.scene().items(event.scenePos()):
                if isinstance(item, QtWidgets.QGraphicsPixmapItem):
                    self.reference = item
                    self.ellipseItem = QtWidgets.QGraphicsEllipseItem(item)
                    self.ellipseItem.setBrush(QtGui.QColor(0, 255, 0, 25))
                    self.start = item.mapFromScene(event.scenePos())
        elif event.type() == QtCore.QEvent.GraphicsSceneMouseMove and self.ellipseItem:
            end = self.reference.mapFromScene(event.scenePos())
            self.ellipseItem.setRect(QtCore.QRectF(self.start, end))
        # elif event.type() == QtCore.QEvent.GraphicsSceneMouseRelease and self.ellipseItem:
        #     self.ellipseItem = None
        return super().eventFilter(source, event)

        ################################################################

    def browse_file(self):
        self.lineEdit.clear()
        file = QtWidgets.QFileDialog.getOpenFileName(self, "Choose file")
        file = str(file[0])
        if file:
            self.lineEdit.setText(file)   # добавить путь в lineEdit
#         cap = cv2.VideoCapture(file)

###################################################################################

    def imageFrame(self):
        file = self.lineEdit.text()
        self.drawing = False
        self.lastPoint = QPoint()
        scene = QtWidgets.QGraphicsScene(self)
        self.image = QPixmap(file)
        item = QtWidgets.QGraphicsPixmapItem(self.image)
        scene.addItem(item)
        view = self.graphicsView.setScene(scene)
#         self.show()

    def paintEvent(self, event):
        qp = QtGui.QPainter(self)
        br = QtGui.QBrush(QtGui.QColor(0, 255, 0, 25))
        qp.setBrush(br)
        ellipse = qp.drawEllipse(QtCore.QRect(self.begin, self.end))
        coord_a = self.begin
        coord_a = str(coord_a)
        coord_a = coord_a[20:][:-1]
        coord_b = self.end
        coord_b = str(coord_b)
        coord_b = coord_b[20:][:-1]
        coord = ('begin = ' + coord_a + ' end = ' + coord_b)
        self.lineEdit_2.setText(coord)

    def mousePressEvent(self, event):
        self.begin = event.pos()
        self.end = event.pos()
        self.update()

    def mouseMoveEvent(self, event):
        self.end = event.pos()
        self.update()

###################################################################################

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    window.show()
    app.aboutToQuit.connect(app.deleteLater)
    sys.exit(app.exec_())
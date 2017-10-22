from PyQt5.QtWidgets import QMainWindow, QPushButton, QFrame, QApplication, QLineEdit
from PyQt5.QtGui import QPainter
from PyQt5.QtCore import QEvent

import sys, signal
import init

class MyWindow(QMainWindow):

    def __init__(self):
        super().__init__()
        init.init(self)

        signal.signal(signal.SIGINT, self.sigIntHandler)
        
        self.show()

    #def mousePressEvent(self, mouse_event):
    #    self.event.mousePress(mouse_event, self)

    def keyPressEvent(self, key_event):
        self.event.keyPress(key_event, self)
        
    def paintEvent(self, event):
        self.note.update(self)

        painter = QPainter(self)
        self.note.paint(self, painter)
     
    def sigIntHandler(self, signal, frame):
        print('Call SigIntHandler')
        sys.exit(0)

if __name__=='__main__':

    app = QApplication(sys.argv)
    my_window = MyWindow()
    sys.exit(app.exec_())

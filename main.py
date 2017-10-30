from PyQt5.QtWidgets import QMainWindow, QPushButton, QFrame, QApplication, QLineEdit
from PyQt5.QtGui import QPainter
from PyQt5.QtCore import QEvent, QObject

import sys, signal, time
import play
import init

class MyWindow(QMainWindow):

    def __init__(self):
        super().__init__()
        init.init(self)

        signal.signal(signal.SIGINT, self.sigIntHandler)

        self.t = time.time()
        self.show()

    def mouseMoveEvent(self, mouse_event):
        self.event.mouseMove(mouse_event, self)

    def mousePressEvent(self, mouse_event):
        self.event.mousePress(mouse_event, self)

    def keyPressEvent(self, key_event):
        self.event.keyPress(key_event, self)
        
    def paintEvent(self, event):
        delta_t = 1000 * time.time() - self.t
        self.t = 1000 * time.time()
        self.note.forward_x_step = self.tempo / 60.0 * self.note.one_fourth_x * delta_t / 1000 * self.input_speed
        
        self.note.update(self)

        painter = QPainter(self)
        self.note.paint(self, painter)
     
    def sigIntHandler(self, signal, frame):
        print('Call SigIntHandler')
        sys.exit(0)

if __name__=='__main__':

    app = QApplication(sys.argv)

    my_window = MyWindow()

    if not app.exec_():
        my_window.play.stream.close()
        my_window.play.p.terminate()
        sys.exit(0)

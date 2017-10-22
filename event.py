from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import Qt, QPoint

class Event:

    def __init__(self):
        self.note_log = open('note.log', 'w')

    def keyPress(self, key_event, w):
        if w.mode == 1:
            if key_event.key() == Qt.Key_Return:
                self.note_log.write('{} '.format(w.note.now_x))

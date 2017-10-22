from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import Qt, QPoint

class Event:

    def __init__(self):
        self.note_log = open('note.log', 'w')

    def keyPress(self, key_event, w):
        if w.mode == 1:
            if key_event.key() == Qt.Key_Return: # 音符の追加
                w.note.note[w.note.note_num].append(['C1', w.note.now_x / w.note.one_x])

        if (key_event.modifiers() and Qt.ControlModifier):
            if key_event.key() == Qt.Key_S: # 保存
                print('save note.log')
                for n in w.note.note[w.note.note_num]:
                    l16 = 1 / 16.0
                    if n[1] % l16 < l16 / 2.0:
                        n[1] -= n[1] % l16
                    else:
                        n[1] += l16 - n[1] % l16
                    self.note_log.write('{},{} '.format(n[0], n[1]))

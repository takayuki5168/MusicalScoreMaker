from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import Qt, QPoint

class Event:

    def __init__(self):
        self.note_log = open('note.log', 'w')

    def mousePress(self, mouse_event, w):
        mouse_pos = mouse_event.pos()
        if (mouse_pos.y() > 550 and mouse_pos.y() < 700):
            for i in range(21):
                s_x = (900 - 60) / 21.0 * i + 60
                e_x = (900 - 60) / 21.0 * (i + 1) + 60
                if mouse_pos.x() > s_x and mouse_pos.x() < e_x:
                    print('po')
                    w.play.playPiano(i)
                    break
                

    def keyPress(self, key_event, w):
        # Ctrlが押されいている
        if (key_event.modifiers() and Qt.ControlModifier):
            if key_event.key() == Qt.Key_S: # 保存
                print('save note.log')
                f = open('note.log', 'a')
                f.write('{} {} {} {}\n'.format(w.octet_num, w.tempo, w.beat_deno, w.beat_nume))
                for n in w.note.note[w.note.modifying_part_id]:
                    n[1] /= w.note.one_x
                    n[2] /= w.note.one_x
                    f.write('{},{},{} '.format(n[0], n[1], n[2]))
                    print(n[0], n[1], n[2])                    
        # Ctrlが押されていない
        else:
            if w.mode == 0:
                if key_event.key() == Qt.Key_A:
                    w.note.now_x -= 10
                if key_event.key() == Qt.Key_D:
                    w.note.now_x += 20
            elif w.mode == 1:
                if key_event.key() == Qt.Key_Return: # 音符の追加
                    l16 = w.note.one_x / 16.0
                    tmp_x = w.note.now_x
                    if tmp_x % l16 < l16 / 2.0:
                        tmp_x -= tmp_x % l16
                    else:
                        tmp_x += l16 - tmp_x % l16
                    if len(w.note.note[w.note.modifying_part_id]) > 0:
                        w.note.note[w.note.modifying_part_id][-1][2] = tmp_x
                    w.note.note[w.note.modifying_part_id].append(['C1', tmp_x, -1])

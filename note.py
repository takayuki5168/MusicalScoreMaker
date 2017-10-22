from PyQt5.QtWidgets import QAction, QMenu
from PyQt5.QtGui import QPen, QColor

import color

class Note:

    def __init__(self, w):
        self.one_x = 200 #一分分音符の長さ
        self.one_fourth_x = self.one_x / 4.0
        self.one_eighth_x = self.one_x / 8.0

        self.now_x = 0
        self.forward_x_step = w.tempo / 60.0 * self.one_fourth_x * w.time_step / 1000 * w.input_speed
        self.max_score_x = 700
        self.min_score_x = 50

        self.note_one_x = []
        self.note_one_fourth_x = []
        self.note_color = color.black
        
    def update(self, w):
        if w.mode == 0: # 停止
            pass
        elif w.mode == 1: # 入力
            tmp_one_score_x = (50 - self.now_x) % self.one_x
            tmp_one_fourth_score_x = (50 - self.now_x) % self.one_fourth_x

            if (self.now_x - 50) % self.one_x < 10:
                self.note_color = color.red
            else:
                self.note_color = color.black

            # どこに音符を表示するか計算
            self.note_one_x = []
            self.note_one_fourth_x = []
            while True:
                if tmp_one_score_x > self.min_score_x:
                    self.note_one_x.append(tmp_one_score_x)
                if tmp_one_score_x> self.max_score_x:
                    break
                tmp_one_score_x += self.one_x
            while True:
                if tmp_one_fourth_score_x > self.min_score_x:
                    self.note_one_fourth_x.append(tmp_one_fourth_score_x)
                if tmp_one_fourth_score_x > self.max_score_x:
                    break
                tmp_one_fourth_score_x += self.one_fourth_x

            self.now_x += self.forward_x_step            
        elif w.mode == 2: # 再生
            pass

    def paint(self, w, painter):
        painter.setPen(QPen(self.note_color, 2))
        painter.drawArc(50 - 5, 400 - 5, 10, 10, 0, 16 * 360)

        painter.setPen(QPen(color.black, 2))
        for x in self.note_one_x:
            painter.drawArc(x - 5, 400 - 5, 10, 10, 0, 16 * 360)
        for x in self.note_one_fourth_x:
            painter.drawArc(x - 2, 400 - 2, 4, 4, 0, 16 * 360)            


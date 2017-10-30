from PyQt5.QtWidgets import QAction, QMenu, QLabel
from PyQt5.QtGui import QPen, QColor
from PyQt5.QtCore import QPoint

import math
import numpy as np

import color, event
import time#, pyaudio
import play

class Note:

    def __init__(self, w):
        self.one_x = 200.0 #一分分音符の長さ
        self.one_fourth_x = self.one_x / 4.0
        self.one_eighth_x = self.one_x / 8.0

        self.now_x = -10 # self.min_score_xが原点
        self.pre_x = 0
        self.forward_x_step = w.tempo / 60.0 * self.one_fourth_x * w.time_step / 1000 * w.input_speed
        self.max_score_x = 900
        self.min_score_x = 50

        self.note_one_x = []
        self.note_one_fourth_x = []
        self.note_color = color.black

        self.note = [[] for i in range(w.octet_num)]
        self.modifying_part_id = 0 # どのパートの編集中か
        self.note_y_center = 310
        self.sound_labels = []

        self.key_name = ['C1', 'D1', 'E1', 'F1', 'G1', 'A1', 'B1', 'C2', 'D2', 'E2', 'F2', 'G2', 'A2', 'B2', 'C3', 'D3', 'E3', 'F3', 'G3', 'A3', 'B3']
        self.key_name += ['C1s', 'D1s', 'E1s', 'F1s', 'G1s', 'A1s', 'B1s', 'C2s', 'D2s', 'E2s', 'F2s', 'G2s', 'A2s', 'B2s', 'C3s', 'D3s', 'E3s', 'F3s', 'G3s', 'A3s', 'B3s']
        self.key_name += ['C1f', 'D1f', 'E1f', 'F1f', 'G1f', 'A1f', 'B1f', 'C2f', 'D2f', 'E2f', 'F2f', 'G2f', 'A2f', 'B2f', 'C3f', 'D3f', 'E3f', 'F3f', 'G3f', 'A3f', 'B3f']
        print(self.key_name[21])

        # メトロノームの音の初期化
        self.m_sound1 = (np.concatenate([np.sin(np.arange(0.1 * 50000) * 440 * math.pow(2, 3 / 12.0) * math.pi * 2 / 44100)]) * 0.25).astype(np.float32).tostring()
        self.m_sound2 = (np.concatenate([np.sin(np.arange(0.1 * 50000) * 440 * math.pow(2, -9 / 12.0) * math.pi * 2 / 44100)]) * 0.25).astype(np.float32).tostring()

        self.t = 0

        #self.p = pyaudio.PyAudio()
        #self.stream = self.p.open(format = pyaudio.paFloat32, channels = 3, rate = 44100, output = 1)
        
    def update(self, w):
        if w.mode == 0: # 停止
            pass
        elif w.mode == 1: # 入力
            self.pre_x = self.now_x
            self.now_x += self.forward_x_step

            if self.pre_x % self.one_x > self.now_x % self.one_x:
                w.play.playPiano(9, 5)
                #self.stream.write(self.m_sound1)
            elif self.pre_x % self.one_fourth_x > self.now_x % self.one_fourth_x:
                w.play.playPiano(7, 5)
                #self.stream.write(self.m_sound2)
        elif w.mode == 2: # 再生
            self.pre_x = self.now_x
            self.now_x += self.forward_x_step

            for notes in self.note:
                for n in notes:
                    if n == []:
                        break
                    nx1 = self.min_score_x - self.now_x + n[1]
                    if n[2] == -1:
                        break
                    else:
                        nx2 = self.min_score_x - self.now_x + n[2]
                    if nx1 - self.min_score_x > 0 and nx1 - self.min_score_x < self.forward_x_step:
                        #w.play.play_chord(self.stream, n[0], nx2 - nx1)
                        #w.play.play_chord(self.stream, n[0], 1)
                        #w.play.setSoundLength(n[0], nx2 - nx1)
                        #w.play.setNextSound()
                        pass
        
        tmp_one_score_x = (50 - self.now_x) % self.one_x
        tmp_one_fourth_score_x = (50 - self.now_x) % self.one_fourth_x
      
      
        # どこに縦棒を表示するか計算
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


    def paint(self, w, painter):
        painter.setPen(QPen(color.blue, 2))
        painter.drawLine(QPoint(50, self.note_y_center - 200), QPoint(50, self.note_y_center + 200))

        # 縦棒の描画
        painter.setPen(QPen(color.black, 2))
        for x in self.note_one_x:
            painter.drawLine(QPoint(x, self.note_y_center - 30), QPoint(x, self.note_y_center + 30))
        for x in self.note_one_fourth_x:
            painter.drawLine(QPoint(x, self.note_y_center - 15), QPoint(x, self.note_y_center + 15))

        # 音符の描画
        for s in self.sound_labels:
            s.clear()
        self.sound_labels = []
        painter.setBrush(color.white)
        for notes in self.note:
            for i in range(len(notes)):
                if i == w.event.selected_note_id:
                    painter.setPen(QPen(color.blue, 3))
                elif i == w.event.on_note_id:
                    painter.setPen(QPen(color.red, 3))
                else:
                    painter.setPen(QPen(color.green, 3))
                n = notes[i]
                if n == []:
                    break
                nx1 = self.min_score_x - self.now_x + n[1]
                if n[2] == -1:
                    break
                    #nx2 = self.min_score_x - self.now_x
                else:
                    nx2 = self.min_score_x - self.now_x + n[2]
                if nx1 < self.max_score_x and nx2 > self.min_score_x:
                    if nx2 < self.max_score_x and nx1 > self.min_score_x:
                        sound_label = QLabel(n[0], w)
                        sound_label.move(nx1, self.note_y_center - 16)
                        #sound_label.setFixedWidth(10)
                        sound_label.setFixedWidth(20)
                        self.sound_labels.append(sound_label)
                        sound_label.setMouseTracking(True)
                        sound_label.show()

                    painter.drawRect(nx1, self.note_y_center - 8, nx2 - nx1, 16)

        # 鍵盤の描画
        painter.setPen(QPen(color.black, 2))
        painter.drawLine(QPoint(60, 550), QPoint(900, 550))
        painter.drawLine(QPoint(60, 700), QPoint(900, 700))
        painter.drawLine(QPoint(60, 550), QPoint(60, 700))
        painter.drawLine(QPoint(900, 550), QPoint(900, 700))
        painter.setBrush(color.white)
        for i in range(21):
            x = (900 - 60) / 21.0 * i + 60
            step = (900 - 60) / 21.0
            painter.drawRect(x, 550, step, 700 - 550)
        painter.setBrush(color.black)
        for i in range(21 - 1):
            if i % 7 == 2 or i % 7 == 6:
                continue
            x = (900 - 60) / 21.0 * (i + 0.8) + 60
            step = (900 - 60) / 21.0 * 0.4
            painter.drawRect(x, 550, step, 650 - 550)

        # 余計な部分を消す
        painter.setPen(QPen(color.origin, 2))
        painter.setBrush(color.origin)
        painter.drawRect(0, self.note_y_center -200, 50 - 1, 400)
        painter.drawRect(900, self.note_y_center -200, 960 - 900, 400)


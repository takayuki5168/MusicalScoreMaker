from PyQt5.QtWidgets import qApp
from PyQt5.QtCore import QTimer

import input, event, note

def init(w):
    initValue(w)
    initUI(w)
    initTimer(w)
    initEvent(w)

    initItem(w)

    w.setMouseTracking(True)

def initValue(w):
    w.mode = 0 # 0:停止 1:入力 2:再生
    w.octet_num = 1 # 何重奏
    w.tempo = 120 # テンポ
    w.input_speed = 1.0 # 何倍で入力するか
    w.beat_deno = 4 # 拍子の分母 not implemented
    w.beat_nume = 4 # 拍子の分子 not implemented
    
def initUI(w):
    initMenubar(w)
    initInput(w)

    w.setGeometry(200, 0, 960, 720)
    w.setWindowTitle('MusicalScoreMaker')

def initMenubar(w):
    pass
    #w.menu_bar = menubar.MenubarManager(w)

def initInput(w):
    input.init(w)
    
def initItem(w):
    w.note = note.Note(w)
    w.event = event.Event()


def initTimer(w):
    w.timer = QTimer(w)
    w.time_step = 10 # 10ms
    w.timer.timeout.connect(w.update)
    w.timer.start(w.time_step)
    
def initEvent(w):
    #w.event = event.Event()
    pass

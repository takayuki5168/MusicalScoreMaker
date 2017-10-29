from PyQt5.QtWidgets import QPushButton, QFileDialog

def setMode(w, toggled):
    source = w.sender()

    if source.text() == 'Pause':
        w.mode = 0
        w.button_input.setChecked(False)
        w.button_play.setChecked(False)
    elif source.text() == 'Input':
        if w.mode == 1:
            w.mode = 0
        else:
            w.mode = 1
        w.button_pause.setChecked(False)
        w.button_play.setChecked(False)
    elif source.text() == 'Play':
        if w.mode == 2:
            w.mode = 0
        else:
            w.mode = 2
        w.button_pause.setChecked(False)
        w.button_input.setChecked(False)            

def openNoteLog(w):
    if w.mode != 0:
        return
    file_name = QFileDialog.getOpenFileName(w, 'Open Note Log', '/home/private/hobby/musical_score_maker')[0]
    now_part_id = -1
    for line in open(file_name, 'r'):
        if now_part_id == -1:
            l = line.split(' ')
            w.octet_num = int(l[0])
            w.tempo = float(l[1])
            w.beat_deno = int(l[2])
            w.beat_nume = int(l[3])
            w.note.note = [[] for i in range(w.octet_num)]
            now_part_id += 1
            continue

        l = line.split(' ')[:-2]
        for note in l:
            n = note.split(',')
            w.note.note[now_part_id].append([n[0], float(n[1]) * w.note.one_x, float(n[2]) * w.note.one_x])
        now_part_id += 1
        

def po():
    return

def init(w):
    w.button_open = makePushButton('Open', w, 10, 10, 80, 90, lambda : openNoteLog(w), False)
    w.button_new = makePushButton('New', w, 90, 10, 80, 90, po, False)
    w.button_save = makePushButton('Save', w, 170, 10, 80, 90, po, False)
    
    w.button_pause = makePushButton('Pause', w, 260, 10, 50, 30, lambda t : setMode(w, t), True)
    w.button_input = makePushButton('Input', w, 260, 40, 50, 30, lambda t : setMode(w, t), True)
    w.button_play = makePushButton('Play', w, 260, 70, 50, 30, lambda t : setMode(w, t), True)
    
def makePushButton(name, w, x, y, width, height, func, check):
    button = QPushButton(name, w)
    button.setCheckable(check)
    if check == True:
        button.toggled.connect(func)
    else:
        button.clicked.connect(func)

    button.move(x, y)
    button.resize(width, height)

    return button

from PyQt5.QtWidgets import QPushButton

def setMode(w, toggled):
    source = w.sender()

    if source.text() == 'Pause':
        w.mode = 0
        w.button_input.setChecked(False)
        w.button_play.setChecked(False)
    elif source.text() == 'Input':
        print(toggled)
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

def po():
    return

def init(w):
    w.button_save = makePushButton('Open', w, 10, 10, 80, 90, po, False)
    w.button_save = makePushButton('New', w, 90, 10, 80, 90, po, False)
    w.button_save = makePushButton('Save', w, 170, 10, 80, 90, po, False)
    
    w.button_pause = makePushButton('Pause', w, 260, 10, 50, 30, lambda t : setMode(w, t), True)
    w.button_input = makePushButton('Input', w, 260, 40, 50, 30, lambda t : setMode(w, t), True)
    w.button_play = makePushButton('Play', w, 260, 70, 50, 30, lambda t : setMode(w, t), True)
    
def makePushButton(name, w, x, y, width, height, func, check):
    button = QPushButton(name, w)
    button.setCheckable(check)
    button.toggled.connect(func)

    button.move(x, y)
    button.resize(width, height)

    return button

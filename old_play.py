import numpy as np
import pyaudio
import math
import threading
import time

class Play(threading.Thread):
    def __init__(self):
        super(Play, self).__init__()
        self.key_name = ['C1', 'D1', 'E1', 'F1', 'G1', 'A1', 'B1', 'C2']
        self.key_diff = [-9, -7, -5, -4, -2, 0, 2, 3]
        self.key_freq = {}
        for key, diff in zip(self.key_name, self.key_diff):
            self.key_freq[key] = 440.0 * math.pow(2, diff / 12.0)
        self.chord_ = 'C2'
    
    # sin波生成
    def sine(self, freq, length, rate):
        factor = float(freq) * math.pi * 2 / rate
        return np.sin(np.arange(length * 1000) * factor)
    
    # 音源生成
    def chord(self, freq, lengths, rate):
        #1st
        res1 = np.array([])
        #for i in range(len(lengths[0])):
        res1 = np.r_[res1, self.sine(self.key_freq[freq], lengths, rate)]
    
        res = res1
        return res
        
    def play_chord(self, stream, freqs, lengths, rate = 44100):
        chunks = []
        chunks.append(self.chord(freqs, lengths, rate))
        chunk = np.concatenate(chunks) * 0.25
        stream.write(chunk.astype(np.float32).tostring())

    def set(self, stream):
        self.stream = stream

    def run(self):
        while True:
             self.play_chord(self.stream, self.chord_, 40)

    def setSound(self, chord_):
        self.chord_ = chord_

play = Play()
p = pyaudio.PyAudio()
stream = p.open(format = pyaudio.paFloat32, channels = 3, rate = 44100, output = 1)
play.set(stream)
play.start()
a = ['C1', 'D1', 'E1', 'F1', 'G1', 'C1', 'D1', 'E1', 'F1', 'G1', 'C1']
b = a[0]
cnt = 0
t = time.time()
for i in range(10000000):
    if i % 1000000 == 0:
        print('po')
        cnt += 1
        b = a[cnt]
        play.setSound(b)
print(time.time() - t)

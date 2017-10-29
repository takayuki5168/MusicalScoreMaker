import numpy as np
import pyaudio
import math

class Play():
    def __init__(self):
        super(Play, self).__init__()
        self.key_name = ['C1', 'D1', 'E1', 'F1', 'G1', 'A1', 'B1', 'C2', 'D2', 'E2', 'F2', 'G2', 'A2', 'B2', 'C3', 'D3', 'E3', 'F3', 'G3', 'A3', 'B3']
        self.key_diff = [-9, -7, -5, -4, -2, 0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24, 26]
        self.key_diff = [x - 12 for x in self.key_diff]
        self.key_freq = {}
        for key, diff in zip(self.key_name, self.key_diff):
            self.key_freq[key] = 440.0 * math.pow(2, diff / 12.0)
        self.pre_count = 0
        self.count = 0

        self.p = pyaudio.PyAudio()
        self.stream = self.p.open(format = pyaudio.paFloat32, channels = 3, rate = 44100, output = 1)
    
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
        
    def play_chord(self, freq, length, rate = 44100):
        chunks = []
        chunks.append(self.chord(freq, length, rate))
        chunk = np.concatenate(chunks) * 0.25
        self.stream.write(chunk.astype(np.float32).tostring())

    def playPiano(self, i, length = 100):
        self.play_chord(self.key_name[i], length)
    
    def test(self, sound):
        self.stream.write(sound)

import numpy as np
import pyaudio
import math

class Play():
    def __init__(self):
        self.key_name = ['C1', 'D1', 'E1', 'F1', 'G1', 'A1', 'B1', 'C2', 'D2', 'E2', 'F2', 'G2', 'A2', 'B2', 'C3', 'D3', 'E3', 'F3', 'G3', 'A3', 'B3']
        self.key_name += ['C1s', 'D1s', 'E1s', 'F1s', 'G1s', 'A1s', 'B1s', 'C2s', 'D2s', 'E2s', 'F2s', 'G2s', 'A2s', 'B2s', 'C3s', 'D3s', 'E3s', 'F3s', 'G3s', 'A3s', 'B3s']
        self.key_name += ['C1f', 'D1f', 'E1f', 'F1f', 'G1f', 'A1f', 'B1f', 'C2f', 'D2f', 'E2f', 'F2f', 'G2f', 'A2f', 'B2f', 'C3f', 'D3f', 'E3f', 'F3f', 'G3f', 'A3f', 'B3f']
        self.key_diff_root = [-9, -7, -5, -4, -2, 0, 2, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24, 26]
        self.key_diff_root = [x - 12 for x in self.key_diff_root]
        self.key_diff = self.key_diff_root + [x + 1 for x in self.key_diff_root] + [x - 1 for x in self.key_diff_root]
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

    def playPiano(self, i, length = 20):
        print(self.key_name[i])
        self.play_chord(self.key_name[i], length)
    
    def test(self, sound):
        self.stream.write(sound)

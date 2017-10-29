import numpy as np
import pyaudio
import math

key_name = ['C1', 'D1', 'E1', 'F1', 'G1', 'A1', 'B1', 'C2']
key_diff = [-9, -7, -5, -4, -2, 0, 2, 3]
key_freq = {}
for key, diff in zip(key_name, key_diff):
    key_freq[key] = 440.0 * math.pow(2, diff / 12.0)

# sin波生成
def sine(freq, length, rate):
    factor = float(freq) * math.pi * 2 / rate
    return np.sin(np.arange(length * 1000  * 300) * factor)

# 音源生成
def chord(freqs, lengths, rate):
    #1st
    res1 = np.array([])
    for i in range(len(lengths[0])):
        res1 = np.r_[res1, sine(key_freq[freqs[0][i]], lengths[0][i], rate)]
        res1 = np.r_[res1, sine(0, 0.03, rate)]

    res = res1
    return res
    
def play_chord(stream, freqs, lengths, rate = 44100):
    chunks = []
    chunks.append(chord(freqs, lengths, rate))
    chunk = np.concatenate(chunks) * 0.25
    stream.write(chunk.astype(np.float32).tostring())

# ログファイルから音符の長さの読み込み
#f = open('note.log', 'r')
f = 'test_note.log'
l = []
for line in open(f, 'r'):
    l.append(line)
point = [float(x.split(',')[1]) for x in l[1].split(' ') if x != '']
length = []
for i in range(len(point) - 1):
    length.append(point[i + 1] - point[i])
length.append(0.1)

l = []
for line in open(f, 'r'):
    l.append(line)
note = [x.split(',')[0] for x in l[1].split(' ') if x!= '']

lengths = [length, [], []]
notes = [note, [], []]

p = pyaudio.PyAudio()
stream = p.open(format = pyaudio.paFloat32, channels = 3, rate = 44100, output = 1)

play_chord(stream, notes, lengths)

stream.close()
p.terminate()

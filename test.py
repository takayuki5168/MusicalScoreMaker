import threading
import time


class SharedObject(object):
    def __init__(self):
        self.val = 0
        self.Lock = threading.Lock()
    
    def read(self):
        with self.Lock:
            return self.val

    def write(self, num):
        with self.Lock:
            self.val = num

class A(threading.Thread):
    def __init__(self, shared_object):
        super(A, self).__init__()
        self.shared_object = shared_object

    def run(self):
        while True:
            po = self.shared_object.read()
            if po % 10 == 0:
                print(po)

class B(threading.Thread):
    def __init__(self, shared_object):
        super(B, self).__init__()
        self.shared_object = shared_object
        self.i = 0

    def run(self):
        while True:
            self.i += 1
            print('-{}'.format(self.i))
            #time.sleep(0.1)
            self.shared_object.write(self.i)

v = SharedObject()
a = A(v)
b = B(v)
a.start()
b.start()
a.join()
b.join()


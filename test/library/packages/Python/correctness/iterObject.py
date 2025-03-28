

class MyCustomObject:
    def __init__(self):
        self.data = [1, 2, 3, 4, 5]
        self.index = 0

    def __iter__(self):
        return self

    def __next__(self):
        if self.index < len(self.data):
            self.index += 1
            return self.data[self.index - 1]
        else:
            raise StopIteration

def myCustomIterator():
    yield 1
    yield 2
    yield 3
    yield 4
    yield 5


class IAmNotIterable():
    def __init__(self):
        self.data = [1, 2, 3, 4, 5]
        self.index = 0

    def __next__(self):
        if self.index < len(self.data):
            self.index += 1
            return self.data[self.index - 1]
        else:
            raise StopIteration

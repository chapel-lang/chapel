

class MyRec:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        return 'MyRec(x={}, y={})'.format(self.x, self.y)


def printAndReturn(rec):
    assert isinstance(rec, MyRec)
    print(rec)
    return rec

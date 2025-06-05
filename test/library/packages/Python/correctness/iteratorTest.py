
def myiter(low, high):
    l = list()
    while low < high:
        l.append(low)
        low += 1
    return [i for i in l]

def mygen(low, high):
    while low < high:
        yield low
        low += 1


def round_trip(value):
    return value

class RoundTrip:
    def __init__(self, value):
        self.value = value
    def getter(self):
        return self.value
    def setter(self, value):
        self.value = value
